#include "common.h"

using namespace std;

PermReverse::PermReverse(std::vector<int> p)
{
    inv = perm = p;
    n = perm.size();
    perm.reserve(2*n);
    for (int i=0; i<n; i++)
        inv[perm[i]] = i;
    shift = 0;
}
void PermReverse::Cut(int l)
{
    shift += l;
    if (shift >= n)
        shift -= n;
}
void PermReverse::Reverse(int l, int r)
{
    l += shift;
    r += shift;
    if (l >= n)
    {
        l -= n;
        r -= n;
    }
    changes.push_back({l, r});
}
void PermReverse::Undo()
{
    changes.pop_back();
}
void PermReverse::Apply()
{
    for (auto [l, r] : changes)
    {
        if (r <= n)
            reverse(perm.begin() + l, perm.begin() + r);
        else
        {
            r -= n;
            for (int i=0; i<r; i++)
                perm.push_back(perm[i]);
            reverse(perm.begin() + l, perm.begin() + r + n);
            for (int i=r-1; i>=0; i--)
            {
                perm[i] = perm.back();
                perm.pop_back();
            }
        }
    }
    for (int i=0; i<n; i++)
        inv[perm[i]] = i;
    changes.clear();
}
int PermReverse::Where(int x)
{
    x = inv[x];
    for (auto [l, r] : changes)
    {
        if (l<=x && x<r)
        {
            x = r-1-(x-l);
            if (x >= n) x -= n;
        }
        else if (l-n<=x && x<r-n)
        {
            x+=n;
            x = r-1-(x-l);
            if (x >= n) x -= n;
        }
    }
    x -= shift;
    if (x < 0) x+=n;
    return x;
}
int PermReverse::At(int i)
{
    int x = i+shift;
    if (x >= n) x -= n;
    for (int _=int(changes.size())-1; _>=0; _--)
    {
        auto [l, r] = changes[_];
        if (l<=x && x<r)
        {
            x = r-1-(x-l);
            if (x >= n) x -= n;
        }
        else if (l-n<=x && x<r-n)
        {
            x+=n;
            x = r-1-(x-l);
            if (x >= n) x -= n;
        }
    }
    return perm[x];
}
