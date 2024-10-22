#include "common.h"

using namespace std;

/// HELPERS
void Assert(bool flag, std::string message)
{
    if (!flag)
    {
        cerr<<message<<endl;
        exit(0xdead);
    }
}
void Assert(bool flag, std::function<void(std::ostream&)> message)
{
    if (!flag)
    {
        message(cerr);
        exit(0xdead);
    }
}
double Distance(Point A, Point B)
{
    return hypot(A.x-B.x, A.y-B.y);
}
double Distance(Point A, Point B, Metric metric)
{
    switch (metric)
    {
        case Metric::Euclidean: return hypot(A.x-B.x, A.y-B.y);
        case Metric::Manhattan: return abs(A.x-B.x) + abs(A.y-B.y);
        case Metric::Chebyshev: return max(abs(A.x-B.x), abs(A.y-B.y));
    }
    Assert(false, "Distance: unknown metric");
    return 0;
}

/// TSP
TSP::TSP(){}
TSP::TSP(std::istream& in)
{
    in>>n;
    assign(n, vector<double>(n));
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
            in>>g(i, j);
    coords.resize(n);
    for (int i=0; i<n; i++)
        in>>coords[i].x>>coords[i].y;

    for (int i=0; i<n; i++)
        for (int j=i+1; j<n; j++)
            Assert(g(i, j) == g(j, i), "TSP constructor: matrix must be symmetrical");
    for (int i=0; i<n; i++)
        Assert(g(i, i) == 0, "TSP constructor: matrix must have zeroes on diagonal");
}
TSP::TSP(std::string filename)
{
    ifstream fin(filename);
    *this = TSP(fin);
}
void TSP::Save(std::ostream& out)
{
    out<<fixed<<setprecision(10);
    out<<n<<"\n";
    for (int i=0; i<n; i++, out<<"\n")
        for (int j=0; j<n; j++)
            out<<g(i, j)<<" ";
    for (int i=0; i<n; i++)
        out<<coords[i].x<<" "<<coords[i].y<<"\n";
    out.flush();
}
void TSP::Save(std::string filename)
{
    ofstream fout(filename);
    Save(fout);
}
double& TSP::g(int i, int j)
{
    return (*this)[i][j];
}
const double& TSP::g(int i, int j) const
{
    return (*this)[i][j];
}
double TSP::Length(const std::vector<int>& tour) const
{
    double ans = 0;
    for (int i=0; i<n; i++)
        ans += g(tour[i], tour[(i+1)%n]);
    return ans;
}
TSP TSP::LoadPlaneTSP(std::istream& in, Metric metric)
{
    TSP tsp;
    in>>tsp.n;
    tsp.coords.resize(tsp.n);
    for (int i=0; i<tsp.n; i++)
        in>>tsp.coords[i].x>>tsp.coords[i].y;
    tsp.assign(tsp.n, vector<double>(tsp.n, 0));
    for (int i=0; i<tsp.n; i++)
        for (int j=i+1; j<tsp.n; j++)
            tsp[i][j] = tsp[j][i] = Distance(tsp.coords[i], tsp.coords[j], metric);
    return tsp;
}
TSP TSP::LoadPlaneTSP(std::string filename, Metric metric)
{
    ifstream fin(filename);
    return LoadPlaneTSP(fin, metric);
}

/// DSU
DSU::DSU(){}
int DSU::Get(int v)
{
    while (int(p.size()) <= v)
    {
        int n = p.size();
        p.push_back(n);
    }
    if (p[v] == v)
        return v;
    return p[v] = Get(p[v]);
}
mt19937 dsu_rng(47);
void DSU::Unite(int u, int v)
{
    u = Get(u);
    v = Get(v);
    if (dsu_rng()&1) swap(u, v);
    p[u] = v;
}
