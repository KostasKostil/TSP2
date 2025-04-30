#include "common.h"

static const uint ISMASK = (1<<29)-1;
static const uint PSMASK = (1<<13)-1;
static const ull  PSZERO = 0xA5B85C5E198ED849ULL;

ull Hash(uint h)
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

/// InsertionSet
InsertionSet::InsertionSet()
{
    sz = 0;
    version = 1;
    a = new cell[ISMASK + 1];
    for (uint i=0; i<=ISMASK; i++)
        a[i].version = 0;
}
InsertionSet::~InsertionSet()
{
    if (a != nullptr)
    {
        delete[] a;
        a = nullptr;
    }
}
void InsertionSet::Insert(ull num)
{
    uint id = num&ISMASK;
    while (a[id].version == version) id = (id+1)&ISMASK;
    a[id] = cell{ version, num };
    sz++;
}
bool InsertionSet::Count(ull num)
{
    uint id = num&ISMASK;
    while (true)
    {
        if (a[id].version != version) return false;
        if (a[id].value == num) return true;
        id = (id+1)&ISMASK;
    }
    return false;
}
void InsertionSet::Clear()
{
    version++;
    sz = 0;
    if (version==0)
    {
        for (uint i=0; i<=ISMASK; i++)
            a[i].version = 0;
        version = 1;
    }
}
int InsertionSet::Size()
{
    return sz;
}
bool InsertionSet::Overflow()
{
    return sz > (ISMASK >> 1);
}

/// PersistentSet
PersistentSet::PersistentSet()
{
    sz = 0;
    a = new ull[PSMASK + 1];
    for (uint i=0; i<=PSMASK; i++)
        a[i] = PSZERO;
    v = new uint[PSMASK + 1];
}
PersistentSet::~PersistentSet()
{
    if (a != nullptr)
    {
        delete[] a;
        a = nullptr;
    }
    if (v != nullptr)
    {
        delete[] v;
        v = nullptr;
    }
}
void PersistentSet::Insert(ull num)
{
    uint id = num&PSMASK;
    while (a[id] != PSZERO) id = (id+1)&PSMASK;
    a[id] = num;
    v[sz] = id;
    sz++;
    if (sz > (PSMASK >> 1)) exit(921);
}
void PersistentSet::Undo()
{
    sz--;
    a[v[sz]] = PSZERO;
}
bool PersistentSet::Count(ull num)
{
    uint id = num&PSMASK;
    while (true)
    {
        if (a[id] == PSZERO) return false;
        if (a[id] == num) return true;
        id = (id+1)&PSMASK;
    }
    return false;
}
void PersistentSet::Clear()
{
    for (uint i=0; i<sz; i++)
        a[v[i]] = PSZERO;
    sz = 0;
}
int PersistentSet::Size()
{
    return sz;
}

/// SetHash
void SetHash::Flip(uint num)
{
    res ^= Hash(num);
}
ull SetHash::GetHash()
{
    return res;
}

