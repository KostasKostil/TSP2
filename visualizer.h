#include "common.h"
#include <SFML/Graphics.hpp>

class Visualizer
{
public:
    Visualizer(int sz);
    void SetTSP(const TSP& tsp);
    void AddGraph(int r, int g, int b, std::vector<std::pair<int, int> > edges);
    void AddCycle(int r, int g, int b, std::vector<int> cycle);
    void SetGraph(int r, int g, int b, std::vector<std::pair<int, int> > edges);
    void SetCycle(int r, int g, int b, std::vector<int> cycle);

    void WaitUntilClosed();
private:
    void Infinity();
    void Update();
    void Render();

    int L;
    bool hold = false;
    sf::Vector2f mouse;

    double scale = 1.0;
    double dx = 0.0;
    double dy = 0.0;

    std::mutex upd_mutex;
    TSP tsp;
    std::map<std::array<int, 3>, std::set<std::pair<int, int> > > ed;

    sf::RenderWindow* window;
    std::future<void> closed;
};
