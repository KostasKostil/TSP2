#include <bits/stdc++.h>

struct SmartSet
{
    int n, mx;
    std::vector<double> a;
    SmartSet(int n);
    void Assign(int id, double x);
    int MaxId();
};
