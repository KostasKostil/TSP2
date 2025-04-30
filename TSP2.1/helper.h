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

class IPermReverse
{
public:
    virtual ~IPermReverse() = default;
    virtual void Cut(int l) = 0;
    virtual void Reverse(int l, int r) = 0;
    virtual void Undo() = 0;
    virtual void Apply() = 0;
    virtual int Where(int x) = 0;
    virtual int At(int i) = 0;
};
class PermReverse : public IPermReverse
{
public:
    PermReverse(std::vector<int> p);
    ~PermReverse();
    void Cut(int l) override; // shifts in such a way that l -> l+1 becomes 0 -> n-1
    void Reverse(int l, int r) override;
    void Undo() override;
    void Apply() override;
    int Where(int x) override;
    int At(int i) override;
private:
    int n, shift;
    int* perm;
    int* inv;
    int m;
    std::pair<int, int>* changes;
};
class PermReverse2 : public IPermReverse
{
public:
    PermReverse2(std::vector<int> p);
    void Cut(int l) override; // shifts in such a way that l -> l+1 becomes 0 -> n-1
    void Reverse(int l, int r) override;
    void Undo() override;
    void Apply() override;
    int Where(int x) override;
    int At(int i) override;
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
ull Hash(uint h);
class InsertionSet
{
public:
    InsertionSet();
    ~InsertionSet();
    InsertionSet(const InsertionSet&) = delete;
    InsertionSet& operator=(const InsertionSet&) = delete;
    void Insert(ull num);
    bool Count(ull num);
    void Clear();
    int Size();
    bool Overflow();
private:
    uint version, sz;
    struct cell
    {
        uint version;
        ull value;
    };
    cell* a;
};
class PersistentSet
{
public:
    PersistentSet();
    ~PersistentSet();
    PersistentSet(const PersistentSet&) = delete;
    PersistentSet& operator=(const PersistentSet&) = delete;
    void Insert(ull num);
    void Undo();
    bool Count(ull num);
    void Clear();
    int Size();
private:
    uint sz;
    uint* v;
    ull* a;
};
class SetHash
{
public:
    void Flip(uint num);
    ull GetHash();
private:
    ull res = 0;
};

class PermReverseTreap : public IPermReverse
{
public:
    PermReverseTreap(const std::vector<int>& V);
    void Cut(int l) override;
    void Reverse(int l, int r) override;
    void Undo() override;
    void Apply() override;
    int Where(int x) override;
    int At(int i) override;

private:
    struct Node
    {
        int val;
        int prior;
        int size;
        bool rev;
        Node* l;
        Node* r;
        Node* parent;
        Node(int v);
    };

    int n;
    Node*                          root;
    std::unordered_map<int, Node*> val_to_node;

    int   getSize(Node* t);
    void  update(Node* t);
    void  push(Node* t);
    void  split(Node* t, int k, Node*& l, Node*& r);
    Node* merge(Node* l, Node* r);
    void  pushToRoot(Node* t);
    int   getPos(Node* t);
    Node* kth(Node* t, int k);

    std::vector<std::pair<int, int> > changes;
};
