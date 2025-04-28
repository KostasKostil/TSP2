#include "common.h"

TSP ReadCoords(const string& filename)
{
    ifstream file(filename);
    string line;
    bool reading_nodes = false;
    vector<pair<int, int>> coords;

    while (getline(file, line))
    {
        if (line == "NODE_COORD_SECTION")
        {
            reading_nodes = true;
            continue;
        }
        if (line == "EOF") break;
        if (!reading_nodes) continue;

        stringstream ss(line);
        int index, x, y;
        ss >> index >> x >> y;
        coords.emplace_back(x, y);
    }

    return coords;
}

vector<int> ReadTour(const string& filename)
{
    ifstream file(filename);
    string line;
    bool reading_tour = false;
    vector<int> tour;

    while (getline(file, line))
    {
        if (line == "TOUR_SECTION")
        {
            reading_tour = true;
            continue;
        }
        if (line == "-1" || line == "EOF") break;
        if (!reading_tour) continue;

        stringstream ss(line);
        int city;
        while (ss >> city) {
            tour.push_back(city - 1);
        }
    }

    return tour;
}

int Dist(const TSP& tsp, int i, int j)
{
    double dx = tsp[i].first - tsp[j].first;
    double dy = tsp[i].second - tsp[j].second;
    if (Common::Wflag)
        return int(sqrt(dx * dx + dy * dy) + 0.5) + Common::W[i] + Common::W[j];
    return int(sqrt(dx * dx + dy * dy) + 0.5);
}

int Length(const TSP& tsp, const vector<int>& tour)
{
    bool cwf = Common::Wflag;
    Common::Wflag = false;

    int res = 0;
    for (int i=0; i<int(tsp.size()); i++)
        res += Dist(tsp, tour[i], tour[(i+1)%int(tsp.size())]);

    Common::Wflag = cwf;
    return res;
}

