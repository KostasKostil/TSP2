#include "solvers.h"

using namespace std;

std::vector<int> SolveGreedyAdder(const TSP& tsp, int iterations)
{
    vector<int> perm;
    for (int i=0; i<tsp.n; i++)
        perm.pb(i);

    pair<double, vector<int> > best = {tsp.Length(perm), perm};
    for (int _=0; _<iterations; _++)
    {
        shuffle(perm.begin(), perm.end(), rng);
        vector<int> ans = {perm[0], perm[1], perm[2]};
        for (int ind=3; ind<tsp.n; ind++)
        {
            int s = ans.size();
            int v = perm[ind];
            pair<double, int> best = {-INFDP, -1};
            for (int i=0; i<s; i++)
            {
                double before = tsp[ans[i]][ans[(i+1)%s]];
                double after = tsp[ans[i]][v] + tsp[v][ans[(i+1)%s]];
                best = max(best, {before-after, (i+1)%s});
            }

            ans.insert(ans.begin()+best.se, v);
        }
        best = min(best, {tsp.Length(ans), ans});
    }
    return best.se;
}
