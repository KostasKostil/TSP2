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

inline mt19937 rng(time(0));
void FeasibleLKH(const TSP& _tsp, const vector<vector<int> >& _g, const vector<vector<AlphaEdge> >& G, vector<int>& tour);
void SequentialLKH(const TSP& _tsp, const vector<vector<int> >& _g, const vector<vector<AlphaEdge> >& G, vector<int>& tour);

vector<int> Wevaluate(const TSP& tsp, const vector<vector<int> >& g, const vector<int>& tour);

#include "helper.h"

class LKH
{
    struct Point
    {
        int x;
        int y;
    };

    int n;
    vector<Point> tsp;
    vector<double> W_optimal;
};
