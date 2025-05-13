#include "common.h"

bool LKH::Teleport(int balance, int first_v, int last_v, int depth, int teleport_depth)
{
    int lim = segments.back();
    // we have two segments - [0, lim), [lim, n)

    vector<array<int, 5> > options; // [priority, a, u, v, b] => add (u, v), delete (a, u) and (v, b), u < lim <= v
    if (lim < n - lim)
    {
        for (int i=0; i<lim; i++)
        {
            int u = pr->At(i);
            for (int v : g[u])
            {
                int j = pr->Where(v);
                if (j >= lim)
                {
                    options.pb({0, pr->At((i+lim+1)%lim), u, v, pr->At((j-lim+n-lim+1)%(n-lim)+lim)});
                    options.pb({0, pr->At((i+lim+1)%lim), u, v, pr->At((j-lim+n-lim-1)%(n-lim)+lim)});
                    options.pb({0, pr->At((i+lim-1)%lim), u, v, pr->At((j-lim+n-lim+1)%(n-lim)+lim)});
                    options.pb({0, pr->At((i+lim-1)%lim), u, v, pr->At((j-lim+n-lim-1)%(n-lim)+lim)});
                }
            }
        }
    }
    else
    {
        for (int j=lim; j<n; j++)
        {
            int v = pr->At(j);
            for (int u : g[v])
            {
                int i = pr->Where(u);
                if (i < lim)
                {
                    options.pb({0, pr->At((i+lim+1)%lim), u, v, pr->At((j-lim+n-lim+1)%(n-lim)+lim)});
                    options.pb({0, pr->At((i+lim+1)%lim), u, v, pr->At((j-lim+n-lim-1)%(n-lim)+lim)});
                    options.pb({0, pr->At((i+lim-1)%lim), u, v, pr->At((j-lim+n-lim+1)%(n-lim)+lim)});
                    options.pb({0, pr->At((i+lim-1)%lim), u, v, pr->At((j-lim+n-lim-1)%(n-lim)+lim)});
                }
            }
        }
    }

    for (auto& [priority, a, u, v, b] : options)
        priority = DistW(a, u) + DistW(v, b) - DistW(u, v);
    sort(options.begin(), options.end());
    options.erase(unique(options.begin(), options.end()), options.end());

    reverse(options.begin(), options.end());
    segments = {n};
    for (auto [priority, a, u, v, b] : options)
    {
        int num1 = num(a, u);
        int num2 = num(u, v);
        int num3 = num(v, b);
        if (changed.Count(num1) || changed.Count(num2) || changed.Count(num3))
            continue;

        int pa = pr->Where(a);
        int pu = pr->Where(u);
        int pv = pr->Where(v);
        int pb = pr->Where(b);

        state.Flip(num1);
        state.Flip(num2);
        state.Flip(num3);
        if (states.Count(state.GetHash()))
        {
            state.Flip(num1);
            state.Flip(num2);
            state.Flip(num3);
            continue;
        }

        int cundo = 0;
        if (pa == 0 && pu == lim-1)
        {

        }
        else if (pa == lim-1 && pu == 0)
        {
            pr->Reverse(0, lim), cundo++;
        }
        else if (pa < pu)
        {
            pr->Reverse(0, pu), cundo++;
            pr->Reverse(pu, lim), cundo++;
        }
        else
        {
            pr->Reverse(0, pa), cundo++;
            pr->Reverse(pa, lim), cundo++;
            pr->Reverse(0, lim), cundo++;
        }

        if (pb == n-1 && pv == lim)
        {

        }
        else if (pb == lim && pv == n-1)
        {
            pr->Reverse(lim, n), cundo++;
        }
        else if (pb > pv)
        {
            pr->Reverse(pb, n), cundo++;
            pr->Reverse(lim, pb), cundo++;
        }
        else
        {
            pr->Reverse(lim, pb+1), cundo++;
            pr->Reverse(pb+1, n), cundo++;
            pr->Reverse(lim, n), cundo++;
        }

        changed.Insert(num1);
        changed.Insert(num2);
        changed.Insert(num3);

        if (Go(balance + priority, a, b, depth + 2, teleport_depth + 1))
            return true;

        changed.Undo();
        changed.Undo();
        changed.Undo();
        for (int i=0; i<cundo; i++) pr->Undo();
        state.Flip(num1);
        state.Flip(num2);
        state.Flip(num3);
    }
    segments = {n, lim};
    return false;
}

