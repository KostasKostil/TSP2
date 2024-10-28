#include "smart_set.h"

SmartSet::SmartSet(int n) : n(n), mx(-1), a(n) {}
void SmartSet::Assign(int id, double x)
{
    a[id] = x;
    if (mx != -1 && x > a[mx])
        mx = id;
    else if (mx == id)
        mx = -1;
}
int SmartSet::MaxId()
{
    if (mx == -1)
    {
        mx = 0;
        for (int i=1; i<n; i++)
            if (a[i] > a[mx])
                mx = i;
    }
    return mx;
}
