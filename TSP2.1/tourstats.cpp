#include "common.h"

void PrintMapStats(map<int, int>& mp)
{
    for (auto [x, y] : mp)
        cout<<x<<": "<<y<<"\n";
}
void PrintMapStats(map<pair<int, int>, int>& mp)
{
    set<int> stx, sty;
    for (auto [pa, c] : mp)
    {
        stx.insert(pa.fi);
        sty.insert(pa.se);
    }
    cout<<setw(5)<<" "<<" ";
    for (int x : stx)
        cout<<setw(5)<<x<<" ";
    cout<<"\n";
    for (int y : sty)
    {
        cout<<setw(5)<<y<<" ";
        for (int x : stx)
            cout<<setw(5)<<mp[{x, y}]<<" ";
        cout<<"\n";
    }
}

void TourStats(const vector<vector<int> >& g, const vector<int>& tour)
{
    map<pair<int, int>, int> mp;
    int n = tour.size();
    for (int i=0; i<n; i++)
    {
        int u = tour[i];
        int v = tour[(i+1)%n];
        int iu = -1;
        int iv = -1;
        for (int j=0; j<int(g[u].size()); j++)
            if (g[u][j] == v)
            {
                iu = j+1;
                break;
            }
        for (int j=0; j<int(g[v].size()); j++)
            if (g[v][j] == u)
            {
                iv = j+1;
                break;
            }
        mp[{min(iu, iv), max(iu, iv)}]++;
    }
    PrintMapStats(mp);
}

void GlobalStats(const vector<vector<int> >& g, int LIM)
{
    map<pair<int, int>, int> mp;
    int n = g.size();

    map<pair<int, int>, int> ed1;
    map<pair<int, int>, int> ed2;
    for (int i=0; i<n; i++)
    {
        int id = 0;
        for (int j : g[i])
        {
            pair<int, int> pa = {min(i, j), max(i, j)};
            if (ed1.count(pa))
                ed2[pa] = ++id;
            else
                ed1[pa] = ++id;
            if (id > LIM)
                break;
        }
    }
    for (auto [pa, c1] : ed1)
        if (ed2.count(pa))
        {
            int c2 = ed2[pa];
            if (c1 <= LIM && c2 <= LIM)
                mp[{min(c1, c2), max(c1, c2)}]++;
        }
    PrintMapStats(mp);
}
