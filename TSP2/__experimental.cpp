#include "common.h"
#include "solvers.h"
#include "visualizer.h"
#include "mst.h"

using namespace std;

void __experiment1()
{
    int n = 1000;
    TSP tsp = TSP::GenerateRandom(n);

    Visualizer vis(1400);
    vis.SetTSP(tsp);

    while (true)
    {
//        tsp = MST_GradientDescentTransform(tsp, -1);

//        {
//            MST mst(tsp);
//            vis.SetGraph(255, 255, 255, mst.edges);
////            auto c = clock();
////            while ((clock()-c)*1.0/CLOCKS_PER_SEC < 0.5)
////                continue;
//        }


        {
            MST mst(tsp);
            set<int> d1;
            for (int i=0; i<n; i++)
                if (mst.g[i].size() == 1)
                    d1.insert(i);

            {//log
                map<int, int> degs;
                for (int i=0; i<n; i++)
                    degs[mst.g[i].size()]++;
                double mn = tsp[0][1], mx = tsp[0][1];
                for (int i=0; i<n; i++)
                    for (int j=0; j<n; j++)
                        if (i != j)
                            mn = min(mn, tsp[i][j]), mx = max(mx, tsp[i][j]);
                for (auto [x, y] : degs) cout<<"["<<x<<","<<y<<"],";
                cout<<"\n";
                cout<<mn<<" "<<mx<<"\n";
            }

            vector<int> order;
            while (d1.size() > 1)
            {
                int v = *d1.begin();
                order.pb(v);
                d1.erase(v);
                mst.deg[v] = 0;
                int u = -1;
                for (int i : mst.g[v])
                    if (mst.deg[i] > 0)
                    {
                        Assert(u == -1, "__exp1 u != -1");
                        u = i;
                    }
                mst.deg[u]--;
                if (mst.deg[u] == 1)
                    d1.insert(u);
            }
            reverse(order.begin(), order.end());
            for (int v : order)
            {
                for (int u : mst.g[v])
                    if (d1.count(u))
                    {
                        double add = -tsp[u][v];
                        for (int i=0; i<n; i++)
                            if (i != v)
                                tsp[i][v] += add, tsp[v][i] += add;
                    }
                d1.insert(v);
            }
        }
        {
            MST mst(tsp);
            vis.SetGraph(255, 255, 255, mst.edges);
//            auto c = clock();
//            while ((clock()-c)*1.0/CLOCKS_PER_SEC < 0.1)
//                continue;
        }
    }
}

void __experiment2() // greedy adder best permutation analyzer
{
    TSP tsp = TSP::GenerateRandom(200);
    double best = 1e18;
    Visualizer vis(1400);
    vis.SetTSP(tsp);
    vector<int> perm;
    for (int i=0; i<tsp.n; i++)
        perm.pb(i);
    int Q = 10;

    while (true)
    {
        shuffle(perm.begin(), perm.end(), rng);
        double x = tsp.Length(SolveGreedyAdderSinglePermutation(tsp, perm));
        if (x < best + 1)
        {
            best = x;
            cout<<best<<"\n";
            for (int i=0; i+1<min(Q, tsp.n); i++)
                vis.SetGraph(255, 255, 255*i/min(Q, tsp.n), {});
            for (int i=0; i+1<min(Q, tsp.n); i++)
                vis.AddGraph(255, 255, 255*i/min(Q, tsp.n), { {perm[i], perm[i+1]} });
        }
    }
}

void __experiment()
{
    __experiment2();
}
