class Visualizer
{
public:
    Visualizer();
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
    bool numbers = false;
    sf::Vector2f mouse;

    double scale = 1.0;
    double dx = 0.0;
    double dy = 0.0;

    std::mutex upd_mutex;
    TSP tsp;
    std::map<std::array<int, 3>, std::set<std::pair<int, int> > > ed;

    sf::RenderWindow* window;
    std::future<void> closed;
    sf::Font* font;
};

class PermReverse
{
public:
    PermReverse(std::vector<int> p);
    void Cut(int l); // shifts in such a way that l -> l+1 becomes 0 -> n-1
    void Reverse(int l, int r);
    void Undo();
    void Apply();
    int Where(int x);
    int At(int i);
private:
    int n, shift;
    std::vector<int> perm;
    std::vector<int> inv;
    std::vector<std::pair<int, int> > changes;
};
class PermReverse2
{
public:
    PermReverse2(vector<int> p);
    void Cut(int l); // shifts in such a way that l -> l+1 becomes 0 -> n-1
    void Reverse(int l, int r);
    void Undo();
    void Apply();
    int Where(int x);
    int At(int i);
private:
    static const int limit = 5; // maximal length of segment for naive processing
    int n;
    int k, b; // if we have current index x, it had index k*x+b before all operations
              // if we had index x,          it has index k*(x-b) after all operations
    std::vector<int> perm;
    std::vector<int> inv;
    std::vector<std::pair<int, int> > naive_changes;
    std::vector<std::pair<int, int> > smart_changes;
    void NaiveReverse(int l, int r);
};

typedef unsigned int uint;
typedef unsigned long long ull;
class UnorderedSet
{
public:
    UnorderedSet();
    ~UnorderedSet();
    UnorderedSet(const UnorderedSet&) = delete;
    UnorderedSet& operator=(const UnorderedSet&) = delete;
    void Insert(ull num);
    bool Count(ull num);
    void Clear();
    int Size();
    bool Overflow();
private:
    uint version, sz;
    uint* v;
    ull* a;
};
class SetHash
{
public:
    void Flip(unsigned int num);
    u128 GetHash();
private:
    u128 res = 0;
};
