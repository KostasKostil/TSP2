#include "solvers.h"
#include "mst.h"
#include "mst2.h"

using namespace std;

Stats::Stats()
{
    lastTime = Clock();
}
void Stats::AddStat(std::string name, double value)
{
    double t = Clock() - lastTime;
    lastTime = Clock();

    cnt[name]++;
    stats[name]+=value;
    times[name]+=t;
}
void Stats::AddStat(double value, std::string name)
{
    AddStat(name, value);
}

std::ostream& operator<<(std::ostream& out, const Stats& s)
{
    vector<pair<double, string> > stats;
    int m = -1;
    int l = 0;

    for (auto [name, x] : s.stats)
    {
        Assert(s.cnt.count(name), "operator<<Stats s.cnt.count(name)==false");
        if (m!=-1) Assert(s.cnt.at(name)==m, [&](ostream& out){ out<<"operator<<Stats s.cnt[name] != m ("<<name<<", "<<s.cnt.at(name)<<"!="<<m<<")";});
        m = s.cnt.at(name);

        stats.pb({x/m, name});
        l = max<int>(l, name.size());
    }

    sort(stats.begin(), stats.end());
    reverse(stats.begin(), stats.end());

    out<<"Statistics successfully collected for "<<m<<" test cases:\n";
    for (auto [x, name] : stats)
    {
        out<<setw(l)<<name<<" : "<<x<<" time="<<s.times.at(name)/m<<"\n";
    }

    return out;
}


TSP Test_MST_GradientDescentTransform(TSP tsp, int root, int mode, int limit)
{
    int n = tsp.n;
    double step = 1e5;
    MST mst = MST(tsp, root);
    auto iterations = 0;
    double minstep = step;

    while (step > EPS && iterations < limit)
    {
        minstep = min(minstep, step);
        TSP next_tsp = tsp;
        for (int i=0; i<n; i++)
            if (mst.deg[i] != 2)
                for (int j=0; j<n; j++)
                    if (j != i)
                    {
                        next_tsp[i][j] += (mst.deg[i]-2)*step;
                        next_tsp[j][i] += (mst.deg[i]-2)*step;
                    }
        MST next_mst(next_tsp, root);
        if (next_mst.weight > mst.weight)
        {
            if (mode == 0) ;
            if (mode == 1) step *= 0.95;
            if (mode == 2) ;
            if (mode == 3) step *= 0.75;
            if (mode == 4) step *= 2;
            if (mode == 5) step *= 2;

            tsp = next_tsp;
            mst = next_mst;
        }
        else
        {
            if (mode == 0) step *= 0.95;
            if (mode == 1) step *= 0.95;
            if (mode == 2) step *= 0.75;
            if (mode == 3) step *= 0.75;
            if (mode == 4) step *= 0.5;
            if (mode == 5) step = minstep * 0.5;
        }

        iterations++;
    }
    return tsp;
}

TSP Alpha_MST_GradientDescentTransform(TSP tsp, int root, double alpha)
{
    int n = tsp.n;
    double step = 1e5;
    MST mst = MST(tsp, root);
    auto iterations = 0;
    double minstep = step;

    while (step > EPS && iterations < 1000)
    {
        minstep = min(minstep, step);
        TSP next_tsp = tsp;
        for (int i=0; i<n; i++)
            if (mst.deg[i] != 2)
                for (int j=0; j<n; j++)
                    if (j != i)
                    {
                        next_tsp[i][j] += (mst.deg[i]-2)*step;
                        next_tsp[j][i] += (mst.deg[i]-2)*step;
                    }
        MST next_mst(next_tsp, root);
        if (next_mst.weight > mst.weight)
        {
            tsp = next_tsp;
            mst = next_mst;
        }
        else
        {
            step *= alpha;
        }

        iterations++;
    }
    return tsp;
}



