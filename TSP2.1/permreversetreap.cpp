#include "common.h"

PermReverseTreap::Node::Node(int v)
    : val(v)
    , prior(rand())
    , size(1)
    , rev(false)
    , l(nullptr)
    , r(nullptr)
    , parent(nullptr)
{
}

PermReverseTreap::PermReverseTreap(const std::vector<int>& V)
{
    n = V.size();
    srand(static_cast<unsigned>(time(nullptr)));
    root = nullptr;
    for(int v : V)
    {
        Node* node = new Node(v);
        val_to_node[v] = node;
        root = merge(root, node);
    }
}

int PermReverseTreap::getSize(Node* t)
{
    return t ? t->size : 0;
}

void PermReverseTreap::update(Node* t)
{
    if(!t) return;
    t->size = 1 + getSize(t->l) + getSize(t->r);
    if(t->l) t->l->parent = t;
    if(t->r) t->r->parent = t;
}

void PermReverseTreap::push(Node* t)
{
    if(t && t->rev)
    {
        std::swap(t->l, t->r);
        if(t->l) t->l->rev = !t->l->rev;
        if(t->r) t->r->rev = !t->r->rev;
        t->rev = false;
    }
}

void PermReverseTreap::split(Node* t, int k, Node*& l, Node*& r)
{
    if(!t)
    {
        l = nullptr;
        r = nullptr;
        return;
    }
    push(t);
    if(getSize(t->l) >= k)
    {
        split(t->l, k, l, t->l);
        if(t->l) t->l->parent = t;
        r = t;
    }
    else
    {
        split(t->r, k - getSize(t->l) - 1, t->r, r);
        if(t->r) t->r->parent = t;
        l = t;
    }
    update(t);
    t->parent = nullptr;
}

PermReverseTreap::Node* PermReverseTreap::merge(Node* l, Node* r)
{
    push(l);
    push(r);
    if(!l) return r;
    if(!r) return l;
    if(l->prior > r->prior)
    {
        l->r = merge(l->r, r);
        update(l);
        l->parent = nullptr;
        return l;
    }
    r->l = merge(l, r->l);
    update(r);
    r->parent = nullptr;
    return r;
}

void PermReverseTreap::pushToRoot(Node* t)
{
    std::vector<Node*> path;
    while(t)
    {
        path.push_back(t);
        t = t->parent;
    }
    for(int i = static_cast<int>(path.size()) - 1; i >= 0; --i)
        push(path[i]);
}

int PermReverseTreap::getPos(Node* t)
{
    push(t);
    int res = getSize(t->l);
    while(t->parent)
    {
        if(t->parent->r == t) res += getSize(t->parent->l) + 1;
        t = t->parent;
    }
    return res;
}

PermReverseTreap::Node* PermReverseTreap::kth(Node* t, int k)
{
    push(t);
    int leftSize = getSize(t->l);
    if(k < leftSize) return kth(t->l, k);
    if(k == leftSize) return t;
    return kth(t->r, k - leftSize - 1);
}

void PermReverseTreap::Cut(int l)
{
    Reverse(0, l+1);
    Reverse(l+1, n);
    Reverse(0, n);
    Apply();
}

void PermReverseTreap::Reverse(int l, int r)
{
    changes.push_back({l, r});

    Node* a = nullptr;
    Node* b = nullptr;
    Node* c = nullptr;
    split(root, r, b, c);
    split(b, l, a, b);
    if(b) b->rev = !b->rev;
    root = merge(a, merge(b, c));
}

int PermReverseTreap::Where(int x)
{
    Node* node = val_to_node[x];
    pushToRoot(node);
    return getPos(node);
}

int PermReverseTreap::At(int i)
{
    return kth(root, i)->val;
}

void PermReverseTreap::Undo()
{
    auto [l, r] = changes.back();
    Reverse(l, r);
    changes.pop_back();
    changes.pop_back();
}

void PermReverseTreap::Apply()
{
    changes.clear();
}
