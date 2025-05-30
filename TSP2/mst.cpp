#include "mst.h"

using namespace std;

MST::MST(){}
MST::MST(const TSP& tsp, int root)
{
    int n = tsp.n;
    Require(root >= -1 && root < n, "MST constructor: incorrect root value " + ConvertToString(root) + "/" + ConvertToString(n));
    weight = 0;
    deg.assign(n, 0);
    g.resize(n);

    vector<int> best(n, -1);
    vector<bool> used(n, false);

    auto add_edge = [&](int u, int v)
    {
        weight += tsp[u][v];
        deg[u]++;
        deg[v]++;
        edges.pb({u, v});
        g[u].pb(v);
        g[v].pb(u);
    };
    auto add = [&](int v)
    {
        used[v] = 1;
        for (int i=0; i<n; i++)
            if (!used[i])
                if (best[i] == -1 || tsp[i][v] < tsp[i][best[i]])
                    best[i] = v;
    };
    if (root != -1)
        used[root] = 1;
    int initial_vertex = 0;
    if (initial_vertex == root)
        initial_vertex++;
    add(initial_vertex);
    for (int _=0; _<n-1-(root!=-1); _++)
    {
        int v = -1;
        for (int i=0; i<n; i++)
            if (!used[i])
                if (v == -1 || tsp[i][best[i]] < tsp[v][best[v]])
                    v = i;
        add_edge(v, best[v]);
        add(v);
    }
    if (root != -1)
    {
        Require(n>=3, "MST constructor: n<=2 and root != -1");
        int min1 = -1;
        int min2 = -1;
        for (int i=0; i<n; i++)
            if (i != root)
            {
                if (min2 == -1 || tsp[root][i] < tsp[root][min2])
                {
                    min2 = i;
                    if (min1 == -1 || tsp[root][min2] < tsp[root][min1])
                        swap(min1, min2);
                }
            }
        add_edge(root, min1);
        add_edge(root, min2);
    }
}

TSP MST_GradientDescentTransform(TSP tsp, int root)
{
    double alpha = 0.97;
    int limit = 1000;

    int n = tsp.n;
    double step = INF;
    MST mst = MST(tsp, root);
    auto iterations = 0;

    while (step > EPS && iterations < limit)
    {
        TSP next_tsp = tsp;
        for (int i=0; i<n; i++)
            if (mst.deg[i] != 2)
                for (int j=0; j<n; j++)
                    if (j != i)
                    {
                        next_tsp[i][j] += (mst.deg[i]-2)*step;
                        next_tsp[j][i] += (mst.deg[i]-2)*step;
                    }
        MST next_mst(next_tsp, root);
        if (next_mst.weight > mst.weight)
        {
            tsp = next_tsp;
            mst = next_mst;
        }
        else
        {
            step *= alpha;
        }

        iterations++;
    }
    return tsp;
}
