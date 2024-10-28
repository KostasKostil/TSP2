#include "solvers.h"
#include "smart_set.h"

using namespace std;

std::vector<int> OptimizeLocalDP(const TSP& tsp, std::vector<int> tour, int dp_size)
{
    int n = tsp.size();
    int m = min(dp_size, n-1);
    Require(m >= 1 && m <= 30, "OptimizeLocalDP: wrong dp_size");

    auto run_dp = [&](int from, vector<int> v, int to) -> pair<double, vector<int> >
    {
        double** dp = new double*[m];
        int** p = new int*[m];
        for (int i=0; i<m; i++)
            dp[i] = new double[1<<m],
            p[i] = new int[1<<m];
        for (int i=0; i<m; i++)
            for (int j=0; j<(1<<m); j++)
                dp[i][j] = INFDP,
                p[i][j] = -1;
        for (int i=0; i<m; i++)
            dp[i][(1<<m)-1] = tsp[v[i]][to];
        for (int mask=(1<<m)-2; mask>=0; mask--)
            for (int from=0; from<m; from++)
                if (((mask>>from)&1)==1)
                    for (int to=0; to<m; to++)
                        if (((mask>>to)&1)==0)
                        {
                            double nt = dp[to][mask | (1<<to)] + tsp[v[from]][v[to]];
                            if (nt < dp[from][mask])
                            {
                                dp[from][mask] = nt;
                                p[from][mask] = to;
                            }
                        }
        for (int i=0; i<m; i++)
        {
            double nt = dp[i][1<<i] + tsp[from][v[i]];
            if (nt < dp[0][0])
            {
                dp[0][0] = nt;
                p[0][0] = i;
            }
        }
        int u = 0;
        int mask = 0;
        vector<int> ans;
        while (mask != (1<<m)-1)
        {
            u = p[u][mask];
            mask |= (1<<u);
            ans.pb(v[u]);
        }
        double res = tsp[from][ans[0]] + tsp[ans.back()][to];
        for (int i=0; i+1<int(ans.size()); i++)
            res += tsp[ans[i]][ans[i+1]];
        for (int i=0; i<m; i++)
        {
            delete[] dp[i];
            delete[] p[i];
        }
        delete[] dp;
        delete[] p;
        return {res, ans};
    };

    SmartSet st(n);
    vector<vector<int> > paths(n);

    auto set_value = [&](int pos)
    {
        vector<int> v;
        for (int i=1; i<=m; i++)
            v.pb(tour[(pos+i)%n]);
        auto [new_len, path] = run_dp(tour[pos], v, tour[(pos+m+1)%n]);
        double old_len = 0;
        for (int i=0; i<=m; i++)
            old_len += tsp[tour[(pos+i)%n]][tour[(pos+i+1)%n]];
        Assert(old_len >= new_len - EPS, [&](ostream& out){ out<<"OptimizeLocalDP: DP is not optimal: "<<old_len<<" => "<<new_len<<", diff = "<<old_len-new_len; });

        st.Assign(pos, old_len - new_len);
        paths[pos] = path;
    };

    for (int i=0; i<n; i++)
        set_value(i);

    while (true)
    {
        int pos = st.MaxId();
        if (st.a[pos] < EPS)
            break;

        for (int i=1; i<=m; i++)
            tour[(pos+i)%n] = paths[pos][i-1];
        // changed [pos+1 .. pos+m]
        // ith guy uses [i .. i+m+1]
        // so it is important to [pos-m .. pos+m]
        int L = pos-m+n;
        int R = pos+m+n;
        R = min(R, L+n-1);
        for (int i=L; i<=R; i++)
            set_value(i%n);
    }
    return tour;
}
