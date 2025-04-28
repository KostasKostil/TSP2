#include "solvers.h"
#include "visualizer.h"

using namespace std;

vector<int> inverse(vector<int> tour)
{
    int n = tour.size();
    vector<int> ans(n);
    for (int i=0; i<n; i++)
        ans[tour[i]] = i;
    return ans;
}

void SolveLinKernighan_GA_2D(const TSP& tsp, double TL, bool visualize)
{
    vector<int> tour = SolveGreedyAdder(tsp, 1);
    int n = tsp.n;
    Visualizer* vis;
    if (visualize)
    {
        cout<<tsp.Length(tour)<<"\n";
        vis = new Visualizer(1400);
        vis->SetTSP(tsp);
        vis->SetCycle(255, 255, 255, tour);
    }

    vector<vector<int> > order(n);
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
            if (i != j)
                order[i].pb(j);
    vector<int> inv;

    auto nt = [&](int x){ return tour[(inv[x]+1)%n]; };
    auto edge_dist = [&](int x, int y)
    {
        double x1 = (tsp.coords[x].x + tsp.coords[nt(x)].x)/2;
        double y1 = (tsp.coords[x].y + tsp.coords[nt(x)].y)/2;
        double x2 = (tsp.coords[y].x + tsp.coords[nt(y)].x)/2;
        double y2 = (tsp.coords[y].y + tsp.coords[nt(y)].y)/2;
        return hypot(x1-x2, y1-y2);
    };
    auto fill_upd = [&]()
    {
        inv = inverse(tour);
        for (int i=0; i<n; i++)
            sort(order[i].begin(), order[i].end(), [&](int x, int y){ return edge_dist(i, x) < edge_dist(i, y); });
    };

    auto t0 = clock();

    cout<<"stage one\n";
    while ((clock() - t0)*1.0/CLOCKS_PER_SEC < 1.0)
    {
        auto t2 = SolveGreedyAdder(tsp, 1);
        if (tsp.Length(t2) < tsp.Length(tour))
        {
            tour = t2;
            cout<<tsp.Length(tour)<<"\n";
        }
    }
    tour = OptimizeLocalDP(tsp, tour, 6);
    cout<<"stage two\n";
    fill_upd();
    int LM = 10;
    int RM = n;
    while ((clock() - t0)*1.0/CLOCKS_PER_SEC < TL)
    {
        // choosing sz
        int m1 = LM + rng()%(RM-LM);
        int m2 = LM + rng()%(RM-LM);
        int m = min(m1, m2);

        // choosing vertex set
        int v0 = rng()%n;
        vector<int> crit = {v0};
        for (int i=0; i<m-1; i++)
            crit.push_back(order[v0][i]);

        // visualizing
//        if (visualize)
//        {
//            vector<pair<int, int> > ed;
//            for (int i=0; i<m; i++)
//                ed.pb({crit[i], nt(crit[i])});
//            vis->SetGraph(255, 0, 0, ed);
//        }

        // calculating current segment
        sort(crit.begin(), crit.end(), [&](int x, int y){ return inv[x] < inv[y]; });
        vector<pair<int, int> > lr;
        for (int i=0; i<m; i++)
            lr.pb({nt(crit[i]), crit[(i+1)%m]});

        // calculating before value
        double before = 0;
        for (int i=0; i<m; i++)
            before += tsp[lr[i].se][lr[(i+1)%m].fi];

        // insertion algo
        shuffle(lr.begin(), lr.end(), rng);
        vector<pair<int, int> > best = {lr[0]};
        double after = tsp[lr[0].fi][lr[0].se];
        for (int id=1; id<m; id++)
        {
            #define bsize id
            pair<int, int> cur = lr[id];
            double best_delta = 1e18;
            int best_position = 0;
            bool best_reverse = false;

            for (int pos=0; pos<bsize; pos++)
                for (int rev_flag=0; rev_flag<=1; rev_flag++)
                {
                    // trying to insert after pos
                    double before = tsp[best[pos].se][best[(pos+1)%bsize].fi];
                    double after = tsp[best[pos].se][cur.fi] + tsp[cur.se][best[(pos+1)%bsize].fi];
                    double delta = after - before;
                    if (delta < best_delta)
                    {
                        best_delta = delta;
                        best_position = (pos+1)%bsize;
                        best_reverse = rev_flag;
                    }
                    swap(cur.fi, cur.se);
                }
            if (best_reverse)
                swap(cur.fi, cur.se);
            best.insert(best.begin() + best_position, cur);
            after += best_delta;
        }
        if (after < before)
        {
            map<pair<int, int>, vector<int> > mp;
            for (int i=0; i<m; i++)
            {
                int u = nt(crit[i]);
                int v = crit[(i+1)%m];
                int id = inv[u];
                vector<int> cur = {u};
                while (cur.back() != v)
                {
                    id = (id+1)%n;
                    cur.pb(tour[id]);
                }
                mp[{u, v}] = cur;
                reverse(cur.begin(), cur.end());
                mp[{v, u}] = cur;
            }
            tour.clear();
            for (int i=0; i<m; i++)
            {
                int u = best[i].fi;
                int v = best[i].se;
                for (int j : mp[{u, v}])
                    tour.pb(j);
            }
            tour = OptimizeLocalDP(tsp, tour, 6);
            cout<<tsp.Length(tour)<<"\n";
            if (visualize)
                vis->SetCycle(255, 255, 255, tour);
            fill_upd();
        }
    }
}
