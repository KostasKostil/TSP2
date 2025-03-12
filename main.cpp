#include "common.h"
#include "mst.h"
#include "solvers.h"
#include "smart_set.h"
#include "mst2.h"

using namespace std;

void collect_stats()
{
    map<int, pair<double, double> > stats;
    for (int i=0; i<20; i++)
    {
        TSP tsp("tests/random/"+to_string(i)+".tsp2");
//        TSP tsp("tests/eolymp/"+to_string(i)+".tsp2");
        TSP tsp2 = MST_GradientDescentTransform(tsp);
        cout<<"ID "<<i<<"\n";
        cout<<tsp.n<<"\n";
        cout<<"MST "<<MST(tsp, 0).weight<<" "<<MST(tsp2, 0).weight<<"\n";
        stats[-1].fi += MST(tsp, 0).weight;
        stats[-1].se += MST(tsp2, 0).weight;
        cout<<"NN "<<tsp.Length(SolveNearestNeighbor(tsp))<<" "<<tsp2.Length(SolveNearestNeighbor(tsp2))<<"\n";
        stats[0].fi += tsp.Length(SolveNearestNeighbor(tsp));
        stats[0].se += tsp2.Length(SolveNearestNeighbor(tsp2));
        for (int m = 1; m <= 20; m++)
        {
        cout<<"SGEP "<<m<<" "<<tsp.Length(SolveGreedyEdgePicker(tsp, m))<<" "<<tsp2.Length(SolveGreedyEdgePicker(tsp2, m))<<"\n";
        stats[m].fi += tsp.Length(SolveGreedyEdgePicker(tsp, m));
        stats[m].se += tsp2.Length(SolveGreedyEdgePicker(tsp2, m));
        }
    }
    for (auto pa : stats)
        cout<<pa.fi<<" "<<pa.se.fi<<" "<<pa.se.se<<"\n";
}

int main()
{
    cout<<fixed<<setprecision(10);

    MST2_Test();

//    vector<double> stats(3);
//    int k = 0;
//    while (true)
//    {
//        k++;
//        TSP tsp = TSP::GenerateRandom(500);
//        tsp = MST_GradientDescentTransform(tsp);
//
//        stats[0] += MST(tsp, 0).weight;
//        cout<<stats[0]/k<<" ";
//        auto tour = SolveGreedyEdgePicker(tsp, 16);
//        stats[1] += tsp.Length(tour);
//        cout<<stats[1]/k<<" ";
//        tour = OptimizeLocalDP(tsp, tour, 12);
//        stats[2] += tsp.Length(tour);
//        cout<<stats[2]/k<<"\n";
//    }

//    map<int, pair<double, double> > stats;
//    for (int i=0; i<60; i++)
//    {
////        TSP tsp("tests/random/"+to_string(i)+".tsp2");
//        TSP tsp("tests/eolymp/"+to_string(i)+".tsp2");
//        cout<<"ID "<<i<<"\n";
//        auto tour = SolveGreedyEdgePicker(tsp, 10);
//        cout<<tsp.Length(tour)<<" ";
//        tour = OptimizeLocalDP(tsp, tour, 15);
//        cout<<tsp.Length(tour)<<"\n";
//    }
    return 0;
}
