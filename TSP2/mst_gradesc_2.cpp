#include "mst.h"
#include "visualizer.h"
#include "solvers.h"

using namespace std;

vector<pair<double, double> > MST_GradientDescentSmartTransform_weights(TSP tsp)
{
    int n = tsp.n;
    vector<pair<double, double> > ans(n, pair<double, double>(-INFDP, -INFDP));

    MST mst = MST(tsp);
    vector<pair<double, pair<int, int> > > e;
//    for (auto [i, j] : mst.edges)
//        e.pb({tsp[i][j], {i, j} });
    for (int i=0; i<n; i++)
        for (int j=i+1; j<n; j++)
            e.pb({tsp[i][j], {i, j} });
    sort(e.begin(), e.end());

    for (int v=0; v<n; v++)
    {
        pair<double, int> mn = {INFDP, -1};
        for (int i=0; i<n; i++)
            if (i != v)
                mn = min(mn, {tsp[v][i], i});
        vector<int> best(n);
        for (int i=0; i<n; i++)
            best[i] = i;
        vector<bool> hasv(n, false);
        hasv[v] = true;

        DSU d;
        d.Get(n-1);
        int comp = n;
        {
            int i = v;
            int j = mn.se;
            if (rng()%2 == 0)
                swap(i, j);
            hasv[i] = hasv[i] | hasv[j];
            if (tsp[v][best[j]] < tsp[v][best[i]])
                best[i] = best[j];
            d.p[j] = i;
            comp--;
        }

        for (auto [w, edge] : e)
        {
            if (comp == 1)
                break;

            auto [i, j] = edge;
            if (i == v || j == v)
                continue;

            i = d.Get(i);
            j = d.Get(j);
            if (i == j)
                continue;

            if (hasv[j] && !hasv[i]) swap(i, j);

            if (hasv[i] && !hasv[j])
            {
                double d1 = w - tsp[v][best[j]];
//                cout<<v<<" "<<w<<" "<<tsp[v][best[j]]<<" "<<d1<<" "<<ans[v].fi<<" "<<ans[v].se<<"\n";
                ans[v].se = max(ans[v].se, min(ans[v].fi, d1));
                ans[v].fi = max(ans[v].fi, d1);
            }

            if (!hasv[i] && !hasv[j])
            {
                double d1 = w - tsp[v][best[i]];
                double d2 = w - tsp[v][best[j]];
                ans[v].se = max(ans[v].se, min(d1, d2));
            }

            if (rng()%2 == 0)
                swap(i, j);
            hasv[i] = hasv[i] | hasv[j];
            if (tsp[v][best[j]] < tsp[v][best[i]])
                best[i] = best[j];
            d.p[j] = i;
            comp--;
        }
    }
    return ans;
}
vector<pair<double, double> > MST_GradientDescentSmartTransform_weights_rooted(TSP tsp, int root)
{
    int n = tsp.n;
    vector<pair<double, double> > ans(n, pair<double, double>(-INFDP, -INFDP));

    MST mst = MST(tsp, root);
    vector<pair<double, pair<int, int> > > e;
//    for (auto [i, j] : mst.edges)
//        e.pb({tsp[i][j], {i, j} });
    for (int i=0; i<n; i++)
        for (int j=i+1; j<n; j++)
            if (i != root && j != root)
                e.pb({tsp[i][j], {i, j} });
    sort(e.begin(), e.end());
    for (int v=0; v<n; v++)
        if (v != root)
    {
        pair<double, int> mn = {INFDP, -1};
        for (int i=0; i<n; i++)
            if (i != v)
                mn = min(mn, {tsp[v][i], i});
        vector<int> best(n);
        for (int i=0; i<n; i++)
            best[i] = i;
        vector<bool> hasv(n, false);
        hasv[v] = true;

        DSU d;
        d.Get(n-1);
        int comp = n - 1;
        {
            int i = v;
            int j = mn.se;
            if (rng()%2 == 0)
                swap(i, j);
            hasv[i] = hasv[i] | hasv[j];
            if (tsp[v][best[j]] < tsp[v][best[i]])
                best[i] = best[j];
            d.p[j] = i;
            comp--;
        }

        for (auto [w, edge] : e)
        {
            if (comp == 1)
                break;

            auto [i, j] = edge;
            if (i == v || j == v)
                continue;

            i = d.Get(i);
            j = d.Get(j);
            if (i == j)
                continue;

            if (hasv[j] && !hasv[i]) swap(i, j);

            if (hasv[i] && !hasv[j])
            {
                double d1 = w - tsp[v][best[j]];

                ans[v].se = max(ans[v].se, min(ans[v].fi, d1));
                ans[v].fi = max(ans[v].fi, d1);
            }

            if (!hasv[i] && !hasv[j])
            {
                double d1 = w - tsp[v][best[i]];
                double d2 = w - tsp[v][best[j]];
                ans[v].se = max(ans[v].se, min(d1, d2));
            }

            if (rng()%2 == 0)
                swap(i, j);
            hasv[i] = hasv[i] | hasv[j];
            if (tsp[v][best[j]] < tsp[v][best[i]])
                best[i] = best[j];
            d.p[j] = i;
            comp--;
        }

    }

    //root processing
    ans[root] = {0, 0};
    vector<double> wroot(n);
    vector<pair<double, int> > vroot;
    for (int i=0; i<n; i++)
        if (i != root)
            vroot.pb({tsp[i][root], i});
    sort(vroot.begin(), vroot.end());
    for (int i=0; i<int(vroot.size()); i++)
    {
        int v = vroot[i].se;
        if (i <= 1)
            wroot[v] = tsp[root][vroot[2].se] - tsp[root][v];
        else
            wroot[v] = tsp[root][vroot[1].se] - tsp[root][v];
    }
    for (int i=0; i<n; i++)
        if (i != root)
        {
            if (wroot[i] >= ans[i].fi)
            {
                ans[i] = {wroot[i], ans[i].fi};
            }
            else if (wroot[i] >= ans[i].se)
            {
                ans[i] = {ans[i].fi, wroot[i]};
            }
        }
    return ans;
}
TSP AddWeights(TSP tsp, std::vector<double> w)
{
    int n = tsp.n;
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
            if (i != j)
                tsp[i][j] += w[i] + w[j];
    return tsp;
}
double sum(vector<double> w)
{
    double x = 0;
    for (double y : w) x += y;
    return x;
}

