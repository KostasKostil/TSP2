#include "mst.h"

using namespace std;

static int n;
static TSP tsp;
static TSP res;
static MST mst;

// mst-values are minimums over all overlapping edges
// nonmst-values ara maximums on the path

double dfs1(int L, int v, int p) // mst-values: includes all overlapping edges coming out of L (downwards)
{
    double mx = tsp[L][v];
    for (int i : mst.g[v])
        if (i != p)
            mx = max(mx, dfs1(L, i, v));

    res[p][v] = res[v][p] = max(res[p][v], mx);
    return mx;
}

int III=0;
void dfs2(int L, int v, int p, double current_max) // nonmst-values: calculates answer for all edges coming out from L (downwards)
{
    if (L != p)
        res[L][v] = res[v][L] = current_max;
    for (int i : mst.g[v])
        if (i != p)
            dfs2(L, i, v, max(current_max, tsp[i][v]));
}

void dfs0(int v, int p = -1)
{
    for (int i : mst.g[v])
        if (i != p)
        {
            dfs2(v, i, v, tsp[i][v]);
            dfs2(i, v, i, tsp[i][v]);
//            dfs2(i, p, v, max(tsp[i][v], tsp[p][i]));
            if (p != -1)
            {
//                dfs1(p, i, v);
            }
            dfs0(i, v);
        }
}

TSP ComputeMST_Matrix(TSP tsp_, int root)
{
    tsp = tsp_;
    n = tsp.n;
    mst = MST(tsp, root);

    res = tsp;
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
            res[i][j] = (i == j) ? 0 : INFDP;

    vector<int> gr = mst.g[root];
    for (int i : gr)
    {
        for (int j=0; j<int(mst.g[i].size()); j++)
            if (mst.g[i][j] == root)
            {
                swap(mst.g[i][j], mst.g[i].back());
                mst.g[i].pop_back();
                break;
            }
    }
    mst.g[root].clear();

    int v0 = 0;
    if (v0 == root) v0++;
    dfs0(v0);

    double mx2 = max(tsp[root][gr[0]], tsp[root][gr[1]]);
    for (int i=0; i<n; i++)
        if (i!=root)
            res[root][i] = res[i][root] = mx2;
    return res;
}
