#include "solvers.h"

using namespace std;

std::vector<int> SolveNearestNeighbor(const TSP& tsp, int from)
{
    int n = tsp.n;
    Require(from >= 0 && from < n, "SolveNearestNeighbor: wrong starting point");
    vector<int> ans;
    vector<bool> used(n);
    ans.pb(from);
    used[from] = true;
    for (int _=0; _<n-1; _++)
    {
        int best = -1;
        for (int i=0; i<n; i++)
            if (!used[i])
                if (best == -1 || tsp[ans.back()][i] < tsp[ans.back()][best])
                    best = i;
        ans.pb(best);
        used[best] = 1;
    }
    return ans;
}
