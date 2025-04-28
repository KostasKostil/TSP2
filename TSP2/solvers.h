#include "common.h"

/// solvers
std::vector<int> SolveNearestNeighbor(const TSP& tsp, int from = 0);
std::vector<int> SolveGreedyEdgePicker(const TSP& tsp, int final_dp_size = 1); // O(n^2 log n + m * 2^m)
std::vector<int> SolveGreedyAdderSinglePermutation(const TSP& tsp, std::vector<int> perm);
std::vector<int> SolveGreedyAdder(const TSP& tsp, int iterations = 1);
std::vector<int> SolveHamiltonian(const TSP& ham, const TSP& tsp);

/// optimizers
std::vector<int> OptimizeLocalDP(const TSP& tsp, std::vector<int> tour, int dp_size); // O( (n + steps * m) * m * 2^m )
void SolveLinKernighan_GA_2D(const TSP& tsp, double TL, bool visualize);

/// other
std::vector<int> FindHamiltinianCycle(const TSP& tsp);

/// estimators
double Estimate_MST_GraDesc(TSP tsp, int num_roots = -1);
double Estimate_MST2(const TSP& tsp, int k, int root = 0);

/// statscollector
struct Stats
{
    Stats();

    std::map<std::string, int> cnt;
    std::map<std::string, double> stats;

    double lastTime;
    std::map<std::string, double> times;

    void AddStat(std::string name, double value);
    void AddStat(double value, std::string name);
};
std::ostream& operator<<(std::ostream& out, const Stats& s);
void CollectStatsTestGraDesc(int n);
void CompareGA(int n);
void CollectStats(int n);
