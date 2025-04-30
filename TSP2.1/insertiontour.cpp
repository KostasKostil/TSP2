#include "common.h"

vector<int> InsertionTour(const TSP& tsp, vector<vector<int> > g)
{
    int n = tsp.size();
    vector<int> perm;
    for (int i=0; i<n; i++)
        perm.pb(i);
    shuffle(perm.begin(), perm.end(), rng);

    vector<int> pr(n, -1);
    vector<int> nt(n, -1);
    auto link = [&](int u, int v)
    {
        if (nt[u] != -1)
            pr[nt[u]] = v;
        if (pr[v] != -1)
            nt[pr[v]] = u;

        nt[u] = v;
        pr[v] = u;
    };

    link(perm[0], perm[1]);
    link(perm[1], perm[2]);
    link(perm[2], perm[0]);
    int root = perm[0];
    int L = Dist(tsp, perm[0], perm[1]) + Dist(tsp, perm[1], perm[2]) + Dist(tsp, perm[2], perm[0]);

    for (int _=3; _<n; _++)
    {
        int v = perm[_];

        int cx = 0;
        pair<int, int> best = {-1e9, -1};
        for (int i : g[v])
            if (nt[i] != -1)
            {
                int before = Dist(tsp, i, nt[i]);
                int after = Dist(tsp, i, v) + Dist(tsp, v, nt[i]);
                best = max(best, {before - after, i});
                cx++;
            }

        if (cx < 3)
        {
            int i = root;
            while (true)
            {
                int before = Dist(tsp, i, nt[i]);
                int after = Dist(tsp, i, v) + Dist(tsp, v, nt[i]);
                best = max(best, {before - after, i});

                i = nt[i];
                if (i == root)
                    break;
            }
        }

        int u = best.se;
        int w = nt[u];
        link(u, v);
        link(v, w);
        L -= best.fi;
    }

    cout<<L<<"\n";

    vector<int> tour;
    int i = root;
    while (true)
    {
        tour.pb(i);
        i = nt[i];
        if (i == root)
            break;
    }
    return tour;
}
