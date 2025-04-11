#include "solvers.h"
#include "mst.h"

using namespace std;

double Estimate_MST_GraDesc(TSP tsp, int num_roots)
{
    if (num_roots < 0 || num_roots > tsp.n)
        num_roots = tsp.n;

    vector<int> perm;
    for (int i=0; i<tsp.n; i++)
        perm.pb(i);
    shuffle(perm.begin(), perm.end(), rng);
    perm.resize(num_roots);

    double estimate = 0;

    auto tsp0 = tsp;
    for (int v : perm)
    {
        tsp = MST_GradientDescentTransform(tsp0, v);
        estimate = max(estimate, MST(tsp, v).weight);
    }
    return estimate;
}

/*
       MSTGD from 10 vertices : 769365.3985046124 time=0.3949377393
        * MST simple estimate : 769321.8830518434 time=0.0405348143
*/
