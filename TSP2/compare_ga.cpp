#include "solvers.h"
#include "mst.h"

using namespace std;

vector<string> descriptions =
{
    "1..n", // 0
    "sort x", // 1
    "sort x+y", // 2
    "sort euc to cm asc", // 3
    "sort sumabs cm asc", // 4
    "sort maxabs cm asc", // 5
    "sort euc to cm desc", // 6
    "sort sumabs cm desc", // 7
    "sort maxabs cm desc", // 8
    "longest cycle (ga)", // 9
    "shortest cycle (ga)", // 10
    "shortest cycle n/2" // 11
};

vector<int> Get_GA_Permutation(TSP tsp, int type)
{
    int n = tsp.n;
    vector<int> perm;
    for(int i=0; i<n; i++)
        perm.pb(i);

    if (type == 1)
    {
        sort(perm.begin(), perm.end(), [&](int x, int y){ return tsp.coords[x].x < tsp.coords[y].x; });
        return perm;
    }
    if (type == 2)
    {
        sort(perm.begin(), perm.end(), [&](int x, int y){ return tsp.coords[x].x + tsp.coords[x].y < tsp.coords[y].x + tsp.coords[y].y; });
        return perm;
    }
    if (type >= 3 && type <= 8)
    {
        double xcm = 0, ycm = 0;
        for (int i=0; i<n; i++)
            xcm += tsp.coords[i].x, ycm += tsp.coords[i].y;
        xcm /= n, ycm /= n;
        auto dist = [&](double x, double y) -> double
        {
            if (type % 3 == 0) return hypot(x, y);
            if (type % 3 == 1) return abs(x) + abs(y);
            if (type % 3 == 2) return max(abs(x), abs(y));
            return 0;
        };
        sort(perm.begin(), perm.end(), [&](int x, int y){ return dist(tsp.coords[x].x - xcm, tsp.coords[x].y - ycm) <
                                                                 dist(tsp.coords[y].x - xcm, tsp.coords[y].y - ycm); });
        if (type >= 6) reverse(perm.begin(), perm.end());
        return perm;
    }
    if (type == 9)
    {
        TSP inv = tsp;
        for (int i=0; i<n; i++)
            for (int j=0; j<n; j++)
                inv[i][j] *= -1;
        return SolveGreedyAdder(inv, 1);
    }
    if (type == 10)
    {
        return SolveGreedyAdder(tsp, 1);
    }
    if (type == 11)
    {
        auto c = SolveGreedyAdder(tsp, 1);
        vector<int> path;
        for (int i=0; i<n/2; i++)
        {
            path.pb(c[i]);
            if (i+n/2 < n)
                path.pb(c[i+n/2]);
        }
        return path;
    }
    return perm;
}

void CompareGA(int n)
{
    Stats stats;

    double lastOutput = Clock();
    while (true)
    {
        TSP tsp = TSP::GenerateRandom(n);

        for (int i=1; i<int(descriptions.size()); i++)
            stats.AddStat(tsp.Length(SolveGreedyAdderSinglePermutation(tsp, Get_GA_Permutation(tsp, i))), descriptions[i]);

        stats.AddStat(tsp.Length(SolveGreedyAdder(tsp, 1)), "random 1");
        stats.AddStat(tsp.Length(SolveGreedyAdder(tsp, 10)), "random 10");
        stats.AddStat(tsp.Length(SolveGreedyAdder(tsp, 100)), "random 100");
        stats.AddStat(tsp.Length(SolveGreedyAdder(tsp, 1000)), "random 1000");
        stats.AddStat(tsp.Length(OptimizeLocalDP(tsp, SolveGreedyAdder(tsp, 1000), 4)), "random 1000 gopt4");
        stats.AddStat(tsp.Length(OptimizeLocalDP(tsp, SolveGreedyAdder(tsp, 1000), 8)), "random 1000 gopt8");
        stats.AddStat(tsp.Length(OptimizeLocalDP(tsp, SolveGreedyAdder(tsp, 1000), 12)), "random 1000 gopt12");

        auto r = SolveGreedyAdder(tsp, 1000);
        for (int i=2; i<=16; i++)
            r = OptimizeLocalDP(tsp, r, i);
        stats.AddStat(tsp.Length(r), "random 1000 gopt-16");




        if (Clock() - lastOutput > 1)
        {
            cout<<stats<<endl;

            lastOutput = Clock();
        }
    }
}