TSP MST_GradientDescentSmartTransform(TSP tsp)
{
    Visualizer vis(1400);
    vis.SetTSP(tsp);
    int n = tsp.n;
    double sumw = 0;
    double path = tsp.Length(SolveGreedyAdder(tsp, 1000));
    double e0 = 0;
    {
        auto q = MST_GradientDescentTransform(tsp, 0);
        e0 = MST(q, 0).weight;
    }
    while (true)
    {
        auto W = MST_GradientDescentSmartTransform_weights(tsp);
        MST mst = MST(tsp, 0);

        vector<double> w(n);
        for (int v=0; v<n; v++)
        {
            double dw = 0;
            double mn = min(W[v].fi, W[v].se);
            double mx = max(W[v].fi, W[v].se);
            if (mn >= 0) dw = mn;
            if (mx <= 0) dw = mx;

            double f = rng()*1.0/UINT_MAX;
            w[v] = (mn*f + mx*(1-f)) / 2;
            cout<<dw;
        }


        tsp = AddWeights(tsp, w);
        sumw += sum(w);
        cout<<e0<<" "<<MST(tsp, 0).weight - 2*sumw<<" "<<path<<"\n";
        vis.SetGraph(255, 255, 255, MST(tsp, 0).edges);
    }
}

TSP MST_GradientDescentMixedTransform(TSP tsp, int root)
{
    double alpha = 0.97;
    int limit = 1000;

    int n = tsp.n;
    double step = INF;
    MST mst = MST(tsp, root);
    auto iterations = 0;

    while (step > EPS && iterations < limit)
    {

        cout<<iterations<<"\n";
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

        {
            auto W = MST_GradientDescentSmartTransform_weights_rooted(tsp, root);
            vector<double> w(n);
            for (int v=0; v<n; v++)
            {
//                double dw = 0;
//                double mn = min(W[v].fi, W[v].se);
//                double mx = max(W[v].fi, W[v].se);
//                if (mn >= 0) dw = mn;
//                if (mx <= 0) dw = mx;
                w[v] = 0.5;
            }
            double sumw = 0;
            for (double x : w)
                sumw += x;
            for (double& x : w)
                x -= sumw / n;
            tsp = AddWeights(tsp, w);
            mst = MST(tsp, root);
        }
        iterations++;
    }
    return tsp;
}
