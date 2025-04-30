#include "common.h"

static int branch_limit = -1;
static int badguys_limit = 3;
static int depth_limit = 1e9;
static int cross_limit = 2;

static TSP tsp;
static vector<vector<int> > g;

typedef PermReverse PERM_REVERSE;

static int n;
static PERM_REVERSE* pr;

static vector<int> badguys;

static SetHash state;
static InsertionSet states;
static PersistentSet changed;

static int num(int u, int v)
{
    return n*min(u, v) + max(u, v);
}
static int dynamic_branch_limit(int depth)
{
    if (depth > 111) return 0;
    return max(1, branch_limit - depth);
//    if (depth <= branch_limit) return 1e9;
//    else return 1;
//    return 3;
}

map<int, int> mp;
static bool cross(int balance)
{
    if (badguys.size() > 2)
        return false;
    int lim = badguys.back();

    int sz = min(lim, n - lim);
    if (sz <= 10)
        return false;
//    if (balance <= 10)

    PERM_REVERSE* oldpr = nullptr;
    bool rev = false;

//    mp[balance]++;
    double LIM = 2e3;
    if (lim < LIM)
    {
        rev = false;
    }
    else if (n - lim < LIM)
    {
        pr->Reverse(0, n);
        lim = n - lim;
        rev = true;
    }
    else
    {
        oldpr = pr;
        pr = new PERM_REVERSE(*pr);
        pr->Apply();
    }

    for (int i=0; i<lim; i++)
    {
        int v = pr->At(i);
        int vx = pr->At((i+1)%lim);
        for (int u : g[v])
        {
            int j = pr->Where(u);
            if (j > lim)
            {
                int jx = j+1;
                if (jx == n) jx = lim;
                int ux = pr->At(jx);
                if (balance + Dist(tsp, v, vx) + Dist(tsp, u, ux) - Dist(tsp, v, u) - Dist(tsp, vx, ux) > 0)
                {
                    vector<int> res;
                    int id = (i+1)%lim;
                    while (true)
                    {
                        res.pb(pr->At(id));
                        id = (id+1)%lim;
                        if (id == (i+1)%lim)
                            break;
                    }
                    id = j;
                    while (true)
                    {
                        res.pb(pr->At(id));
                        id = (id-1);
                        if (id == lim-1) id = n-1;
                        if (id == j)
                            break;
                    }
                    if (oldpr) delete oldpr;
                    delete pr;
                    pr = new PERM_REVERSE(res);
                    return true;
                }
                jx = jx - 1;
                if (jx == lim-1) jx = n-1;
                ux = pr->At(jx);
                if (balance + Dist(tsp, v, vx) + Dist(tsp, u, ux) - Dist(tsp, v, u) - Dist(tsp, vx, ux) > 0)
                {
                    vector<int> res;
                    int id = (i+1)%lim;
                    while (true)
                    {
                        res.pb(pr->At(id));
                        id = (id+1)%lim;
                        if (id == (i+1)%lim)
                            break;
                    }
                    id = j;
                    while (true)
                    {
                        res.pb(pr->At(id));
                        id = (id+1);
                        if (id == n) id = lim;
                        if (id == j)
                            break;
                    }
                    if (oldpr) delete oldpr;
                    delete pr;
                    pr = new PERM_REVERSE(res);
                    return true;
                }
            }
        }
    }

    if (oldpr)
        pr = oldpr;
    else if (rev)
        pr->Undo();

    return false;
}

