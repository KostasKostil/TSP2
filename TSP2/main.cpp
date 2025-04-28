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

int main()
{
    cout<<fixed<<setprecision(10);
//    __experiment();
//    return 0;

    string s = "rat195.tsp";
    cout<<TSPLIB_Data::bounds[s].fi<<" "<<TSPLIB_Data::bounds[s].se<<"\n";
    TSP tsp = TSP::LoadTSPLIB_GPT("tests/tsplib/"+s);

    SolveLinKernighan_GA_2D(tsp, 1e18, true);

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
