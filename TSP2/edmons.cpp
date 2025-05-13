#include "common.h"

using namespace std;

vector<pair<int,int>> minWeightPerfectMatching(const vector<vector<double>>& w)\
{
    int n = w.size();
    // 1) жадное решение для upper bound
    vector<tuple<double,int,int>> edges;
    edges.reserve(n*(n-1)/2);
    for(int i = 0; i < n; ++i)
        for(int j = i+1; j < n; ++j)
            edges.emplace_back(w[i][j], i, j);
    std::sort(edges.begin(), edges.end(),
              [](auto &a, auto &b){ return get<0>(a) < get<0>(b); });

    vector<bool> used0(n,false);
    double best_w = 0;
    vector<pair<int,int>> best_match;
    for(auto &e : edges){
        int i = get<1>(e), j = get<2>(e);
        if(!used0[i] && !used0[j]){
            used0[i] = used0[j] = true;
            best_w += w[i][j];
            best_match.emplace_back(i,j);
        }
    }

    // для ветвей и границ
    vector<bool> used(n,false);
    vector<pair<int,int>> curr;
    int total_unmatched = n;

    std::function<void(double,int)> dfs = [&](double cur_w, int unmatched){
        // отсечка по верхней границе
        if(cur_w >= best_w) return;

        // нижняя граница: для каждого неиспользованного берём min-ребро
        double lb_sum = 0;
        for(int i = 0; i < n; ++i){
            if(used[i]) continue;
            double mn = std::numeric_limits<double>::infinity();
            for(int j = 0; j < n; ++j){
                if(i==j || used[j]) continue;
                mn = std::min(mn, w[i][j]);
            }
            lb_sum += mn;
        }
        double lb = lb_sum * 0.5;
        if(cur_w + lb >= best_w) return;

        if(unmatched == 0){
            best_w = cur_w;
            best_match = curr;
            return;
        }

        // выбираем первую неиспользованную вершину i
        int i = 0;
        while(used[i]) ++i;
        used[i] = true;
        // ветвим по всем j > i
        for(int j = i+1; j < n; ++j){
            if(used[j]) continue;
            used[j] = true;
            curr.emplace_back(i,j);

            dfs(cur_w + w[i][j], unmatched - 2);

            curr.pop_back();
            used[j] = false;
        }
        used[i] = false;
    };

    dfs(0.0, total_unmatched);
    return best_match;
}
