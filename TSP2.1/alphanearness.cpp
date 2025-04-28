#include "common.h"

#define root 0
vector<vector<AlphaEdge> > AlphaNearness(const TSP& tsp, vector<vector<int> > g, vector<double> W)
{
    int n = tsp.size();
    vector<vector<AlphaEdge> > ans(n);
    vector<MSTEdge> ed = GetVectorOfMSTEdges(tsp, g);

    DSU dsu(n);
    sort(ed.begin(), ed.end(), [&](const MSTEdge& l, const MSTEdge& r){ return l.w + W[l.u] + W[l.v] < r.w + W[r.u] + W[r.v]; });

    vector<vector<int> > t(n);
    for (const MSTEdge& e : ed)
    {
        if (e.u == root || e.v == root)
            continue;
        if (dsu.Get(e.u) != dsu.Get(e.v))
        {
            dsu.Unite(e.u, e.v);
            t[e.u].pb(e.v);
            t[e.v].pb(e.u);
        }
    }

    vector<double> vroot;
    for (int i : g[root])
        vroot.pb(Dist(tsp, root, i) + W[root] + W[i]);
    sort(vroot.begin(), vroot.end());
    double rootcrit = vroot[1];
    for (int i : g[root])
    {
        AlphaEdge ae;
        ae.v = i;
        ae.w0 = Dist(tsp, root, i);
        ae.W = ae.w0 + W[root] + W[i];
        ae.alpha = max<double>(0, ae.W - rootcrit);
        ans[root].pb(ae);
    }
    for (int i=0; i<n; i++)
        if (i != root)
        {
            if ((i+1)%1000 == 0)
                cout<<i<<" ";

            vector<double> mx(n, -1e18);
            auto dfs = [&](auto&& self, int v, int p) -> void
            {
                for (int i : t[v])
                    if (i != p && i != root)
                    {
                        mx[i] = max(mx[v], Dist(tsp, v, i) + W[v] + W[i]);
                        self(self, i, v);
                    }
            };
            dfs(dfs, i, -1);

            for(int j : g[i])
            {
                AlphaEdge ae;
                ae.v = j;
                ae.w0 = Dist(tsp, i, j);
                ae.W = ae.w0 + W[i] + W[j];
                if (j == root)
                    ae.alpha = ae.W - rootcrit;
                else
                    ae.alpha = ae.W - mx[j];
                ans[i].pb(ae);
            }
        }
    cout<<"\n";
    return ans;
}
void SaveAlphaGraph(vector<vector<AlphaEdge> > g, string file)
{
    ofstream fout(file);
    fout<<fixed<<setprecision(18);
    fout<<g.size()<<"\n";
    for (int i=0; i<int(g.size()); i++)
        for (AlphaEdge e : g[i])
            fout<<i<<" "<<e.v<<" "<<e.w0<<" "<<e.W<<" "<<e.alpha<<"\n";
    fout<<-1<<"\n";
}
vector<vector<AlphaEdge> > LoadAlphaGraph(string file)
{
    ifstream fin(file);
    int n, i;
    fin>>n;
    vector<vector<AlphaEdge> > g(n);
    while (fin>>i)
    {
        if (i == -1)
            break;
        AlphaEdge e;
        fin>>e.v>>e.w0>>e.W>>e.alpha;
        g[i].pb(e);
    }
    return g;
}
