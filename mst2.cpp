#include "mst2.h"
#include "common.h"

using namespace std;

MST2::MST2(int n_, std::vector<Edge> edges, int k)
{
    n = n_;
    m = edges.size();
    ed = edges;
    g.resize(n);
    for (int i=0; i<m; i++)
    {
        g[ed[i].u].pb({ed[i].v, i});
        g[ed[i].v].pb({ed[i].u, i});
    }

    Initialization();
    for (int i=1; i<k; i++)
        if (!Iteration(i))
            break;
}

void MST2::Initialization() // generates 0-th tree
{
    in = out = T = {vector<bool>(m, false)};
    Q = {vector<int>(m, -1)};
    Qopt = {-1};

    vector<pair<double, int> > ed_sorted;
    for (int i=0; i<m; i++)
        ed_sorted.pb({ed[i].w, i});
    sort(ed_sorted.begin(), ed_sorted.end());

    double w0 = 0;
    DSU dsu;
    for (auto [w, id] : ed_sorted)
    {
        int u = ed[id].u;
        int v = ed[id].v;
        if (dsu.Get(u) == dsu.Get(v))
            continue;
        dsu.Unite(u, v);
        T[0][id] = true;
        w0 += w;
    }

    W = {w0};

    for (int i=0; i<m; i++)
        if (T[0][i])
            UpdateQ(0, i);
    SetQopt(0);
}

bool MST2::Iteration(int k) // calculates k-th tree using [0, k-1]th trees
{
    int I = -1; // find best exchange
    for (int i=0; i<k; i++)
        if (Qopt[i] != -1)
            if (I == -1 || W[i] + Cost(Q[i][Qopt[i]], Qopt[i]) < W[I] + Cost(Q[I][Qopt[I]], Qopt[I]))
                I = i;
    if (I == -1)
        return false;
    int i = I;

    int f = Qopt[i]; // do the exchange
    int e = Q[i][f];
    in.pb(in[i]);
    out.pb(out[i]);
    T.pb(T[i]);
    Q.pb(Q[i]);
    Qopt.pb(-1);

    W.pb(W[i] - ed[e].w + ed[f].w);
    in[k][f] = true;
    out[i][f] = true;
    T[k][e] = false;
    T[k][f] = true;
    SetQopt(i);

    // now we need to recalc Q[k]
    // e.g. for all edges [a, b] such that edge e lied on path [a, b] we
    UpdateQ(k, f);
    SetQopt(k);
    return true;
}

bool MST2::Valid(int i, int f)
{
    return !out[i][f] && !T[i][f];
}

double MST2::Cost(int e, int f)
{
    return ed[f].w - ed[e].w;
}

void MST2::SetQopt(int i)
{
    Qopt[i] = -1;
    for (int f=0; f<m; f++)
        if (Valid(i, f))
            if (Q[i][f] != -1)
                if (Qopt[i] == -1 || Cost(Q[i][f], f) < Cost(Q[i][Qopt[i]], Qopt[i]))
                    Qopt[i] = f;
}

std::vector<std::pair<double, int> > MST2::CalcPrefixMaximas(int k, int v, int p)
{
    vector<pair<double, int> > ans(n, {-INFDP, -2});
    ans[v] = {-INFDP, -1};

    auto dfs = [&](auto&& self, int v, int p) -> void
    {
        for (auto [u, id] : g[v])
            if (u != p && T[k][id])
            {
                ans[u] = ans[v];
                if (!in[k][id])
                    ans[u] = max(ans[u], {ed[id].w, id});
                self(self, u, v);
            }
    };

    dfs(dfs, v, p);
    return ans;
}

void MST2::UpdateQ(int k, int f)
{
    int U = ed[f].u;
    int V = ed[f].v;
    auto prefmaxU = CalcPrefixMaximas(k, U, V);
    auto prefmaxV = CalcPrefixMaximas(k, V, U);

    for (int id=0; id<m; id++)
        if (!T[k][id] && !out[k][id])
        {
            int A = ed[id].u;
            int B = ed[id].v;
            Assert((prefmaxU[A].se == -2) != (prefmaxV[A].se == -2), "MST2::Iteration error: both parts of tree");
            Assert((prefmaxU[B].se == -2) != (prefmaxV[B].se == -2), "MST2::Iteration error: both parts of tree");

            if (prefmaxU[A].se == -2)
                swap(A, B);
            if (prefmaxU[A].se == -2 || prefmaxU[B].se != -2)
                continue;

            pair<double, int> best = max(prefmaxU[A], prefmaxV[B]);
            if (best.se == -2)
                best.se = -1;
            if (!in[k][f])
                best = max(best, {ed[f].w, f});

            Q[k][id] = best.se;
        }
}
