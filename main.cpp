#include "common.h"
#include "mst.h"
#include "basic_solvers.h"

using namespace std;

int main()
{
    cout<<fixed<<setprecision(10);
    for (int i=0; i<60; i++)
    {
//        TSP tsp("tests/random/"+to_string(i)+".tsp2");
        TSP tsp("tests/eolymp/"+to_string(i)+".tsp2");
        cout<<"ID "<<i<<"\n";
        cout<<MST(tsp, 0).weight<<"\n";
        cout<<tsp.Length(SolveGreedyEdgePicker(tsp))<<"\n";
        tsp = MST_GradientDescentTransform(tsp);
        cout<<MST(tsp, 0).weight<<"\n";
        cout<<tsp.Length(SolveGreedyEdgePicker(tsp))<<"\n";
        cout<<"\n";
    }
    return 0;
}
