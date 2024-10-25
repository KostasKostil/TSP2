#include "common.h"
#include "mst.h"
#include "solvers.h"

using namespace std;

int main()
{
    cout<<fixed<<setprecision(10);

    for (int i=0; i<60; i++)
    {
//        TSP tsp("tests/random/"+to_string(i)+".tsp2");
        TSP tsp("tests/eolymp/"+to_string(i)+".tsp2");
        TSP tsp2 = MST_GradientDescentTransform(tsp);
        cout<<"ID "<<i<<"\n";
        cout<<"MST "<<MST(tsp, 0).weight<<" "<<MST(tsp2, 0).weight<<"\n";
        cout<<"NN "<<tsp.Length(SolveNearestNeighbor(tsp))<<" "<<tsp2.Length(SolveNearestNeighbor(tsp2))<<"\n";
        for (int m = 1; m <= 15; m++)
        cout<<"SGEP "<<m<<" "<<tsp.Length(SolveGreedyEdgePicker(tsp, m))<<" "<<tsp2.Length(SolveGreedyEdgePicker(tsp2, m))<<"\n";
        cout<<"\n";
    }
    return 0;
}