bool LKH::Go(int balance, int first_v, int last_v, int depth, int teleport_depth)
{
    int current_cycle_balance = balance - DistW(first_v, last_v);
    if (current_cycle_balance > 0 && Segments() == 1)
    {
        curlength -= current_cycle_balance;
        updates_since_last_output++;
        pr->Apply();
        return true;
    }
    if (current_cycle_balance > teleport_limit && Segments() == 2 && teleport_depth == 0)
    {
        if (Teleport(current_cycle_balance, first_v, last_v, depth, teleport_depth))
            return true;
    }

    int branch_limit = dynamic_branch_limit(depth);
    int segments_limit = dynamic_segment_limit(depth);

    states.Insert(state.GetHash());
    if (states.Overflow())
    {
        if (!overflown)
        {
            overflown = true;
            overflow_counter++;
        }
        return false;
    }
    if (depth > 512)
        return false;

    int branches = 0;
    int last_id = segments.back() - 1;
    for (int i : g[first_v])
        if (i != last_v)
        {
            if (branches >= branch_limit) break;
            int new_balance = balance - DistW(first_v, i);
            if (new_balance <= (use_dp ? (teleport_depth > 0 ? -3 : dp[i]) : 0))
                continue;
            int inum = num(first_v, i);
            if (changed.Count(inum))
                continue;
            int pos = pr->Where(i);
            if (pos == 1) continue; // can we be one vertex?

            state.Flip(inum);
            changed.Insert(inum);

            if (pos >= segments.back())
            {
                vector<int> saved_segments = segments;
                int id = 0;
                while (segments[id] > pos)
                    id++;
                int L = segments.back();
                int R = segments[id-1];
                pr->Reverse(L, R);
                for (int i=int(segments.size())-1; i>=id+1; i--)
                    segments[i] = (R-1)-(segments[i-1]-L)+1;
                reverse(segments.begin() + id, segments.end());
                segments.pop_back();
                pos = R-1-(pos-L);

                for (int delta : vector<int>{-1, 1})
                {
                    if (branches >= branch_limit) break;
                    if (pos + delta <= last_id) continue; // too left
                    if (pos + delta >= segments.back()) continue; // too right

                    int u = pr->At(pos + delta);
                    int unum = num(i, u);
                    if (changed.Count(unum))
                        continue;

                    if (delta == -1)
                    {
                        state.Flip(unum);
                        if (states.Count(state.GetHash()))
                        {
                            state.Flip(unum);
                        }
                        else
                        {
                            pr->Reverse(0, pos);
                            pr->Reverse(pos-1-last_id, segments.back());
                            changed.Insert(unum);

                            branches++;
                            if (Go(new_balance + DistW(u, i), u, last_v, depth+1, teleport_depth))
                                return true;

                            changed.Undo();
                            pr->Undo();
                            pr->Undo();

                            state.Flip(unum);
                        }
                    }
                    else
                    {
                        state.Flip(unum);
                        if (states.Count(state.GetHash()))
                        {
                            state.Flip(unum);
                        }
                        else
                        {
                            pr->Reverse(0, pos+1);
                            pr->Reverse(pos-last_id, segments.back());
                            pr->Reverse(0, segments.back() - (last_id+1));
                            changed.Insert(unum);

                            branches++;
                            if (Go(new_balance + DistW(u, i), u, last_v, depth+1, teleport_depth))
                                return true;

                            changed.Undo();
                            pr->Undo();
                            pr->Undo();
                            pr->Undo();

                            state.Flip(unum);
                        }
                    }
                }
                pr->Undo();
                segments = saved_segments;
            }
            else
            {
                if (branches < branch_limit && Segments() <= segments_limit)
                {
                    int u = pr->At(pos-1);
                    int unum = num(i, u);
                    if (!changed.Count(unum))
                    {
                        state.Flip(unum);
                        if (states.Count(state.GetHash()))
                        {
                            state.Flip(unum);
                        }
                        else
                        {
                            pr->Reverse(0, pos);
                            changed.Insert(unum);

                            branches++;
                            if (Go(new_balance + DistW(u, i), u, last_v, depth+1, teleport_depth))
                                return true;

                            changed.Undo();
                            pr->Undo();

                            state.Flip(unum);
                        }
                    }
                }
                if (branches < branch_limit && Segments() + 1 <= segments_limit && pos+1 < last_id)
                {
                    int u = pr->At(pos+1);
                    int unum = num(i, u);
                    if (!changed.Count(unum))
                    {
                        pr->Reverse(pos+1, last_id+1);
                        pr->Reverse(0, last_id+1);
                        changed.Insert(unum);
                        segments.push_back(segments.back() - (pos+1));

                        branches++;
                        state.Flip(unum);
                        if (Go(new_balance + DistW(u, i), u, last_v, depth+1, teleport_depth))
                            return true;
                        state.Flip(unum);

                        segments.pop_back();
                        changed.Undo();
                        pr->Undo();
                        pr->Undo();
                    }
                }
            }

            state.Flip(inum);
            changed.Undo();
        }
    return false;
}

