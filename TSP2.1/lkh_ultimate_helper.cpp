#include "common.h"

LKH::LKH(vector<pair<int, int> > tsp_, vector<vector<int> > g_, vector<double> W_)
{
    n = tsp_.size();
    for (int i=0; i<n; i++)
        tsp.pb(Point{tsp_[i].x, tsp_[i].y});
    g = g_;
    W_optimal = W_;

    L_Output = 1e9;
    T_Output = -1e9;
    updates_since_last_output = 0;
    tries_since_last_output = 0;
    teleport_limit = 0;
}


int LKH::Length(vector<int> tour)
{
    int s = 0;
    for (int i=0; i<n; i++)
        s += Dist(tour[i], tour[(i+1)%n]);
    return s;
}
int LKH::Segments()
{
    return segments.size();
}
vector<int> LKH::Tour()
{
    vector<int> t;
    for (int i=0; i<n; i++)
        t.pb(pr->At(i));
    return t;
}
void LKH::CheckForOutput()
{
    double T = Time();
    if (
//        curlength != L_Output
//        &&
        T > T_Output + 1.0
        )
    {
        cout<<"current length: "<<curlength<<", upd="<<updates_since_last_output<<"/"<<tries_since_last_output<<", t="<<T-T_Init<<".\n";
        updates_since_last_output = 0;
        tries_since_last_output = 0;

        auto t = Tour();
        assert(Length(t) == curlength);

        ofstream fout("tours/lkh.txt");
        for (int i : t)
            fout<<i<<"\n";

        T_Output = Time();
        L_Output = curlength;
    }
}
void LKH::OutputSkipStage()
{
    T_Output = -1e9;
    L_Output = 1e9;
    CheckForOutput();
}
void LKH::InitializeGo()
{
    tries_since_last_output++;
    segments = {n};
    state = SetHash();
    changed.Clear();
    states.Clear();
}
