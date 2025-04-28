#include "common.h"

static int DELTA_LIMIT = 200;
vector<vector<int> > ExtractEdges(const TSP& tsp, int C)
{
    vector<pair<double, pair<int, int> > > deltas;
    for (int i=0; i<DELTA_LIMIT; i++)
        for (int j=1; j<DELTA_LIMIT; j++)
            deltas.pb({hypot(max<double>(0, i-1), max<double>(0, j-1)), {i, j} });
    sort(deltas.begin(), deltas.end());

    int n = tsp.size();
    int L = 100;
    vector<vector<int> > ans(n);
    map<pair<int, int>, vector<int> > mp;
    for (int i=0; i<n; i++)
        mp[{tsp[i].x/L, tsp[i].y/L}].pb(i);

    for (int i=0; i<n; i++)
    {
        if (i%1000 == 0)
            cout<<i/1000<<"% ";

        set<pair<double, int> > st = { {1e18, -1} };
        auto add = [&](int j)
        {
            st.insert({Dist(tsp, i, j), j});
            if (int(st.size()) > C)
                st.erase(prev(st.end()));
        };
        int x0 = tsp[i].x/L;
        int y0 = tsp[i].y/L;
        for (int j : mp[{x0, y0}])
            if (i != j)
                add(j);
        for (auto [d, pa] : deltas)
        {
            if (d*L >= prev(st.end())->fi)
                break;
            int x = pa.fi;
            int y = pa.se;
            for (int _=0; _<4; _++)
            {
                for (int j : mp[{x0 + x, y0 + y}])
                    add(j);
                swap(x, y);
                y = -y;
            }
        }
        assert(int(st.size()) == C);
        for (auto [d, j] : st)
            ans[i].pb(j);
    }
    cout<<"\n";
    return ans;
}
