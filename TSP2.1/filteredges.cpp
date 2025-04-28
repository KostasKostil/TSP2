#include "common.h"

vector<vector<int> > FilterEdges(const vector<vector<int> >& g, int C)
{
    int n = g.size();
    map<pair<int, int>, int> mp;
    for (int i=0; i<n; i++)
        for (int j=0; j<min<int>(C, g[i].size()); j++)
        {
            int u = i;
            int v = g[i][j];
            mp[{min(u, v), max(u, v)}]++;
        }
    vector<vector<int> > res(n);
    for (auto [pa, cnt] : mp)
        if (cnt == 2)
        {
            res[pa.fi].pb(pa.se);
            res[pa.se].pb(pa.fi);
        }
    for (int i=0; i<n; i++)
    {
        set<int> cur;
        for (int j : res[i])
            cur.insert(j);
        res[i].clear();
        for (int j : g[i])
            if (cur.count(j))
                res[i].pb(j);
    }
    return res;
}
