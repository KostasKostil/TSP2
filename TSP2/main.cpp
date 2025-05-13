#include "common.h"
#include "mst.h"
#include "solvers.h"
#include "smart_set.h"
#include "mst2.h"
#include "visualizer.h"
#include "tsplibdata.h"

using namespace std;

vector<pair<int, int> > ExtendedMSTEdges(TSP tsp, double limit = 1.0)
{
    auto m = ComputeMST_Matrix(tsp, 0);
    for (int i=0; i<tsp.n; i++)
        for (int j=i+1; j<tsp.n; j++)
            if (i != j)
                tsp[i][j] = tsp[i][j] - m[i][j];
    vector<pair<int, int> > ans;
    for (int i=0; i<tsp.n; i++)
        for (int j=i+1; j<tsp.n; j++)
            if (tsp[i][j] < limit)
                ans.pb({i, j});
    return ans;
}

vector<vector<int> > g;
set<int> alr;
vector<int> order;
void dfs(int v, int p = -1)
{
    if (!alr.count(v))
    {
        alr.insert(v);
        order.pb(v);
    }
    for (int i : g[v])
        if (i != p)
            dfs(i, v);
}

vector<int> EulerOrder(int n, vector<vector<int> > g)
{
    int m = 0;
    map<pair<int, int>, int> mp;
    for (int i=0; i<n; i++)
        for (int j : g[i])
            if (j > i) mp[{i, j}]++, m++;

    vector<int> order;
    set<int> alr;
    int v = rng()%n;
    alr = {v};
    order = {v};

    while (true)
    {
        shuffle(g[v].begin(), g[v].end(), rng);
        bool ok = false;
        for (int u : g[v])
            if (mp[{min(u, v), max(u, v)}] > 0)
            {
                mp[{min(u, v), max(u, v)}]--;
                if (!alr.count(u))
                {
                    alr.insert(u);
                    order.pb(u);
                }
                ok = true;
                v = u;
                break;
            }
        if (!ok)
            break;
        m--;
    }
    if (m == 0)
        return order;
    else
        return EulerOrder(n, g);
}
vector<pair<int, int> > Christofides()
{
    int n = 100;
    TSP tsp = TSP::GenerateRandom(n);
    Visualizer vis(700);
    vis.SetTSP(tsp);

    MST mst = MST(tsp, -1);
    vector<int> odd;
    for (int i=0; i<n; i++)
        if (mst.deg[i] % 2 == 1)
            odd.pb(i);
    cout<<odd.size()<<"\n";
//    odd.resize(48);
    int m = odd.size();
    vector<vector<double> > wodd(m);
    for (int i=0; i<m; i++)
        for (int j=0; j<m; j++)
            wodd[i].pb(tsp[odd[i]][odd[j]]);
    auto e = minWeightPerfectMatching(wodd);
    cout<<e.size()<<"\n";
    for (auto [x, y] : e)
        cout<<x<<" "<<y<<"\n";
//    return 0;

    for (auto& [x, y] : e)
        x = odd[x], y = odd[y];
    set<pair<int, int> > red(e.begin(), e.end());
    vector<pair<int, int> > green, redv, yellow;
    for (auto [x, y] : mst.edges)
        if (!red.count({x, y}) && !red.count({y, x}))
            green.pb({x, y});
        else
            yellow.pb({x, y}), red.erase({x, y}), red.erase({y, x});
    for (auto [x, y] : red)
        redv.pb({x, y});
//    vis.SetGraph(255, 0, 0, redv);
//    vis.SetGraph(64, 64, 255, yellow);
//    vis.SetGraph(0, 255, 0, green);

    vector<vector<int> > g(n);
    for (auto [x, y] : green)
        g[x].pb(y), g[y].pb(x);
    for (auto [x, y] : redv)
        g[x].pb(y), g[y].pb(x);
    for (auto [x, y] : yellow)
        g[x].pb(y), g[y].pb(x);
    for (auto [x, y] : yellow)
        g[x].pb(y), g[y].pb(x);
    auto order = EulerOrder(n, g);

    vis.SetCycle(0, 255, 0, order);
}

int main()
{
    cout<<fixed<<setprecision(10);

//    Christofides();
    CollectStats(1000);

//    for (int i=0; i)

//    __experiment();
//    return 0;

//    string s = "rat195.tsp";
//    cout<<TSPLIB_Data::bounds[s].fi<<" "<<TSPLIB_Data::bounds[s].se<<"\n";
//    TSP tsp = TSP::LoadTSPLIB_GPT("tests/tsplib/"+s);
//
//    SolveLinKernighan_GA_2D(tsp, 1e18, true);

//    Visualizer vis(1400);
//    vis.SetTSP(tsp);
//    double best = 1e18;
//    while (true)
//    {
//        auto p = OptimizeLocalDP(tsp, SolveGreedyAdder(tsp, 1000), 8);
//
//        double cur = tsp.Length(p);
//        if (cur < best)
//        {
//            best = cur;
//            vis.SetCycle(255, 255, 255, p);
//            cout<<best<<"\n";
//        }
//    }
//
//    while (true)
//    {
//        tsp = MST_GradientDescentTransform(tsp, 0);
//        vis.SetGraph(255, 255, 255, ExtendedMSTEdges(tsp));
//    }


    return 0;
}
