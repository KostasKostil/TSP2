#pragma once
#include <bits/stdc++.h>

constexpr double Pi = acos(-1);

void Assert(bool flag, std::string message);
void Assert(bool flag, std::function<void(std::ostream&)> message);

struct Point
{
    double x, y;
};

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
};

enum Metric
{
    Euclidean,
    Manhattan,
    Chebyshev,
};
double Distance(Point A, Point B);
double Distance(Point A, Point B, Metric metric);
TSP LoadPlaneTSP(std::istream& in, Metric metric);
TSP LoadPlaneTSP(std::string filename, Metric metric);

