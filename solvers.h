#include "common.h"

std::vector<int> SolveNearestNeighbor(const TSP& tsp, int from = 0);
std::vector<int> SolveGreedyEdgePicker(const TSP& tsp, int final_dp_size = 1); // O(n^2 log n + m * 2^m)

std::vector<int> OptimizeLocalDP(const TSP& tsp, std::vector<int> tour, int dp_size); // O( (n + steps * m) * m * 2^m )
