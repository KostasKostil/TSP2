#include "common.h"

static TSP tsp;
static vector<vector<int> > g;

static int n;
static int v1, vn;
static int branch_limit = -1;
static PermReverse* pr;
static unordered_set<int> changed;
static int num(int u, int v)
{
    return n*min(u, v) + max(u, v);
}
static int dynamic_branch_limit(int depth)
{
    if (depth <= branch_limit) return 1e9;
    else return 1;
}

static bool go(int balance, int depth = 0)
{
    if (balance - Dist(tsp, v1, vn) > 0)
    {
        pr->Apply();
        return true;
    }

    int branches = 0;
    for (int i : g[v1])
        if (i != vn)
        {
            int new_balance = balance - Dist(tsp, v1, i);
            if (new_balance <= 0)
                continue;
            if (!changed.count(num(v1, i)))
                if (i != pr->At(1))
                {
                    int pos = pr->Where(i);
                    int new_v1 = pr->At(pos-1);

                    if (changed.count(num(new_v1, i)))
                        continue;
                    changed.insert(num(v1, i));
                    changed.insert(num(new_v1, i));

                    pr->Reverse(0, pos);
                    int old_v1 = v1;
                    v1 = new_v1;

                    if (go(new_balance + Dist(tsp, new_v1, i), depth+1))
                        return true;

                    v1 = old_v1;
                    pr->Undo();

                    changed.erase(num(v1, i));
                    changed.erase(num(new_v1, i));

                    branches++;
                    if (branches >= dynamic_branch_limit(depth))
                        break;
                }
        }
    return false;
}

void FeasibleLKH(const TSP& _tsp, const vector<vector<int> >& _g, const vector<vector<AlphaEdge> >& G, vector<int>& tour)
{
    tsp = _tsp;
    g = _g;
    n = tsp.size();

    unordered_map<int, double> alpha;
    for (int i=0; i<n; i++)
        for (AlphaEdge e : G[i])
            alpha[num(i, e.v)] = e.alpha;

    pr = new PermReverse(tour);

    int upd = 0;
    int total = 0;
    int id = 0;
    vector<pair<int, int> > candidate_list;

    auto reset_candidates = [&]()->int
    {
        candidate_list.clear();
        upd = 0;
        total = 0;
        vector<pair<double, pair<int, int> > > temp;
        for (int i=0; i<n; i++)
        {
            int u = tour[i];
            int v = tour[(i+1)%n];

            if (alpha.count(num(u, v)))
                temp.pb({alpha[num(u, v)], {u, v} });
            else
                candidate_list.pb({u, v});
        }
        int ans = temp.size();
        sort(temp.begin(), temp.end());
        reverse(temp.begin(), temp.end());
        for (auto [x, pa] : temp)
            candidate_list.pb(pa);
        id = 0;
        return ans;
    };

    double lastOutputTime = -1e18;
    Visualizer vis;
    vis.SetTSP(tsp);

    reset_candidates();
    while (true)
    {
        if (id == n || (2*upd+1 < total && 4*upd > total+1))
        {
            if (upd > 0)
            {
                tour.clear();
                for (int i=0; i<n; i++)
                    tour.pb(pr->At(i));
                cout<<"length="<<Length(tsp, tour)<<" id="<<id<<" upd="<<upd<<" total="<<total;
                cout<<" myedges="<<reset_candidates()<<" branches="<<branch_limit<<"\n";
                double currentTime = Time();
                if (currentTime - lastOutputTime > 5.0)
                {
                    vis.SetCycle(255, 255, 255, tour);
                    lastOutputTime = currentTime;
                }

                ofstream fout("data/feasible_lkh.txt");
                for (int i : tour)
                    fout<<i<<"\n";
            }
        }
        if (id == n)
        {
            tour.clear();
            for (int i=0; i<n; i++)
                tour.pb(pr->At(i));
            branch_limit++;
            cout<<"BRANCH_LIMIT = "<<branch_limit<<"\n";
            id = 0;
            reset_candidates();
            continue;
        }

        int V = candidate_list[id].fi;
        int U = candidate_list[id].se;
        id++;

        int L = pr->Where(V);
        if (pr->At( (L+n-1)%n ) != U )
        {
            if (pr->At( (L+1)%n ) != U )
                continue;
            L=(L+1)%n;
        }

        pr->Cut(L);
        v1 = pr->At(0);
        vn = pr->At(n-1);
        changed.clear();

        total++;
        if (go(Dist(tsp, v1, vn)))
            upd++;
    }

    cout<<Time()<<"\n";
    delete pr;
}
