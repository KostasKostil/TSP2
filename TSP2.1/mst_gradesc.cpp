#include "common.h"

#define root 0
#define alpha 0.25

vector<MSTEdge> GetVectorOfMSTEdges(const TSP& tsp, vector<vector<int> > g)
{
    int n = tsp.size();
    vector<MSTEdge> ed;
    for (int i=0; i<n; i++)
        for (int j : g[i])
            if (j > i)
                ed.pb(MSTEdge{i, j, Dist(tsp, i, j)});
    return ed;
}

vector<double> MST_Gradesc(int n, vector<MSTEdge> ed)
{
    vector<double> W(n, 0);

    {
        ifstream fin("data/w.txt");
        for (double& w : W)
            fin>>w;
    }
    vector<double> grad(n, 0);
    int iter = 1;
    ofstream flog("log.txt");
    flog<<fixed<<setprecision(18);

//    for (double step = 1000; ; step *= (1 - step/1e6))
    for (double step = 0.0; ; step = 0.02 * (1.00 + rng()*0.01/UINT_MAX))
    {
        double T = 0;
        DSU dsu(n);
        vector<int> deg(n);
        sort(ed.begin(), ed.end(), [&](const MSTEdge& l, const MSTEdge& r){ return l.w + W[l.u] + W[l.v] < r.w + W[r.u] + W[r.v]; });
        for (const MSTEdge& e : ed)
        {
            if (e.u == root || e.v == root)
            {
                if (deg[root] < 2)
                    deg[e.u]++, deg[e.v]++, T+=e.w+W[e.u]+W[e.v];
            }
            else
            if (dsu.Get(e.u) != dsu.Get(e.v))
            {
                dsu.Unite(e.u, e.v);
                deg[e.u]++, deg[e.v]++, T+=e.w+W[e.u]+W[e.v];
            }
        }
        for (int i=0; i<n; i++)
        {
            T -= 2*W[i];
            grad[i] = alpha*(deg[i] - 2) + (1 - alpha)*grad[i];
            W[i] += step*grad[i];
        }

        if (iter%100 == 0)
        {
            ofstream os("data/w.txt");
            os<<fixed<<setprecision(18);
            for (int i=0; i<n; i++)
                os<<W[i]<<"\n";

            map<int, int> mp;
            for (int i=0; i<n; i++)
                mp[deg[i]]++;
            PrintMapStats(mp);
        }
        cout<<iter<<" "<<step<<" "<<T<<"\n";
        flog<<iter<<" "<<T<<"\n";
        iter++;
    }
}
