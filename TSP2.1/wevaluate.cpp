#include "common.h"

vector<int> Wevaluate(const TSP& tsp, const vector<vector<int> >& g0, const vector<int>& tour)
{
    int n = tsp.size();

    vector<array<int, 3> > edges;
    for (int i=0; i<n; i++)
    {
        int v = tour[(i+1)%n];
        int a = tour[(i+0)%n];
        int b = tour[(i+2)%n];

        int cnt = 0;
        for (int i : g0[v])
            if (i != a && i != b)
                for (int u : {a, b})
                {
                    cnt++;
                    // edge u-v must be <= than edge i-v
                    // e[uv] + w[u] + w[v] <= e[iv] + w[i] + w[v]
                    // w[u] <= w[i] + (e[iv] - e[uv])
                    edges.pb({u, i, Dist(tsp, i, v) - Dist(tsp, u, v)});
                }
    }
    // now in graph on edges we don't want negative cycles, otherwise
    // w[u] <= w[i] + c[ui] <= ... <= w[u] + (sum[c])

    vector<vector<pair<int, int> > > g(n); // graph
    vector<int> h(n, 0); // potentials (e.g. negative current w values)
    vector<pair<int, int> > changes; // to revert if it is not possible to add edge

//    sort(edges.begin(), edges.end(), [&](array<int, 3> a, array<int, 3> b){ return a[2] > b[2]; });
//    shuffle(edges.begin(), edges.end(), rng);

    int good = 0;
    int id = 0;
    for (auto [u, v, c] : edges)
    {
        if ((++id)%10000 == 0) cout<<id<<" ";
        g[u].pb({v, c});

        queue<int> q;
        changes.clear();
        auto upd = [&](int i, int new_value) -> bool
        { // returns true iff we need to break
            if (h[i] <= new_value)
                return false;
            if (i == u)
                return true;

            changes.pb({i, h[i]});
            h[i] = new_value;
            q.push(i);
            return false;
        };

        bool bad = upd(v, h[u] + c);
        while (!q.empty())
        {
            int a = q.front();
            q.pop();
            for (auto [b, c] : g[a])
                if (upd(b, h[a] + c))
                {
                    bad = true;
                    break;
                }
            if (bad)
                break;
        }

        if (bad)
        {
            g[u].pop_back();
            reverse(changes.begin(), changes.end());
            for (auto [i, x] : changes) h[i] = x;
        }
        else
            good++;
    }
    cout<<good<<"/"<<edges.size()<<" = "<<good*100.0/edges.size()<<"%\n";
    for (int& i : h) i = -i;
    return h;
}
