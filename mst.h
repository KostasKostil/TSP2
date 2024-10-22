#pragma once
#include "common.h"

struct MST
{
    double weight;
    std::vector<int> deg;
    std::vector<std::pair<int, int> > edges;
    std::vector<std::vector<int> > g;

    // either simple MST (root == -1)
    // or connected graph with minimum total weight such that:
    // - root has degree 2
    // - if you remove root it remains connected
    MST(const TSP& tsp, int root = -1);
};

TSP MST_GradientDescentTransform(TSP tsp, int root = 0);
