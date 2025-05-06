#include "common.h"

LKH::LKH(vector<pair<int, int> > tsp_, vector<vector<int> > g_, vector<double> W_)
{
    n = tsp_.size();
    for (int i=0; i<n; i++)
        tsp.pb(Point{tsp_[i].x, tsp_[i].y});
    g = g_;
    W_optimal = W_;

    L_Output = 1e9;
    T_Output = -1e9;
    updates_since_last_output = 0;
    tries_since_last_output = 0;
    teleport_limit = 0;
    overflow_counter = 0;
}


int LKH::Length(vector<int> tour)
{
    int s = 0;
    for (int i=0; i<n; i++)
        s += Dist(tour[i], tour[(i+1)%n]);
    return s;
}
int LKH::Segments()
{
    return segments.size();
}
vector<int> LKH::Tour()
{
    vector<int> t;
    for (int i=0; i<n; i++)
        t.pb(pr->At(i));
    return t;
}
void LKH::CheckForOutput()
{
    double timelimit = 4.0;
    double T = Time();
    if (
        (T > T_Output + timelimit)
        ||
        (T > T_Output + 2*timelimit && curlength != L_Output)
        )
    {
        if (overflow_counter > 0)
            cout<<"overflows: "<<overflow_counter<<"\n";
        cout<<"current length: "<<curlength<<", upd="<<updates_since_last_output<<"/"<<tries_since_last_output<<", t="<<T-T_Init<<".\n";
        updates_since_last_output = 0;
        tries_since_last_output = 0;

        auto t = Tour();
        assert(Length(t) == curlength);

        ofstream fout("tours/lkh.txt");
        for (int i : t)
            fout<<i<<"\n";

        T_Output = Time();
        L_Output = curlength;
        overflow_counter = 0;
    }
}
void LKH::OutputSkipStage()
{
    T_Output = -1e9;
    L_Output = 1e9;
    CheckForOutput();
}
void LKH::InitializeGo()
{
    tries_since_last_output++;
    overflown = false;
    segments = {n};
    state = SetHash();
    changed.Clear();
    states.Clear();
}

int LKH::DpSingleRecalc(int v, int depth)
{
    if (depth <= 0)
        return dp[v];

    int best = 1e9;
    int pos = pr->Where(v);
    int crit = dp[v];

    vector<int> d_ord = {-1, 1};
    vector<int> u_ord;
    for (int d : d_ord)
        u_ord.pb(pr->At((pos+d+n)%n));
    if (DistW(v, u_ord[0]) < DistW(v, u_ord[1]))
        swap(d_ord[0], d_ord[1]);
    for (int d : d_ord)
    {
        int u = pr->At((pos+d+n)%n);
        if (changed.Count(num(v, u)))
            continue;
        changed.Insert(num(v, u));
        for (int w : g[u])
            if (w != v && w != pr->At((pos+2*d+n)%n))
            {
                if (changed.Count(num(u, w)))
                    continue;
                int extraneed = DistW(u, w) - DistW(v, u);
                if (extraneed + dp[w] >= best)
                    continue;

                changed.Insert(num(u, w));
                best = min(best, extraneed + max(dp[w], DpSingleRecalc(w, depth-1) ) );

                changed.Undo();
                if (best <= crit)
                {
                    changed.Undo();
                    return crit;
                }
            }
        changed.Undo();
    }
    return best;
}

void LKH::DpFullRecalc()
{
    cout<<"Starting DP calculation...\n";

    dp.assign(n, 0);

    for (int d = 1; d <= 15; d++)
    {
        for (int i=0; i<n; i++)
        {
            changed.Clear();
            dp[i] = max(dp[i], DpSingleRecalc(i, d));
        }

        cout<<"dp depth "<<d<<":\n";
        map<int, int> mp;
        for (int i=0; i<n; i++)
            mp[min(10, dp[i])]++;
        PrintMapStats(mp);
    }

    ofstream fout("data/dp.txt");
    for (int i : dp)
        fout<<i<<endl;
    cout<<"DP calculated!\n";
}
