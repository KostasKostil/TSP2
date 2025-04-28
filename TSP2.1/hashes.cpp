#include "common.h"

static const uint USMASK = (1<<27)-1;

UnorderedSet::UnorderedSet()
{
    s = 0;
    a = new u128[USMASK + 1];
    for (uint i=0; i<=USMASK; i++)
        a[i] = USZERO;
    v = new uint[USMASK + 1];
}
UnorderedSet::~UnorderedSet()
{
    if (a != nullptr)
    {
        delete[] a;
        a = nullptr;
    }
}
void UnorderedSet::Insert(u128 num)
{
    uint id = num&USMASK;
    while (a[id] != USZERO) id = (id+1)&USMASK;
    a[id] = num;
    v[s] = id;
    s++;
}
bool UnorderedSet::Count(u128 num)
{
    uint id = num&USMASK;
    while (true)
    {
        if (a[id] == USZERO) return false;
        if (a[id] == num) return true;
        id = (id+1)&USMASK;
    }
    return false;
}
void UnorderedSet::Clear()
{
    for (uint i=0; i<s; i++)
        a[v[i]] = USZERO;
    s = 0;
}
int UnorderedSet::Size()
{
    return s;
}
bool UnorderedSet::Overflow()
{
    return s > (USMASK >> 2);
}

void SetHash::Flip(unsigned int num)
{
    u128 h = num;
    h = h + 0xbf58476d1ce4e5b9ULL;
    h = (h ^ (h << 64)) * 0x9e3779b97f4a7c15ULL;
    h ^= h >> 64;
    res ^= h;
}
u128 SetHash::GetHash()
{
    return res;
}

