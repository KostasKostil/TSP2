#include "common.h"
PermReverse2::PermReverse2(vector<int> p)
{
    inv = perm = p;
    n = perm.size();
    for (int i=0; i<n; i++)
        inv[perm[i]] = i;
    k = 1;
    b = 0;
}
void PermReverse2::Cut(int l)
{
    b += l;
    if (b >= n)
        b -= n;
}
void PermReverse2::Reverse(int l, int r)
{
    l = k*l+b;
    r = k*r+b;
    if (l < 0)
    {
        l += n;
        r += n;
    }
    if (l >= n)
    {
        l -= n;
        r -= n;
    }

    if (smart_changes.empty() && (r-l <= limit))
    { // naive change
        NaiveReverse(l, r);
        naive_changes.push_back({l, r});
    }
    else
    { // smart change
        smart_changes.push_back({l, r});
    }
}
void PermReverse2::Undo()
{
    if (smart_changes.empty())
    { // naive change
        NaiveReverse(naive_changes.back().fi, naive_changes.back().se);
        naive_changes.pop_back();
    }
    else
    { // smart change
        smart_changes.pop_back();
    }
}
void PermReverse2::Apply()
{
    for (auto [l, r] : smart_changes)
        NaiveReverse(l, r);
    naive_changes.clear();
    smart_changes.clear();
}
int PermReverse2::Where(int x)
{
    x = inv[x];
    for (auto [l, r] : smart_changes)
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
    x = (x-b)*k;
    if (x < 0) x+=n;
    return x;
}
int PermReverse2::At(int i)
{
    int x = k*i+b;
    if (x < 0) x += n;
    if (x >= n) x -= n;
    for (int _=int(smart_changes.size())-1; _>=0; _--)
    {
        auto [l, r] = smart_changes[_];
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
void PermReverse2::NaiveReverse(int l, int r)
{
    r--;
    int l0 = l, r0 = r;
    if (r0 >= n) r0 -= n;
    while (l < r)
    {
        swap(perm[l0], perm[r0]);
        inv[perm[l0]] = l0;
        inv[perm[r0]] = r0;
        l++;
        r--;
        l0++;
        if (l0 == n) l0 = 0;
        r0--;
        if (r0 == -1) r0 = n-1;
    }
}