void CollectStatsTestGraDesc(int n)
{
    Stats stats;

    double lastOutput = Clock();
    while (true)
    {
        TSP tsp = TSP::GenerateRandom(n);

        {
            auto sol = SolveNearestNeighbor(tsp);
            stats.AddStat(tsp.Length(sol), "NearestNeighbour");
            sol = OptimizeLocalDP(tsp, sol, 8);
            stats.AddStat(tsp.Length(sol), "NearestNeighbour + opt8");
        }
        {
            auto sol = SolveGreedyEdgePicker(tsp, 1);
            stats.AddStat(tsp.Length(sol), "GreedyEdgePicker");
            sol = OptimizeLocalDP(tsp, sol, 8);
            stats.AddStat(tsp.Length(sol), "GreedyEdgePicker + opt8");
        }
        {
            auto sol = SolveGreedyEdgePicker(tsp, 12);
            stats.AddStat(tsp.Length(sol), "GreedyEdgePicker sz=12");
            sol = OptimizeLocalDP(tsp, sol, 8);
            stats.AddStat(tsp.Length(sol), "GreedyEdgePicker sz=12 + opt8");
        }

        stats.AddStat(MST(tsp, 0).weight, "MST simple estimate");

        vector<int> lims = {250, 1000, 4000};
        for (int lim : lims)
        for (int i=0; i<6; i++)
        {
            auto tsp2 = Test_MST_GradientDescentTransform(tsp, 0, i, lim);
            stats.AddStat(MST(tsp2, 0).weight, "MST Test Gradient descent estimate " + ConvertToString(i) + "/" + ConvertToString(lim));
        }

        vector<double> alphas = {0.99, 0.97, 0.95, 0.9, 0.8};
        for (double a : alphas)
        {
            auto tsp2 = Alpha_MST_GradientDescentTransform(tsp, 0, a);
            stats.AddStat(MST(tsp2, 0).weight, "MST Alpha Gradient descent estimate " + ConvertToString(a));
        }

        if (Clock() - lastOutput > 1)
        {
            cout<<stats<<endl;

            lastOutput = Clock();
        }
    }
}

void CollectStats(int n)
{
    Stats stats;

    double lastOutput = Clock();
    while (true)
    {
        TSP tsp = TSP::GenerateRandom(n);

//        stats.AddStat(Estimate_MST_GraDesc(tsp, 10), "MSTGD from 10 vertices");
        for (int mst_transform = 0; mst_transform <= 1; mst_transform++)
        {
            if (mst_transform == 1)
            {
                stats.AddStat(MST(tsp, 0).weight, "MST simple estimate");
//                stats.AddStat(Estimate_MST2(tsp, 100, 0), "MST2 simple estimate");

                tsp = MST_GradientDescentTransform(tsp, 0);

                stats.AddStat(MST(tsp, 0).weight, "* MST simple estimate");
//                stats.AddStat(Estimate_MST2(tsp, 100, 0), "** MST2 simple estimate");
            }

            for (int dp_opt = 0; dp_opt <= 0; dp_opt++)
            {
                string pref = "";
                string suf = "";
                if (mst_transform) pref = "*";
                if (dp_opt) suf = " +opt8";

                auto go = [&](vector<int> sol, string name)
                {
                    if (dp_opt) sol = OptimizeLocalDP(tsp, sol, 8);
                    stats.AddStat(tsp.Length(sol), pref + name + suf);
                };

                go(SolveNearestNeighbor(tsp), "NearestNeighbour");
                go(SolveGreedyEdgePicker(tsp), "GreedyEdgePicker");
//                go(SolveGreedyEdgePicker(tsp, 12), "GreedyEdgePicker sz=12");

                if (mst_transform == 0)
                {
                    go(SolveGreedyAdder(tsp), "GreedyAdder");
                    go(SolveGreedyAdder(tsp, 200), "GreedyAdder iter=200");
                }

//                {
//                    auto matr = ComputeMST_Matrix(tsp, 0);
//                    auto tspx = tsp;
//                    for (int i=0; i<tsp.n; i++)
//                        for (int j=0; j<tsp.n; j++)
//                            if (i != j)
//                                tspx[i][j] = max(0.0, tsp[i][j] - matr[i][j]);
//                    go(SolveHamiltonian(tspx, tsp), "Hamiltonian");
//                }
            }
        }

        if (Clock() - lastOutput > 1)
        {
            cout<<stats<<endl;

            lastOutput = Clock();
        }
    }
}
