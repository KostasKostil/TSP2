#include "solvers.h"

using namespace std;

class PermReverse
{
public:
    std::vector<int> perm;
    std::vector<int> inv;

    PermReverse(const std::vector<int>& p) : perm(p), inv(p.size())
    {
        int n = perm.size();
        for (int i = 0; i < n; ++i)
            inv[perm[i]] = i;
    }

    inline void Reverse(int r)
    {
        reverse(perm.begin(), perm.begin() + r);
        for (int i = 0; i < r; ++i)
            inv[perm[i]] = i;
    }
    inline void Reverse(int l, int r)
    {
        reverse(perm.begin() + l, perm.begin() + r);
        for (int i = l; i < r; ++i)
            inv[perm[i]] = i;
    }
};


std::vector<int> FindHamiltinianCycle(const TSP& tsp)
{
    int n = tsp.n;
    PermReverse p(SolveGreedyEdgePicker(tsp));
    double cur = tsp.Length(p.perm);

    vector<vector<int> > G(n);
    for (int i=0; i<n; i++)
        for (int j=i+1; j<n; j++)
            if (tsp[i][j] == 0)
                G[i].pb(j), G[j].pb(i);


    vector<int> rperm = p.perm;
    while (cur > 0)
    {
        cout<<cur<<"\n";

        shuffle(rperm.begin(), rperm.end(), rng);
        for (int i : rperm)
            if (tsp[p.perm[i]][p.perm[(i+1)%n]] > 0)
            {
                if (i != n-1)
                {
                    p.Reverse(0, i+1);
                    p.Reverse(i+1, n);
                }
                break;
            }

        while (true)
        {   // trying to improve path
            int v = p.perm[0];
            int u = p.perm[1];
            while (u == p.perm[1])
                u = G[v][rng()%G[v].size()];

            p.Reverse(p.inv[u]);
            if (tsp[p.perm[0]][p.perm.back()] == 0)
            {
                cur--;
                break;
            }
        }
    }
    return p.perm;
}
