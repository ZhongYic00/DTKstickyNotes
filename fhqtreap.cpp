#include <QDebug>
#include <bits/stdc++.h>
template <typename ValueType>
class Treap {
    enum constNumbers { P_INF = INT_MAX,
        N_INF = INT_MIN };
    struct Node {
        std::shared_ptr<ValueType> key;
        int prio, tSize;
        Node* ch[2];
        Node(const ValueType& value)
        {
            key = std::make_shared<ValueType>(value);
            prio = random();
            ch[0] = ch[1] = nullptr;
            tSize = 1;
        }
        ~Node()
        {
            key.reset();
        }
        inline void maintain() { tSize = (ch[0] ? ch[0]->tSize : 0) + (ch[1] ? ch[1]->tSize : 0) + 1; }
        //        inline void reverse(){rev^=1,swap(ch[0],ch[1]);}
        //        inline void pushdown(){if(rev){if(ch[0])ch[0]->reverse();if(ch[1])ch[1]->reverse();rev=false;}}
        void print()
        {
            //            pushdown();
            if (ch[0])
                ch[0]->print();
            qDebug() << key << ' ';
            if (ch[1])
                ch[1]->print();
        }
        std::list<ValueType> collect()
        {
            std::list<ValueType> m({ *(key.get()) }), l, r;
            if (ch[0])
                l = ch[0]->collect();
            if (ch[1])
                r = ch[1]->collect();
            m.splice(m.end(), r);
            l.splice(l.end(), m);
            return l;
        }
    };
    typedef Node* tree;

public:
    Treap()
        : root(nullptr)
    {
    }
    ~Treap()
    {
        srand(time(nullptr));
    }
    void insert(const ValueType& value)
    {
        //        qDebug()<<"insert"<<value.getOverview();
        tree few, more, cur = new Node(value);
        split(root, value, few, more);
        root = merge(few, merge(cur, more));
    }
    void erase(const ValueType& value)
    {
        tree few, more, cur;
        split(root, value, few, more);
        upper_split(few, value, few, cur);
        delete cur;
        root = merge(few, more);
    }
    ValueType getKth(const int& k) const
    {
        return getKth(root, k);
    }
    inline ValueType getKth(const ValueType& k) const
    {
        return getKth(root, k);
    }
    int queryIndex(const ValueType& k)
    {
        //        qDebug()<<"Treap::root="<<root;
        return queryIndex(root, k);
    }
    template <typename F>
    void modifyKth(int k, const F& modifier)
    {
        modifyKth(root, k, modifier);
    }
    template <typename F>
    void modifyKth(const ValueType& k, const F& modifier)
    {
        modifyKth(root, k, modifier);
    }
    int size() const
    {
        return root ? root->tSize : 0;
    }
    std::list<ValueType> getAll() const
    {
        return root ? root->collect() : std::list<ValueType>();
    }
#ifndef RELEASE
    int depth()
    {
        return root ? depth(root) : 0;
    }
    int depth(tree t, int d = 0)
    {
        d++;
        int rt = d;
        if (t->ch[0])
            rt = std::max(rt, depth(t->ch[0], d));
        if (t->ch[1])
            rt = std::max(rt, depth(t->ch[1], d));
        return rt;
    }
#endif

private:
    int seed = 10007;
    tree root;
    static int random()
    {
        return rand();
    }
    void split(const tree& t, const int& jud, tree& l, tree& r)
    {
        if (!t)
            return (void)(l = r = nullptr);
        int lsize = t->ch[0] ? (t->ch[0]->tSize + 1) : 1;
        if (lsize <= jud)
            l = t, split(t->ch[1], jud - lsize, t->ch[1], r);
        else
            r = t, split(t->ch[0], jud, l, t->ch[0]);
        t->maintain();
    }
    void split(const tree& t, const ValueType& jud, tree& l, tree& r) // split whatever greater than jud into right subTree
    {
        if (!t)
            return (void)(l = r = nullptr);
        if (t->key->operator<=(jud))
            l = t, split(t->ch[1], jud, t->ch[1], r);
        else
            r = t, split(t->ch[0], jud, l, t->ch[0]);
        t->maintain();
    }
    void upper_split(const tree& t, const ValueType& jud, tree& l, tree& r) // split whatever smaller than jud into left subTree
    {
        if (!t)
            return (void)(l = r = nullptr);
        if (t->key->operator<(jud))
            l = t, upper_split(t->ch[1], jud, t->ch[1], r);
        else
            r = t, upper_split(t->ch[0], jud, l, t->ch[0]);
        t->maintain();
    }
    ValueType getKth(const tree& t, const int& k) const
    {
        if (!t)
            return ValueType();
        int lsize = 1 + (t->ch[0] ? t->ch[0]->tSize : 0);
        if (lsize < k)
            return getKth(t->ch[1], k - lsize);
        else if (lsize == k)
            return *(t->key);
        else
            return getKth(t->ch[0], k);
    }
    ValueType getKth(const tree& t, const ValueType& k) const
    {
        if (!t)
            return ValueType();
        if (*t->key < k)
            return getKth(t->ch[1], k);
        else if (*t->key > k)
            return getKth(t->ch[0], k);
        else
            return *(t->key);
    }
    int queryIndex(const tree& t, const ValueType& k)
    {
        //        qDebug()<<"Treap::queryIndex("<<t<<")";
        //        qDebug()<<t->key->getUpdateTimeRaw();
        if (!t)
            return 0;
        if (*t->key < k)
            return queryIndex(t->ch[1], k) + (1 + (t->ch[0] ? t->ch[0]->tSize : 0));
        else if (*t->key > k)
            return queryIndex(t->ch[0], k);
        else
            return (1 + (t->ch[0] ? t->ch[0]->tSize : 0));
    }
    template <typename F>
    void modifyKth(const tree& t, const int& k, const F& modifier)
    {
        if (!t)
            return;
        int lsize = 1 + (t->ch[0] ? t->ch[0]->tSize : 0);
        if (lsize < k)
            modifyKth(t->ch[1], k - lsize, modifier);
        else if (lsize == k)
            modifier(t->key.get());
        else
            modifyKth(t->ch[0], k, modifier);
    }
    template <typename F>
    void modifyKth(const tree& t, const ValueType& k, const F& func)
    {
        if (!t)
            return;
        if (*t->key < k)
            modifyKth(t->ch[1], k, func);
        else if (*t->key > k)
            modifyKth(t->ch[0], k, func);
        else
            func(t->key.get());
    }
    tree merge(const tree& l, const tree& r)
    {
        if (!l || !r)
            return l ? l : r;
        tree rt;
        if (l->prio < r->prio)
            rt = l, l->ch[1] = merge(l->ch[1], r);
        else
            rt = r, r->ch[0] = merge(l, r->ch[0]);
        rt->maintain();
        return rt;
    }
};
