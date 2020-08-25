#include <bits/stdc++.h>
#include <QDebug>
template<typename T>
class Treap
{
    enum constNumbers{P_INF=2147483647,N_INF=1<<31};
    struct Node
    {
        std::shared_ptr<T> key;
        int prio,tSize;
        Node *ch[2];
        Node(T &value)
        {
            key=std::make_shared<T>(value);
            prio=random();
            ch[0]=ch[1]=nullptr;
            tSize=1;
        }
        ~Node()
        {
            key.reset();
        }
        inline void maintain(){tSize=(ch[0]?ch[0]->tSize:0)+(ch[1]?ch[1]->tSize:0)+1;}
//        inline void reverse(){rev^=1,swap(ch[0],ch[1]);}
//        inline void pushdown(){if(rev){if(ch[0])ch[0]->reverse();if(ch[1])ch[1]->reverse();rev=false;}}
        void print()
        {
//            pushdown();
            if(ch[0])ch[0]->print();
            key->print();
            if(ch[1])ch[1]->print();
        }
        std::list<T> goThrough()
        {
            std::list<T> m({*(key.get())}),l,r;
            if(ch[0])l=ch[0]->goThrough();
            if(ch[1])r=ch[1]->goThrough();
            m.splice(m.end(),r);
            l.splice(l.end(),m);
            return l;
        }
    };
    typedef Node* tree;
public:
    Treap():root(nullptr){}
    ~Treap()
    {
        srand(time(NULL));
    }
    void insert(T value)
    {
//        qDebug()<<"insert"<<value.getOverview();
        tree few,more,cur=new Node(value);
        split(root,value,few,more);
        root=merge(few,merge(cur,more));
    }
    void erase(T value)
    {
        tree few,more,cur;
        split(root,value,few,more);
        upper_split(few,value,few,cur);
        delete cur;
        root=merge(few,more);
    }
    T getKth(int k) const
    {
        return getKth(root,k);
    }
    int size() const
    {
        return root?root->tSize:0;
    }
    std::list<T> getAll() const
    {
        return root?root->goThrough():std::list<T>();
    }
private:
    int seed=10007;
    tree root;
    static int random()
    {
        return rand();
    }
    void split(tree t,int jud,tree &l,tree &r)
    {
        if(!t)return (void)(l=r=nullptr);
        int lsize=t->ch[0]?(t->ch[0]->tSize+1):1;
        if(lsize<=jud)
            l=t,split(t->ch[1],jud-lsize,t->ch[1],r);
        else
            r=t,split(t->ch[0],jud,l,t->ch[0]);
        t->maintain();
    }
    void split(tree t,T &jud,tree &l,tree &r)// split whatever greater than jud into right subTree
    {
        if(!t)return (void)(l=r=nullptr);
        if(*(t->key)<=jud)
            l=t,split(t->ch[1],jud,t->ch[1],r);
        else
            r=t,split(t->ch[0],jud,l,t->ch[0]);
        t->maintain();
    }
    void upper_split(tree t,T &jud,tree &l,tree &r)// split whatever smaller than jud into left subTree
    {
        if(!t)return (void)(l=r=nullptr);
        if(*(t->key)<jud)
            l=t,upper_split(t->ch[1],jud,t->ch[1],r);
        else
            r=t,upper_split(t->ch[0],jud,l,t->ch[0]);
        t->maintain();
    }
    T getKth(tree t,int k) const
    {
        if(!t)return T();
        int lsize=1+(t->ch[0]?t->ch[0]->tSize:0);
        if(lsize<k)
            return getKth(t->ch[1],k-lsize);
        else if(lsize==k)
            return *(t->key);
        else
            return getKth(t->ch[0],k);
    }
    tree merge(tree l,tree r)
    {
        if(!l||!r)
            return l?l:r;
        tree rt;
        if(l->prio<r->prio)
            rt=l,l->ch[1]=merge(l->ch[1],r);
        else
            rt=r,r->ch[0]=merge(l,r->ch[0]);
        rt->maintain();
        return rt;
    }
};