vector<pair<int, int> > ed0;
int dmax = 0;
static bool go(int balance, int first_v, int last_id, int last_v, int depth = 0)
{
    if (depth > dmax)
    {
        dmax = depth;
//        cout<<dmax<<"\n";
//        for (auto [x, y] : ed0)
//        cout<<"{"<<x<<", "<<y<<"},\n";
    }
//    assert(pr->At(0) == first_v);
//    assert(pr->At(last_id) == last_v);
//    assert(last_id + 1 == badguys.back());

    int current_cycle_balance = balance - Dist(tsp, first_v, last_v);
    if (current_cycle_balance > 0 && badguys.back() == n)
    {
        pr->Apply();
        return true;
    }
    if (current_cycle_balance >= cross_limit)
    {
        if (cross(current_cycle_balance))
            return true;
    }
    if (depth > depth_limit)
        return false;
//    if (balance - Dist(tsp, first_v, last_v)/100 <= 0)
//        return false;
    if (states.Count(state.GetHash()))
        return false;
    states.Insert(state.GetHash());
    if (states.Overflow())
    {
        cout<<"ovf";
        return false;
    }

    int branches = 0;
    for (int i : g[first_v])
        if (i != last_v)
    {
        if (branches >= dynamic_branch_limit(depth)) break;
        int new_balance = balance - Dist(tsp, first_v, i);
        if (new_balance <= 0)
            continue;
        if (changed.Count(num(first_v, i)))
            continue;
        int pos = pr->Where(i);
        if (pos == 1) continue; // can we be one vertex?

        state.Flip(num(first_v, i));
        changed.Insert(num(first_v, i));

        if (pos > last_id) // we entered bad guy
        {
            vector<int> savedbg = badguys;
            // find which badguy is current
            int id = 0;
            while (badguys[id] > pos)
                id++;
            // reverse a segment such that current badguy becomes last one
            int L = badguys.back();
            int R = badguys[id-1];
            pr->Reverse(L, R);
            for (int i=int(badguys.size())-1; i>=id+1; i--)
                badguys[i] = (R-1)-(badguys[i-1]-L)+1;
            reverse(badguys.begin() + id, badguys.end());
            // delete current badguy
            badguys.pop_back();
            pos = pr->Where(i);

            for (int delta : vector<int>{-1, 1})
            {
                if (branches >= dynamic_branch_limit(depth)) break;
                if (pos + delta <= last_id) continue; // too left
                if (pos + delta >= badguys.back()) continue; // too right

                int u = pr->At(pos + delta);
                if (changed.Count(num(i, u)))
                    continue;

                if (delta == -1)
                {
                    pr->Reverse(0, pos);
                    pr->Reverse(pos-1-last_id, badguys.back());
                    changed.Insert(num(i, u));

                    branches++;
                    state.Flip(num(u, i));
                    if (go(new_balance + Dist(tsp, u, i), u, badguys.back() - 1, last_v, depth+1))
                        return true;
                    state.Flip(num(u, i));

                    changed.Undo();
                    pr->Undo();
                    pr->Undo();
                }
                else
                {
                    pr->Reverse(0, pos+1);
                    pr->Reverse(pos-last_id, badguys.back());
                    pr->Reverse(0, badguys.back() - (last_id+1));
                    changed.Insert(num(i, u));

                    branches++;

                    state.Flip(num(u, i));
                    if (go(new_balance + Dist(tsp, u, i), u, badguys.back() - 1, last_v, depth+1))
                        return true;
                    state.Flip(num(u, i));

                    pr->Undo();
                    pr->Undo();
                    pr->Undo();
                    changed.Undo();
                }
            }
            pr->Undo();
            badguys = savedbg;
        }
        else
        {
            { // feasible
                int u = pr->At(pos-1);
                if (!changed.Count(num(i, u)))
                {
                    pr->Reverse(0, pos);
                    changed.Insert(num(i, u));

                    branches++;

                    ed0.pb({u, i});
                    ed0.pb({first_v, i});

                    state.Flip(num(u, i));
                    if (go(new_balance + Dist(tsp, u, i), u, last_id, last_v, depth+1))
                        return true;
                    state.Flip(num(u, i));

                    ed0.pop_back();
                    ed0.pop_back();

                    pr->Undo();
                    changed.Undo();
                }
            }
            if (branches < dynamic_branch_limit(depth))
                if (int(badguys.size())-1 < badguys_limit)
//                    if (pos+1 < last_id) // not sure if this is needed
            { // unfeasible
                int u = pr->At(pos+1);
                if (!changed.Count(num(i, u)))
                {
                    pr->Reverse(pos+1, last_id+1);
                    pr->Reverse(0, last_id+1);
                    changed.Insert(num(i, u));
                    badguys.push_back(badguys.back() - (pos+1));

                    branches++;
                    state.Flip(num(u, i));                    if (go(new_balance + Dist(tsp, u, i), u, badguys.back()-1, last_v, depth+1))
                        return true;
                    state.Flip(num(u, i));

                    badguys.pop_back();
                    pr->Undo();
                    pr->Undo();
                    changed.Undo();
                }
            }
        }

        state.Flip(num(first_v, i));
        changed.Undo();
    }
    return false;
}

void SequentialLKH(const TSP& _tsp, const vector<vector<int> >& _g, const vector<vector<AlphaEdge> >& G, vector<int>& tour)
{
    tsp = _tsp;
    g = _g;
    n = tsp.size();

    unordered_map<int, double> alpha;
    for (int i=0; i<n; i++)
        for (AlphaEdge e : G[i])
            alpha[num(i, e.v)] = e.alpha;

    pr = new PERM_REVERSE(tour);

    int upd = 0;
    int total = 0;
    int id = 0;
    vector<pair<int, int> > candidate_list;

    long long totalstates = -1;
    map<int, int> dstats;

    auto reset_candidates = [&]()->int
    {
        cout<<totalstates<<"\n";
//        PrintMapStats(dstats);
        totalstates = 0;
        dstats.clear();

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

        shuffle(candidate_list.begin(), candidate_list.end(), rng);
        return ans;
    };

    double lastOutputTime = -1e18;
//    Visualizer vis;
//    vis.SetTSP(tsp);
//    vis.SetCycle(255, 255, 255, tour);

    reset_candidates();

    while (true)
    {
        if (upd > 0)
        if (id == n)
        {
            reset_candidates();
        }
        if (id == n)
        {
            tour.clear();
            for (int i=0; i<n; i++)
                tour.pb(pr->At(i));
            branch_limit++;
            cout<<"BRANCH_LIMIT = "<<branch_limit<<"\n";
            PrintMapStats(mp);
            id = 0;
            reset_candidates();
//            exit(0);
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
        int first_v = pr->At(0);
        int last_id = n-1;
        int last_v = pr->At(last_id);
        changed.Clear();
        changed.Insert(num(first_v, last_v));
        badguys = {n};

        total++;
        ed0.clear();
        ed0.pb({first_v, last_v});

        state = SetHash();
        totalstates += states.Size();
        states.Clear();

        dmax = 0;
        if (go(Dist(tsp, first_v, last_v), first_v, last_id, last_v, 0))
        {
            upd++;

            double currentTime = Time();
            if (currentTime - lastOutputTime > 2.0 || Length(tsp, tour) < 5760000)
            {
                tour.clear();
                for (int i=0; i<n; i++)
                tour.pb(pr->At(i));
                cout<<"length="<<Length(tsp, tour)<<" id="<<id<<" upd="<<upd<<" total="<<total<<" branches="<<branch_limit<<"\n";
//                vis.SetCycle(255, 255, 255, tour);
                lastOutputTime = currentTime;
                ofstream fout("data/sequential_lkh.txt");
                for (int i : tour)
                    fout<<i<<"\n";
            }
        }
        dstats[dmax]++;

        if (states.Overflow())
            cout<<"\n";
    }

    cout<<Time()<<"\n";
    delete pr;
}
