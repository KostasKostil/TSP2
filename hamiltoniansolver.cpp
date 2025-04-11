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

std::vector<int> SolveHamiltonian(const TSP& ham, const TSP& tsp)
{
    typedef pair<double, pair<int, int> > edge;
    edge e0;
    e0.fi = -INFDP;

    auto get = [&](int i, int j) -> edge
    {
        return { ham[min(i, j)][max(i, j)], {min(i, j), max(i, j)} };
    };

    int n = ham.n;
    PermReverse p(SolveGreedyAdder(tsp));

    vector<vector<edge> > G(n);
    for (int i=0; i<n; i++)
        for (int j=i+1; j<n; j++)
        {
            G[i].pb(get(i, j));
            G[j].pb(get(i, j));
        }

    pair<double, vector<int> > best = {tsp.Length(p.perm), p.perm};

    edge crit;
    while (true)
    {
        edge del = e0;
        {   // calculating and deleting maximal edge of current cycle
            for (int i=0; i<n; i++)
                del = max(del, get(p.perm[i], p.perm[(i+1)%n]));
            int a = del.se.fi;
            int b = del.se.se;
            a = p.inv[a];
            b = p.inv[b];
            if (b != (a+1)%n)
                swap(a, b);
            Assert(b == (a+1)%n, "a and b are not adjacent");

            if (a != n-1)
            {
                p.Reverse(0, b);
                p.Reverse(b, n);
            }
        }
        crit = del;
        cout<<"Deleting edge "<<del.fi<<" ["<<del.se.fi<<"-"<<del.se.se<<"]"<<"\n";

        auto gen_rnd = [&](int i)
        {
            int v = -1;
            while (!G[i].empty())
            {
                int j = rng()%G[i].size();
                if (G[i][j] >= del)
                {
                    swap(G[i][j], G[i].back());
                    G[i].pop_back();
                    continue;
                }

                v = G[i][j].se.fi;
                if (v == i)
                    v = G[i][j].se.se;
                break;
            }
            return v;
        };

        if (best.fi < EPS)
            return best.se;

        bool ok = false;
        for (int _=0; _<1e3; _++)
        {   // trying to improve path
            int v = p.perm[0];
            int u = -1;

            while (G[v].size() > 1)
            {
                u = gen_rnd(v);
                if (u == p.perm[1])
                    continue;
                break;
            }
            if (G[v].size() <= 1 || u == -1)
                break;
            p.Reverse(p.inv[u]);

            best = min(best, {tsp.Length(p.perm), p.perm});

            if (get(p.perm[0], p.perm.back()) < del)
            {
                ok = true;
                break;
            }
        }
        if (!ok)
            break;
    }

    vector<int> IND;
    vector<vector<int> > GI(n);
    for (int i=0; i<n; i++)
    {
        for (auto e : G[i])
            if (e <= crit)
        {
            if (e.se.fi == i) GI[i].pb(e.se.se);
            else GI[i].pb(e.se.fi);
        }

        if (GI[i].size() > 2)
            IND.push_back(i);
    }

    auto perm = p.perm;
    cout<<best.fi<<" => ";
    for (int _=0; _<1e4 && IND.size() > 2; _++)
    {
        int x = IND[rng()%IND.size()];
        int xid = p.inv[x];

        int y = -1;
        while ((y == -1) || (y == p.perm[(xid+1)%n]) || (y == p.perm[(xid+n-1)%n]))
            y = GI[x][rng()%int(GI[x].size())];
        int yid = p.inv[y];

        if (yid < xid) swap(xid, yid);

        edge e1 = get(perm[xid%n], perm[yid%n]);
        if (e1 > crit) continue;
        edge e2 = get(perm[(xid+n-1)%n], perm[(yid+n-1)%n]);
        if (e2 > crit) continue;

        Assert(xid >= 0 && yid <= n && xid < yid && int(perm.size()) == n, "xy error");
        reverse(perm.begin()+xid, perm.begin()+yid);
        best = min(best, {tsp.Length(perm), perm});
    }
    cout<<best.fi<<"\n";
//    cout<<"E\n";
    return best.se;
}
