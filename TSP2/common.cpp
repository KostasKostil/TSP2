#include "common.h"

using namespace std;

/// HELPERS
void Assert(bool flag, std::string message)
{
    if (!flag)
    {
        cerr<<"ASSERT FAILED"<<endl<<message<<endl;
        exit(0xdead);
    }
}
void Assert(bool flag, std::function<void(std::ostream&)> message)
{
    if (!flag)
    {
        cerr<<"ASSERT FAILED"<<endl;
        message(cerr);
        exit(0xdead);
    }
}
void Require(bool flag, std::string message)
{
    if (!flag)
    {
        cerr<<"REQUIRE FAILED"<<endl;
        cerr<<message<<endl;
        throw invalid_argument(message);
    }
}
void Require(bool flag, std::function<void(std::ostream&)> message)
{
    if (!flag)
    {
        cerr<<"REQUIRE FAILED"<<endl;
        stringstream ss;
        message(ss);
        cerr<<ss.str()<<endl;
        throw invalid_argument(ss.str());
    }
}

double Clock()
{
    static const auto startTime = chrono::high_resolution_clock::now();
    auto currentTime = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = currentTime - startTime;
    return elapsed.count();
}
std::string ConvertToString(int x)
{
    stringstream ss;
    ss<<x;
    return ss.str();
}
std::string ConvertToString(double x)
{
    stringstream ss;
    ss<<x;
    return ss.str();
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
            Require(g(i, j) == g(j, i), "TSP constructor: matrix must be symmetrical");
    for (int i=0; i<n; i++)
        Require(g(i, i) == 0, "TSP constructor: matrix must have zeroes on diagonal");
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
    Require(int(tour.size()) == n, "TSP::Length tour length is not n");
    vector<bool> used(n);
    for (int i=0; i<n; i++)
    {
        Require(tour[i] >= 0 && tour[i] < n, "TSP::Length invalid vertex number");
        used[tour[i]] = true;
    }
    for (int i=0; i<n; i++)
        Require(used[i], "TSP::Length not all vertices are unique");

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
TSP TSP::GenerateRandom(int n, double AMAX, Metric metric)
{
    stringstream strm;
    strm<<fixed<<setprecision(10);
    strm<<n<<"\n";
    for (int i=0; i<n; i++)
    {
        strm<<rng()*AMAX/UINT_MAX<<" ";
        strm<<rng()*AMAX/UINT_MAX<<"\n";
    }
    return TSP::LoadPlaneTSP(strm, metric);
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

/// TSPLIB

TSP TSP::LoadHCP(std::string filename)
{
    ifstream fin(filename);
    Require(bool(fin), "LoadHCP incorrect file " + filename);
    string s;

    TSP tsp;
    while (fin>>s)
    {
        if (s == "DIMENSION")
        {
            fin>>s;
            if (s == ":") fin>>s;
            stringstream ss;
            ss<<s;
            int n;
            ss>>n;

            tsp.n = n;
            tsp.assign(n, vector<double>(n, 1.0));
            for (int i=0; i<n; i++) tsp[i][i] = 0.0;
        }
        else if (s == "EDGE_DATA_SECTION")
        {
            int x, y;
            while (true)
            {
                fin>>x;
                if (x == -1)
                    break;
                fin>>y;

                x--;
                y--;
                tsp[x][y] = tsp[y][x] = 0;
            }
        }
    }
    return tsp;
}

TSP TSP::LoadTSPLIB(std::string filename)
{
    ifstream fin(filename);
    Require(bool(fin), "LoadHCP incorrect file " + filename);
    string s;

    TSP tsp;
    while (fin>>s)
    {
        if (s == "DIMENSION")
        {
            fin>>s;
            if (s == ":") fin>>s;
            stringstream ss;
            ss<<s;
            int n;
            ss>>n;

            tsp.n = n;
            tsp.assign(n, vector<double>(n, 1.0));
            for (int i=0; i<n; i++) tsp[i][i] = 0.0;
        }
        else if (s == "EDGE_DATA_SECTION")
        {
            int x, y;
            while (true)
            {
                fin>>x;
                if (x == -1)
                    break;
                fin>>y;

                x--;
                y--;
                tsp[x][y] = tsp[y][x] = 0;
            }
        }
    }
    return tsp;
}

void AnalyzeCycle(TSP tsp, std::vector<int> v)
{
    int n = tsp.n;

    vector<pair<int, int> > e;
    vector<vector<int> > ev(n);
    for (int i=0; i<n; i++)
        for (int j=i+1; j<n; j++)
        {
            ev[i].pb(j);
            ev[j].pb(i);
            e.pb({i, j});
        }
    for (int i=0; i<n; i++)
        sort(ev[i].begin(), ev[i].end(), [&](int x, int y){ return tsp[i][x] < tsp[i][y]; });
    sort(e.begin(), e.end(), [&](pair<int, int> x, pair<int, int> y){ return tsp[x.fi][x.se] < tsp[y.fi][y.se]; });

    map<int, int> all;
    int mx = 0;
    for (int i=0; i<n; i++)
    {
        int x = v[i];
        int y = v[(i+1)%n];

        for (int _=0; _<2; _++)
        {
            int id = find(ev[x].begin(), ev[x].end(), y) - ev[x].begin();
            all[id+1]++;
            swap(x, y);
        }
        mx = max<int>(mx, find(e.begin(), e.end(), pair<int, int>{min(x, y), max(x, y)}) - e.begin() + 1);
    }

    for (auto pa : all)
        cout<<pa.fi<<" -> "<<pa.se<<" times\n";
    cout<<mx<<"\n";

}
