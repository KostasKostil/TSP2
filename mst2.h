#pragma once
#include <bits/stdc++.h>

struct Edge
{
    int u, v;
    double w;
};

struct MST2
{
public:
    int n, m;
    std::vector<Edge> ed;
    std::vector<std::vector<std::pair<int, int> > > g;

    // calculates first k MSTs
    // O(mlogm + mk + mn + k^2), last two term can be avoided if needed
    MST2(int n_, std::vector<Edge> edges, int k);

    std::vector<std::vector<bool> > in, out, T;
    // in, out - sets of edges for katoh-algo
    // T - trees themselves
    std::vector<std::vector<int> > Q;
    std::vector<int> Qopt;
    // Qopt[i] - edge f that gives best exchange
    // Q[i][k] - edge e that gives best exchange (e->f)
    std::vector<double> W;
private:
    void Initialization();
    bool Iteration(int k);
    bool Valid(int i, int f);
    double Cost(int e, int f);
    void SetQopt(int i);
    std::vector<std::pair<double, int> > CalcPrefixMaximas(int k, int v, int p);
    void UpdateQ(int k, int f);
};

struct MST2_Bruteforce
{
public:
    int n, m;
    std::vector<double> W;
    std::vector<Edge> ed;
    std::vector<std::vector<bool> > T;
    MST2_Bruteforce(int n_, std::vector<Edge> edges, int k);
};

void MST2_Test();
