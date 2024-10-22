#include "solvers.h"

using namespace std;

std::vector<int> SolveNearestNeighbor(const TSP& tsp, int from)
{
    Assert(false, "nearest neighbor not implemented");
    return {};
}

std::vector<int> SolveGreedyEdgePicker(const TSP& tsp)
{
    int n = tsp.n;
    vector<pair<double, pair<int, int> > > edges;
    for (int i=0; i<n; i++)
        for (int j=i+1; j<n; j++)
            edges.pb({tsp[i][j], {i, j}});
    sort(edges.begin(), edges.end());

    DSU dsu;
    vector<vector<int> > g(n);

    for (auto [w, ed] : edges)
    {
        auto [u, v] = ed;
        if (g[u].size() == 2 || g[v].size() == 2 || dsu.Get(u) == dsu.Get(v))
            continue;
        dsu.Unite(u, v);
        g[u].pb(v);
        g[v].pb(u);
    }
    vector<int> ones;
    for (int i=0; i<n; i++)
        if (g[i].size() == 1)
            ones.pb(i);
    Assert(ones.size() == 2, "SolveGreedyEdgePicker: not a path");
    g[ones[0]].pb(ones[1]);
    g[ones[1]].pb(ones[0]);

    vector<int> ans;
    int pr = -1, v = 0;
    while (true)
    {
        ans.pb(v);
        if (g[v][0] != pr) pr = v, v = g[v][0];
        else               pr = v, v = g[v][1];
        if (v == 0)
            break;
    }
    return ans;
}
