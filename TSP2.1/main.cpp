#include "common.h"

vector<int> LoadTour(int n, string filename)
{
    ifstream fin(filename);
    vector<int> v(n);
    for (int& i : v)
        fin>>i;
    return v;
}
vector<int> RandomTour(int n)
{
    vector<int> t0;
    for (int i=0; i<n; i++)
        t0.pb(i);
    shuffle(t0.begin(), t0.end(), rng);
    return t0;
}

int main()
{
    cout<<fixed<<setprecision(10);

    /// reading
    TSP tsp = ReadCoords("tsp/monalisa.tsp");
    auto tour = ReadTour("tsp/monalisa.tour");

    /// calculating edges which are in top-10 for both endpoints
    int n = tsp.size();
    auto g = ExtractEdges(tsp, 10);
    auto gex = ExtractEdges(tsp, 100);
    g = FilterEdges(g, 10);
    TourStats(g, tour);
    GlobalStats(g, 10);

//    MST_Gradesc(n, GetVectorOfMSTEdges(tsp, g));
//    return 0;

    /// W-transform
    ifstream wfile("data/w5751572.txt");
    vector<double> W(n);
    for (double& x : W) wfile>>x;
    for (int i=0; i<n; i++)
        sort(g[i].begin(), g[i].end(), [&](int l, int r){ return Dist(tsp, i, l) + W[l] < Dist(tsp, i, r) + W[r]; });
    TourStats(g, tour);
    GlobalStats(g, 10);
    for (int i=0; i<n; i++)
        Common::W.pb(W[i] + 0.5);

    /// alpha-nearness
    vector<vector<AlphaEdge> > G;
//    G = AlphaNearness(tsp, g, W);
//    SaveAlphaGraph(G, "data/alphagraph.txt");
    G = LoadAlphaGraph("data/alphagraph.txt");
    for (int i=0; i<n; i++)
    {
        sort(G[i].begin(), G[i].end(), [&](const AlphaEdge& l, const AlphaEdge& r){ return l.alpha < r.alpha; });
        g[i].clear();
        for (const AlphaEdge& e : G[i])
            g[i].pb(e.v);
    }
    TourStats(g, tour);
    GlobalStats(g, 10);
    g = FilterEdges(g, 7);

    ifstream fin("data/dp.txt");
    Common::DP.resize(n);
    for (int& i : Common::DP)
        fin>>i;

//    int id = 0;
//    while (true)
//    {
////        auto t = InsertionTour(tsp, gex);
//        auto t = RandomTour(n);
////        auto t = LoadTour(n, "data/5762191.txt");
//        LKH lkh(tsp, g, W);
//        lkh.InitialOptimize(t);
//
//        int L = Length(tsp, t);
//        stringstream ss; ss<<L<<"_"<<(id++);
//        ofstream fout("tours/initial3/" + ss.str() + ".txt");
//        for (int i : t) fout<<i<<" ";
//    }
//    return 0;

//    {
////        vector<int> t = LoadTour(n, "tours/initial3/5759521_241.txt");
////        vector<int> t = LoadTour(n, "tours/res/5758000.txt");
//        vector<int> t = LoadTour(n, "tours/lkh.txt");
//        LKH lkh(tsp, g, W);
//        lkh.FinalOptimize(t);
//    }

    /// solver
//    Common::Wflag = true;
//    vector<int> t = LoadTour(n, "data/5762191.txt");
//    shuffle(t.begin(), t.end(), rng);

//    auto wa = Wevaluate(tsp, g, t);
//    for (int i=0; i<n; i++) Common::W[i] += wa[i];
//    Common::W.assign(n, 0);

/// checking a path
//    Visualizer vis;
//    vis.SetTSP(tsp);
//    vis.SetCycle(255, 0, 0, t);
//    vector<pair<int, int> > ed = ;
//    vis.SetGraph(0, 0, 255, ed);
//    return 0;

    vector<int> t = LoadTour(n, "tours/lkh.txt");
    cout<<"LENGTH = "<<Length(tsp, t)<<"\n";

/// compare to world record
    {
        Visualizer vis;
        vis.SetTSP(tsp);
        vis.SetCycle(0, 255, 0, tour);
        vis.SetCycle(255, 0, 0, t);

        auto num = [&](int x, int y){ return (long long)(n*max(x, y)) + min(x, y); };

        set<long long> st;
        for (int i=0; i<n; i++)
        {
            long long x = num(t[i], t[(i+1)%n]);
            st.insert(x);
        }
        int cnt = 0;
        for (int i=0; i<n; i++)
        {
            long long x = num(tour[i], tour[(i+1)%n]);
            if (st.count(x))
                cnt++;
        }
        cout<<"COMMON EDGES: "<<cnt<<"\n";
        return 0;
    }

//    assert(int(set<int>(t.begin(), t.end()).size()) == n);
//    FeasibleLKH(tsp, g, G, t);
//    SequentialLKH(tsp, g, G, t);

    return 0;

}
