#include "solvers.h"

using namespace std;

std::vector<int> SolveGreedyEdgePicker(const TSP& tsp, int final_dp_size)
{
    int n = tsp.n;
    int m = final_dp_size;
    Require(m >= 1 && m <= 30, "SolveGreedyEdgePicker: wrong final_dp_size");

// MST-like initialization
    vector<pair<double, pair<int, int> > > edges;
    for (int i=0; i<n; i++)
        for (int j=i+1; j<n; j++)
            edges.pb({tsp[i][j], {i, j}});
    sort(edges.begin(), edges.end());
    DSU dsu;
    vector<vector<int> > g(n);
    int comps = n;
    for (auto [w, ed] : edges)
    {
        if (comps <= m)
            break;
        auto [u, v] = ed;
        if (g[u].size() == 2 || g[v].size() == 2 || dsu.Get(u) == dsu.Get(v))
            continue;
        dsu.Unite(u, v);
        comps--;
        g[u].pb(v);
        g[v].pb(u);
    }

// generating dp endpoints
    m = comps;
    map<int, vector<int> > mp;
    for (int i=0; i<n; i++)
    {
        if (g[i].size() == 0)
            mp[dsu.Get(i)].pb(i);
        if (g[i].size() <= 1)
            mp[dsu.Get(i)].pb(i);
    }
    vector<array<int, 2> > pos;
    for (auto [x, v] : mp)
    {
        Assert(v.size() == 2, "SolveGreedyEdgePicker: path is not a path");
        pos.pb({v[0], v[1]});
    }

// initializing dp
    double** dp = new double*[2*m];
    int** p = new int*[2*m];
    for (int i=0; i<2*m; i++)
        dp[i] = new double[1<<m],
        p[i] = new int[1<<m];
    for (int i=0; i<2*m; i++)
        for (int j=0; j<(1<<m); j++)
            dp[i][j] = INFDP,
            p[i][j] = -1;

// calculating dp
    for (int i=0; i<2*m; i++)
        dp[i][(1<<m)-1] = tsp[pos[i/2][i%2]][pos[0][0]];
    for (int mask=(1<<m)-2; mask>=0; mask--)
        for (int from=0; from<2*m; from++)
            if (((mask>>(from/2))&1)==1)
                for (int to=0; to<2*m; to++)
                    if (((mask>>(to/2))&1)==0)
                    {
                        double nt = dp[to][mask | (1<<(to/2))] + tsp[pos[from/2][from%2]][pos[to/2][1-to%2]];
                        if (nt < dp[from][mask])
                        {
                            dp[from][mask] = nt;
                            p[from][mask] = to;
                        }
                    }

// restoring dp answer
    int v = 1;
    int mask = 1;
    vector<int> path;
    while (true)
    {
        path.pb(pos[v/2][v%2]);
        v = p[v][mask];
        mask |= (1<<(v/2));
        if (v == -1)
            break;
    }
    reverse(path.begin(), path.end());

// restoring full answer
    vector<int> ans;
    for (int i : path)
    {
        int pr = -1, v = i;
        while (true)
        {
            ans.pb(v);
            if (g[v].size() == 0) break;
            else if (g[v].size() == 1)
            {
                if (pr == -1) pr = v, v = g[v][0];
                else break;
            }
            else if (g[v].size() == 2)
            {
                if (g[v][0] != pr) pr = v, v = g[v][0];
                else               pr = v, v = g[v][1];
            }
            else Assert(false, "SolveGreedyEdgePicker");
        }
    }

    for (int i=0; i<2*m; i++)
    {
        delete[] dp[i];
        delete[] p[i];
    }
    delete[] dp;
    delete[] p;
    return ans;
}
