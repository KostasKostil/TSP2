#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#define x first
#define y second
#define fi first
#define se second
#define pb push_back

using namespace std;

namespace Common
{
    inline bool Wflag;
    inline vector<int> W;
}

struct MSTEdge
{
    int u, v, w;
};
struct DSU
{
    DSU();
    DSU(int n);
    vector<int> p;
    int Get(int v);
    void Unite(int u, int v);
};

inline sf::Clock sfclock;
typedef vector<pair<int, int> > TSP;
TSP ReadCoords(const string& filename);
vector<int> ReadTour(const string& filename);
int Dist(const TSP& tsp, int i, int j);
int Length(const TSP& tsp, const vector<int>& tour);
double Time();

vector<vector<int> > ExtractEdges(const TSP& tsp, int C);
vector<vector<int> > FilterEdges(const vector<vector<int> >& g, int C);
void PrintMapStats(map<int, int>& mp);
void PrintMapStats(map<pair<int, int>, int>& mp);
void TourStats(const vector<vector<int> >& g, const vector<int>& tour);
void GlobalStats(const vector<vector<int> >& g, int LIM);

vector<MSTEdge> GetVectorOfMSTEdges(const TSP& tsp, vector<vector<int> > g);
vector<double> MST_Gradesc(int n, vector<MSTEdge> ed);

struct AlphaEdge
{
    int v;
    int w0;
    double W;
    double alpha;
};
void SaveAlphaGraph(vector<vector<AlphaEdge> > g, string file);
vector<vector<AlphaEdge> > LoadAlphaGraph(string file);
vector<vector<AlphaEdge> > AlphaNearness(const TSP& tsp, vector<vector<int> > g, vector<double> W);

vector<int> InsertionTour(const TSP& tsp, vector<vector<int> > g);

inline mt19937 rng(time(0));
void FeasibleLKH(const TSP& _tsp, const vector<vector<int> >& _g, const vector<vector<AlphaEdge> >& G, vector<int>& tour);
void SequentialLKH(const TSP& _tsp, const vector<vector<int> >& _g, const vector<vector<AlphaEdge> >& G, vector<int>& tour);

vector<int> Wevaluate(const TSP& tsp, const vector<vector<int> >& g, const vector<int>& tour);

#include "helper.h"

class LKH
{
public:
    struct Point
    {
        int x;
        int y;
    };

    int n;
    int curlength;
    vector<Point> tsp;
    vector<vector<int> > g;
    vector<double> W_optimal;

    LKH(vector<pair<int, int> > tsp_, vector<vector<int> > g_, vector<double> W_);
    int Dist(int i, int j);
    int DistW(int i, int j);
    int L_Output;
    double T_Init;
    double T_Output;
    int updates_since_last_output;
    int tries_since_last_output;
    void CheckForOutput();
    void OutputSkipStage();
    void InitializeGo();

    vector<int> W;
    SetHash state;
    InsertionSet states;
    PersistentSet changed;
    shared_ptr<IPermReverse> pr;
    vector<int> segments;
    int Segments();

    int teleport_limit;
    function<int(int)> dynamic_branch_limit, dynamic_segment_limit;

    int num(int u, int v);

    bool Go(int balance, int first_v, int last_v, int depth, int teleport_depth);
    bool Teleport(int balance, int first_v, int last_v, int depth, int teleport_depth);

    void GoRandom();
    void GoRandomFor(double t);
    void GoWhile();

    int Length(vector<int> tour);
    vector<int> Tour();
    void Main(vector<int>& tour);
};

inline int LKH::Dist(int i, int j)
{
    double dx = tsp[i].x - tsp[j].x;
    double dy = tsp[i].y - tsp[j].y;
    return int(std::sqrt(dx*dx + dy*dy) + 0.5);
}
inline int LKH::DistW(int i, int j)
{
    return Dist(i, j) + W[i] + W[j];
}
inline int LKH::num(int u, int v)
{
    return (u > v) ? (n*u + v) : (n*v + u);
}
