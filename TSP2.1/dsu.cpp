#include "common.h"

DSU::DSU(){}
DSU::DSU(int n)
{
    p.resize(n);
    for (int i=0; i<n; i++)
        p[i] = i;
}
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
static mt19937 dsu_rng(47);
void DSU::Unite(int u, int v)
{
    u = Get(u);
    v = Get(v);
    if (dsu_rng()&1) swap(u, v);
    p[u] = v;
}