void LKH::GoRandom()
{
    use_dp = false;
    int r = rng()%n;
    pr->Cut(r);
    int first_v = pr->At(0);
    int last_v = pr->At(n-1);
    InitializeGo();
    Go(DistW(first_v, last_v), first_v, last_v, 0, 0);
    CheckForOutput();
}
void LKH::GoRandomFor(double t)
{
    use_dp = false;
    auto t0 = Time();
    while (Time() - t0 < t)
        GoRandom();
}
void LKH::GoWhile()
{
    use_dp = false;
    vector<int> perm;
    for (int i=0; i<n; i++)
        perm.pb(i);
    shuffle(perm.begin(), perm.end(), rng);
    int id = 0;
    int no_upd = 0;
    while (no_upd < n)
    {
        int delta = (perm[(id+1)%n] - perm[id%n] + n)%n;
        id=(id+1)%n;
        pr->Cut(delta);
        int first_v = pr->At(0);
        int last_v = pr->At(n-1);
        InitializeGo();
        if (Go(DistW(first_v, last_v), first_v, last_v, 0, 0))
            no_upd = 0;
        else
            no_upd++;
        CheckForOutput();
    }
}
void LKH::GoRandomWhile(double t)
{
    use_dp = false;
    vector<int> perm;
    for (int i=0; i<n; i++)
        perm.pb(i);
    shuffle(perm.begin(), perm.end(), rng);
    int id = 0;
    int no_upd = 0;
    auto t0 = Time();
    while (no_upd < n && Time() - t0 < t)
    {
        int delta = (perm[(id+1)%n] - perm[id%n] + n)%n;
        id=(id+1)%n;
        pr->Cut(delta);
        int first_v = pr->At(0);
        int last_v = pr->At(n-1);
        InitializeGo();
        if (Go(DistW(first_v, last_v), first_v, last_v, 0, 0))
            no_upd = 0;
        else
            no_upd++;
        CheckForOutput();
    }
}
void LKH::GoWhileDP()
{
    use_dp = true;
    DpFullRecalc();
    vector<int> perm;
    for (int i=0; i<n; i++)
        perm.pb(i);
    shuffle(perm.begin(), perm.end(), rng);
    int id = 0;
    int no_upd = 0;
    while (no_upd < n)
    {
        int delta = (perm[(id+1)%n] - perm[id%n] + n)%n;
        id=(id+1)%n;
        pr->Cut(delta);
        int first_v = pr->At(0);
        int last_v = pr->At(n-1);
        InitializeGo();
        if (Go(DistW(first_v, last_v), first_v, last_v, 0, 0))
        {
            no_upd = 0;
            DpFullRecalc();
        }
        else
        {
            no_upd++;
        }
        CheckForOutput();
    }
}

void LKH::InitialOptimize(vector<int>& tour)
{
    T_Init = Time();
    curlength = Length(tour);

    W.clear();
    for (double w : W_optimal)
        W.pb(floor(w+0.5));

    struct Step
    {
        int s;
        int b;
        int tp;
        double time;
    };
    int tpinf = 1e9;
    auto ProcessStep = [&](Step s)
    {
        cout<<"s"<<s.s<<" b"<<s.b<<" t";
        if (s.tp == tpinf) cout<<"inf";
        else cout<<s.tp;
        cout<<"\n";

        teleport_limit = s.tp;
        dynamic_segment_limit = [s](int d){ return s.s; };
        dynamic_branch_limit = [s](int d){ return (d <= s.b) ? 100 : 0; };
        GoRandomWhile(s.time);
        OutputSkipStage();
    };

    vector<Step> V1 =
    {
        Step{ 1, 1, tpinf, 15.0 },
        Step{ 1, 3, tpinf,  5.0 },
    };
    pr = make_shared<PermReverseTreap>(tour);
    cout<<"init\n";
    OutputSkipStage();
    for (Step s : V1)
        ProcessStep(s);

    vector<Step> V2 =
    {
        Step{ 1,  5, tpinf, 15.0 },
        Step{ 1, 10, tpinf, 15.0 },
        Step{ 1, 15, tpinf, 15.0 },
        Step{ 1, 20, tpinf, 15.0 },

        Step{ 2,  5, tpinf, 10.0 },
        Step{ 2,  5,     0, 10.0 },

        Step{ 2, 10, tpinf, 3.0 },
        Step{ 2, 10,     0, 10.0 },

        Step{ 2, 15, tpinf, 3.0 },
        Step{ 2, 15,     0, 10.0 },

        Step{ 2, 20, tpinf, 3.0 },
        Step{ 2, 20,     0, 20.0 },
    };
    pr = make_shared<PermReverse>(Tour());
    for (Step s : V2)
        ProcessStep(s);

    tour = Tour();
}

void LKH::FinalOptimize(vector<int>& tour)
{
    T_Init = Time();
    curlength = Length(tour);
    W.clear();
    for (double w : W_optimal)
        W.pb(floor(w+0.5));
    reverse(tour.begin(), tour.end());

    pr = make_shared<PermReverse>(tour);

    for (int s = 2; ; s++)
    {
        cout<<"s"<<s<<"\n";
        teleport_limit = 0;
        dynamic_segment_limit = [s](int d){ return s; };
        dynamic_branch_limit = [](int d){ return 200; };
        GoWhileDP();
        OutputSkipStage();
    }
}
