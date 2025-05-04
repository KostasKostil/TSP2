#include "common.h"

using namespace std;

const bool dpnum = true;

double radius = 1.0;
typedef pair<int, int> Point;

Visualizer::Visualizer()
{
    L = 1400;
    closed = async(Visualizer::Infinity, this);
}

void Visualizer::SetTSP(const TSP& tsp_)
{
    lock_guard<mutex> lg(upd_mutex);
    tsp = tsp_;
    for (auto& pa : tsp) pa.se = -pa.se;
}

void Visualizer::SetCycle(int r, int g, int b, std::vector<int> cycle)
{
    vector<pair<int, int> > e;
    for (int i=0; i<int(cycle.size()); i++)
        e.pb({cycle[i], cycle[(i+1)%cycle.size()]});
    SetGraph(r, g, b, e);
}

void Visualizer::SetGraph(int r, int g, int b, std::vector<std::pair<int, int> > edges)
{
    lock_guard<mutex> lg(upd_mutex);
    ed[{r, g, b}].clear();
    for (auto [u, v] : edges)
        ed[{r, g, b}].insert({min(u, v), max(u, v)});
}

void Visualizer::AddCycle(int r, int g, int b, std::vector<int> cycle)
{
    vector<pair<int, int> > e;
    for (int i=0; i<int(cycle.size()); i++)
        e.pb({cycle[i], cycle[(i+1)%cycle.size()]});
    SetGraph(r, g, b, e);
}

void Visualizer::AddGraph(int r, int g, int b, std::vector<std::pair<int, int> > edges)
{
    lock_guard<mutex> lg(upd_mutex);
    for (auto [u, v] : edges)
        ed[{r, g, b}].insert({min(u, v), max(u, v)});
}

void Visualizer::WaitUntilClosed()
{
    closed.get();
}

void Visualizer::Infinity()
{
    font = new sf::Font();
    if (!font->loadFromFile("font.otf")) exit(6);
    sf::VideoMode vm = sf::VideoMode(L, L);
    window = new sf::RenderWindow(vm, "TSP", sf::Style::Close);
    while (window->isOpen())
    {
        Update();
        Render();
        std::chrono::milliseconds timespan(1000 / 60);
        std::this_thread::sleep_for(timespan);
    }
}

#undef x
#undef y
void Visualizer::Update()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            hold = true;
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            hold = false;
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M)
            numbers ^= true;
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::N)
            vnumbers ^= true;

        if (event.type == sf::Event::MouseMoved)
        {
            sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
            if (hold)
            {
                sf::Vector2f d = mouse - mousePos;
                dx -= d.x * scale / L;
                dy -= d.y * scale / L;
            }
            mouse = mousePos;
        }

        if (event.type == sf::Event::MouseWheelScrolled)
        {
            sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

            float delta = event.mouseWheelScroll.delta;
            double mul = 1.25;
            if (delta < 0) mul = 1.0/mul;

            double mx = mousePos.x * scale / L;
            double my = mousePos.y * scale / L;
            scale *= mul;

            dx -= mx;
            dy -= my;

            dx *= mul * mul;
            dy *= mul * mul;

            mx *= mul;
            my *= mul;

            dx += mx;
            dy += my;
        }
    }
}
#define x first
#define y second

void Visualizer::Render()
{
    lock_guard<mutex> lg(upd_mutex);

    window->clear(sf::Color(0, 0, 0));

    if (tsp.size() > 0)
    {
        Point mn = tsp[0];
        Point mx = tsp[0];
        for (Point q : tsp)
        {
            mn.x = min(mn.x, q.x);
            mn.y = min(mn.y, q.y);
            mx.x = max(mx.x, q.x);
            mx.y = max(mx.y, q.y);
        }
        double DX = mx.x - mn.x;
        double DY = mx.y - mn.y;
        DX = 1 + DX*1.1;
        DY = 1 + DY*1.1;
        DX = DY = max(DX, DY);
        double SX = (mn.x + mx.x)/2 - DX/2;
        double SY = (mn.y + mx.y)/2 - DY/2;

        auto fa = [&](double x, double d){ return x*scale+d/scale; };
        auto fx = [&](double x){ return fa((x-SX)/DX, dx) * L; };
        auto fy = [&](double y){ return fa((y-SY)/DY, dy) * L; };

        int id = -1;
        for (Point q : tsp)
        {
            id++;
            double px = fx(q.x)-radius;
            double py = fy(q.y)-radius;
            if (px < -radius || py < -radius || px > L || py > L)
                continue;

            sf::CircleShape c;
            c.setRadius(radius);
            c.setFillColor(sf::Color::White);
            c.setPosition(px, py);

            window->draw(c);

            if (vnumbers && dpnum)
            {
                stringstream ss;
                ss<<Common::DP[id];
                string s = ss.str();

                sf::Text text;
                text.setFont(*font);
                text.setCharacterSize(20);
                text.setString(s);
                text.setFillColor(sf::Color(255, 128, 0));
                text.setOutlineColor(sf::Color::Black);
                text.setOutlineThickness(0);

                sf::FloatRect bounds = text.getLocalBounds();
                text.setOrigin(bounds.width / 2, bounds.height / 2);

                text.setPosition(sf::Vector2f(px, py));
                window->draw(text);
            }
        }

        map<pair<int, int>, array<int, 4> > mp;
        for (auto [col, e] : ed)
            for (auto [u, v] : e)
            {
                if (!mp.count({u, v})) mp[{u, v}] = {0, 0, 0, 0};
                mp[{u, v}][0] += col[0];
                mp[{u, v}][1] += col[1];
                mp[{u, v}][2] += col[2];
                mp[{u, v}][3] += 1;
            }
        for (auto [pa, col] : mp)
        {
            Point p = tsp[pa.fi];
            Point q = tsp[pa.se];

            double px = fx(p.x);
            double py = fy(p.y);
            double qx = fx(q.x);
            double qy = fy(q.y);
            #define LIM 0.1
            if ((px < L*(-LIM) && qx < L*(-LIM))
             || (py < L*(-LIM) && qy < L*(-LIM))
             || (px > L*(1+LIM)&& qx > L*(1+LIM))
             || (py > L*(1+LIM)&& qy > L*(1+LIM)))
                continue;

            int r = col[3];
            sf::Color c = sf::Color(col[0]/r, col[1]/r, col[2]/r);
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f( px, py ), c),
                sf::Vertex(sf::Vector2f( qx, qy ), c)
            };
            window->draw(line, 2, sf::Lines);

            if (numbers)
            {
                stringstream ss;
                ss<<Dist(tsp, pa.fi, pa.se);
                string s = ss.str();

                sf::Text text;
                text.setFont(*font);
                text.setCharacterSize(20);
                text.setString(s);
                text.setFillColor(sf::Color::White);
                text.setOutlineColor(sf::Color::Black);
                text.setOutlineThickness(0);

                sf::FloatRect bounds = text.getLocalBounds();
                text.setOrigin(bounds.width / 2, bounds.height / 2);

                text.setPosition(sf::Vector2f((px+qx)/2, (py+qy)/2));
                window->draw(text);
            }
        }
    }

    window->display();
}
