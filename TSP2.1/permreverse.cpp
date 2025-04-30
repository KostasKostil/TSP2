#include "common.h"

using namespace std;

PermReverse::PermReverse(std::vector<int> p)
{
    n = p.size();
    perm = new int[n];
    inv = new int[n];
    for (int i=0; i<n; i++)
        perm[i] = p[i];
    for (int i=0; i<n; i++)
        inv[perm[i]] = i;
    m = 0;
    changes = new pair<int, int>[n];
    shift = 0;
}
PermReverse::~PermReverse()
{
    delete[] perm;
    delete[] inv;
    delete[] changes;
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
    changes[m] = {l, r};
    m++;
}
void PermReverse::Undo()
{
    m--;
}
void PermReverse::Apply()
{
    for (int chid = 0; chid < m; chid++)
    {
        auto [l, r] = changes[chid];
        if (r <= n)
        {
            int l0 = l;
            int r0 = r-1;
            for (int i=0; i<(r-l)/2; i++)
            {
                swap(perm[l0], perm[r0]);
                inv[perm[l0]] = l0;
                inv[perm[r0]] = r0;
                l0++;
                r0--;
            }
        }
        else
        {
            int l0 = l;
            int r0 = (r-1)-n;
            for (int i=0; i<(r-l)/2; i++)
            {
                if (l0 == n) l0 = 0;
                if (r0 == -1) r0 = n-1;
                swap(perm[l0], perm[r0]);
                inv[perm[l0]] = l0;
                inv[perm[r0]] = r0;
                l0++;
                r0--;
            }
        }
    }
    m = 0;
}
int PermReverse::Where(int x)
{
    x = inv[x];
    for (int chid = 0; chid < m; chid++)
    {
        auto [l, r] = changes[chid];
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
    for (int chid = m-1; chid >= 0; chid--)
    {
        auto [l, r] = changes[chid];
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
