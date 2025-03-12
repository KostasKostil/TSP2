#include "mst2.h"
#include "common.h"

using namespace std;

MST2_Bruteforce::MST2_Bruteforce(int n_, std::vector<Edge> edges, int k)
{
    n = n_;
    m = edges.size();
    ed = edges;
    vector<pair<double, vector<bool> > > all_sts;

    for (int mask=0; mask<(1<<m); mask++)
        if (__builtin_popcount(mask) == n-1)
    {
        DSU dsu;
        for (int i=0; i<m; i++)
            if ((mask>>i)&1)
                dsu.Unite(ed[i].u, ed[i].v);
        bool ok = true;
        for (int i=1; i<n; i++)
            if (dsu.Get(0) != dsu.Get(i))
                ok = false;
        if (!ok)
            continue;
        double w = 0;
        vector<bool> v(m, false);
        for (int i=0; i<m; i++)
            if ((mask>>i)&1)
                w += ed[i].w, v[i]=true;
        all_sts.pb({w, v});
    }
    sort(all_sts.begin(), all_sts.end());
    if (int(all_sts.size()) > k)
        all_sts.resize(k);
    for (auto [w, v] : all_sts)
        W.pb(w), T.pb(v);
}

namespace MST2_Test_namespace
{
    void SmallHandTest()
    {
        int n = 4;
        vector<Edge> edges;
        edges.pb(Edge{0, 1, 5.131});
        edges.pb(Edge{1, 2, 3.512});
        edges.pb(Edge{0, 3, 8.122});
        edges.pb(Edge{3, 1, 1.321});
        edges.pb(Edge{2, 3, 7.199});

        MST2 A(n, edges, 100);
        MST2_Bruteforce B(n, edges, 100);

        Assert(A.W.size() == B.W.size(), "MST2::SmallHandTest incorrect size");
        for (int i=0; i<min<int>(A.W.size(), B.W.size()); i++)
            Assert(abs(A.W[i] - B.W[i]) < 1e-6, "MST2::SmallHandTest incorrect W");
    }

    void LargeHandTest()
    {
        int n = 7;
        vector<Edge> edges;
        edges.pb(Edge{0, 1, 5.131});
        edges.pb(Edge{1, 2, 2.512});
        edges.pb(Edge{3, 5, 1.159});
        edges.pb(Edge{3, 4, 9.529});
        edges.pb(Edge{0, 3, 8.122});
        edges.pb(Edge{1, 4, 6.321});
        edges.pb(Edge{4, 5, 2.539});
        edges.pb(Edge{2, 5, 3.529});
        edges.pb(Edge{2, 3, 7.199});
        edges.pb(Edge{0, 6, 9.219});
        edges.pb(Edge{2, 6, 7.609});
        edges.pb(Edge{6, 5, 2.129});

        MST2 A(n, edges, 100);
        MST2_Bruteforce B(n, edges, 100);

        Assert(A.W.size() == B.W.size(), "MST2::LargeHandTest incorrect size");
        for (int i=0; i<min<int>(A.W.size(), B.W.size()); i++)
            Assert(abs(A.W[i] - B.W[i]) < 1e-6, "MST2::LargeHandTest incorrect W");
    }

    void Stress(int n, int k)
    {
        vector<Edge> edges;
        for (int i=0; i<n; i++)
            for (int j=i+1; j<n; j++)
                edges.pb(Edge{i, j, (rng()%100 + 1)*1.0/100});
        MST2 A(n, edges, k);
        MST2_Bruteforce B(n, edges, k);
        Assert(A.W.size() == B.W.size(), "MST2::Stress incorrect size");
        for (int i=0; i<min<int>(A.W.size(), B.W.size()); i++)
           Assert(abs(A.W[i] - B.W[i]) < 1e-6, "MST2::Stress incorrect W");
    }
}

void MST2_Test()
{
    using namespace MST2_Test_namespace;
    SmallHandTest();
    LargeHandTest();
    while (true)
    {
        Stress(7, 1000);
        cerr<<"@";
    }
}
