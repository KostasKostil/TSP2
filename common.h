#pragma once
#include <bits/stdc++.h>
#define fi first
#define se second
#define pb push_back

const double INF = 1e12;
const double EPS = 1e-12;

void Assert(bool flag, std::string message);
void Assert(bool flag, std::function<void(std::ostream&)> message);

struct Point
{
    double x, y;
};
enum Metric
{
    Euclidean,
    Manhattan,
    Chebyshev,
};
double Distance(Point A, Point B);
double Distance(Point A, Point B, Metric metric);
struct TSP : std::vector<std::vector<double> >
{
    std::vector<Point> coords;

    TSP();
    TSP(std::istream& in);
    TSP(std::string filename);
    void Save(std::ostream& out);
    void Save(std::string filename);

    int n;
    double& g(int i, int j);
    const double& g(int i, int j) const;
    double Length(const std::vector<int>& tour) const;

    static TSP LoadPlaneTSP(std::istream& in, Metric metric = Metric::Euclidean);
    static TSP LoadPlaneTSP(std::string filename, Metric metric = Metric::Euclidean);
};

struct DSU
{
    DSU();
    std::vector<int> p;
    int Get(int v);
    void Unite(int u, int v);
};
