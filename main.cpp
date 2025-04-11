#include "common.h"
#include "mst.h"
#include "solvers.h"
#include "smart_set.h"
#include "mst2.h"
#include "visualizer.h"

using namespace std;

int main()
{
    cout<<fixed<<setprecision(10);
//    __experiment();
//    return;

    Visualizer vis(1400);
    TSP tsp = TSP::GenerateRandom(1000);
    vis.SetTSP(tsp);
    tsp = MST_GradientDescentTransform(tsp, 0);
    vis.SetGraph(255, 255, 255, MST(tsp, 0).edges);

    auto m = ComputeMST_Matrix(tsp, 0);
    for (int i=0; i<tsp.n; i++)
        for (int j=i+1; j<tsp.n; j++)
            if (m[i][j] > tsp[i][j] - 1)
                vis.AddGraph(255, 0, 0, { {i, j} });
//    vis.SetCycle(255, 255, 255, SolveGreedyEdgePicker(tsp));

//    tsp = MST_GradientDescentTransform(tsp, 0);
//    vis.SetGraph(255, 255, 255, MST(tsp, 0).edges);

//    {
//        auto m = ComputeMST_Matrix(tsp, 0);
//        auto h = tsp;
//        for (int i=0; i<h.n; i++)
//            for (int j=0; j<h.n; j++)
//                h[i][j] = max(0.0, tsp[i][j] - m[i][j]);
//        vis.SetCycle(0, 255, 0, SolveHamiltonian(h, tsp));
//    }

//    auto path = SolveGreedyAdder(tsp);
//    for (int i=1; ; i++)
//    {
//        cout<<i<<" "<<tsp.Length(path)<<"\n";
//        vis.SetCycle(255, 255, 255, path);
//        path = OptimizeLocalDP(tsp, path, i);
//    }

//    for (int i=59; ; i++)
//    {
//        TSP t = TSP::LoadHCP("tests/hcp_challenge/graph" + ConvertToString(i) + ".hcp");
//        cout<<i<<" "<<t.n<<"\n";
//        auto c = FindHamiltinianCycle(t);
//        cout<<"Length: "<<t.Length(c)<<"\n";
//    }

//    for (int i : c)
//        cout<<i<<"\n";

//    vis.WaitUntilClosed();

    return 0;
}
