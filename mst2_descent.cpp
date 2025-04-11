#include "mst2.h"

using namespace std;

MST2_GradientDescentResult MST2_GradientDescentEstimate(TSP tsp, int k, int root)
{
    int n = tsp.n;
    double step = 1e3;
    auto iterations = 0;
    double estimate = 0;

    MST2_GradientDescentResult res;
    res.found = false;
    while (step > EPS && iterations < 1)
    {
        vector<Edge> e;
        for (int i=0; i<n; i++)
            for (int j=i+1; j<n; j++)
                if (i != root && j != root)
                    e.pb(Edge{i - (i>root), j - (j>root), tsp[i][j]});
        MST2 mst(n-1, e, k);
        vector<int> deg(n, 0);
        for (int i=0; i<int(e.size()); i++)
            if (mst.T.back()[i])
                deg[e[i].u +(e[i].u >= root)]++,
                deg[e[i].v +(e[i].v >= root)]++;

        vector<pair<double, int> > roote;
        for (int i=0; i<n; i++)
            if (i != root)
                roote.pb({tsp[root][i], i});
        nth_element(roote.begin(), roote.begin() + 1, roote.end());
        deg[roote[0].se]++;
        deg[roote[1].se]++;
        deg[root] = 2;
        estimate = max(estimate, mst.W.back() + roote[0].fi + roote[1].fi);

        cout<<mst.W[0]<<" "<<mst.W.back()<<" "<<roote[0].fi<<" "<<roote[1].fi<<" "<<mst.W.back() + roote[0].fi + roote[1].fi<<"\n";

        pair<double, vector<int> > bestpath;
        bestpath.fi = INFDP;

        for (int t=0; t<int(mst.T.size()); t++)
        {
            vector<int> leaf;
            vector<int> cur(n-1, 0);
            for (int i=0; i<int(e.size()); i++)
                if (mst.T[t][i])
                    cur[e[i].u]++, cur[e[i].v]++;
            for (int i=0; i<n-1; i++)
                if (cur[i] == 1)
                    leaf.pb(i + (i>=root));
            if (int(leaf.size()) == 2)
            {
                double W = mst.W[t] + tsp[root][leaf[0]] + tsp[root][leaf[1]];
                if (W < bestpath.fi)
                {
                    int u = root;
                    int v = leaf[0];
                    vector<int> path = {u, v};
                    while (v != leaf[1])
                    {
                        for (auto [q, id] : mst.g[v - (v > root)])
                            if (mst.T[t][id] && q != (u - (u > root)))
                            {
                                path.pb(q + (q >= root));
                                u = v;
                                v = path.back();
                                break;
                            }
                    }
                    Assert( abs(tsp.Length(path) - W) / max<double>(tsp.Length(path), W) < EPS, [&](ostream& out){ out<<"MST2GD Incorrect invariant "<<tsp.Length(path)<<" "<<W<<"\n";});
                    bestpath = min(bestpath, {tsp.Length(path), path});
                }
            }
        }
        if (!bestpath.se.empty())
            if (res.path.empty() || bestpath.fi < tsp.Length(res.path))
                res.path = bestpath.se;
        if (!res.path.empty() && tsp.Length(res.path) < estimate + EPS)
        {
            res.found = true;
            res.estimate = tsp.Length(res.path);
            return res;
        }

        TSP next_tsp = tsp;
        for (int i=0; i<n; i++)
            if (deg[i] != 2)
                for (int j=0; j<n; j++)
                    if (j != i)
                    {
                        next_tsp[i][j] += (deg[i]-2)*step;
                        next_tsp[j][i] += (deg[i]-2)*step;
                    }
        tsp = next_tsp;

        step *= 0.99;
        iterations++;
    }
    res.estimate = estimate;
    return res;
}

double Estimate_MST2(const TSP& tsp, int k, int root)
{
    int n = tsp.n;
    double estimate = 0;
    double example = INFDP;

    vector<Edge> e;
    for (int i=0; i<n; i++)
        for (int j=i+1; j<n; j++)
            if (i != root && j != root)
                e.pb(Edge{i - (i>root), j - (j>root), tsp[i][j]});
    MST2 mst(n-1, e, k);
    vector<int> deg(n, 0);
    for (int i=0; i<int(e.size()); i++)
        if (mst.T.back()[i])
            deg[e[i].u +(e[i].u >= root)]++,
            deg[e[i].v +(e[i].v >= root)]++;

    vector<pair<double, int> > roote;
    for (int i=0; i<n; i++)
        if (i != root)
            roote.pb({tsp[root][i], i});
    nth_element(roote.begin(), roote.begin() + 1, roote.end());
    deg[roote[0].se]++;
    deg[roote[1].se]++;
    deg[root] = 2;
    estimate = max(estimate, mst.W.back() + roote[0].fi + roote[1].fi);

    pair<double, vector<int> > bestpath;
    bestpath.fi = INFDP;

    for (int t=0; t<int(mst.T.size()); t++)
    {
        vector<int> leaf;
        vector<int> cur(n-1, 0);
        for (int i=0; i<int(e.size()); i++)
            if (mst.T[t][i])
                cur[e[i].u]++, cur[e[i].v]++;
        for (int i=0; i<n-1; i++)
            if (cur[i] == 1)
                leaf.pb(i + (i>=root));
        if (int(leaf.size()) == 2)
        {
            double W = mst.W[t] + tsp[root][leaf[0]] + tsp[root][leaf[1]];
            if (W < bestpath.fi)
            {
                int u = root;
                int v = leaf[0];
                vector<int> path = {u, v};
                while (v != leaf[1])
                {
                    for (auto [q, id] : mst.g[v - (v > root)])
                        if (mst.T[t][id] && q != (u - (u > root)))
                        {
                            path.pb(q + (q >= root));
                            u = v;
                            v = path.back();
                            break;
                        }
                }
                Assert( abs(tsp.Length(path) - W) / max<double>(tsp.Length(path), W) < EPS, [&](ostream& out){ out<<"MST2GD Incorrect invariant "<<tsp.Length(path)<<" "<<W<<"\n";});
                bestpath = min(bestpath, {tsp.Length(path), path});
            }
        }
    }
    if (!bestpath.se.empty())
    {
        example = min(example, bestpath.fi);
    }

    return min(example, estimate);
}
