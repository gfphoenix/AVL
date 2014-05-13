#ifndef  _AVL_H
#define  _AVL_H

struct avl_node {
    struct avl_node *l;   // left child
    struct avl_node *r;  // right child
    struct avl_node *p;  // parent
    int balance;  		// balance : -1, 0 or 1, 
    avl_node():l(nullptr),r(nullptr),p(nullptr),balance(0){}
};

template <typename K, typename V>
class avl_node_wrapper : public avl_node {
    public:
        K key;
        V val;
        avl_node_wrapper(const K &k, const V &v):avl_node(),key(k), val(v){}
        avl_node_wrapper(K &&k, V &&v):avl_node(),key(k),val(v){}
};
template <typename K, typename V>
class avl_root{
    private:
        avl_node *root;
    public:
        avl_root():root(0){}
        // insert
        int _insert(avl_node *x);
        int insert(K &key, V &val);
        // find
        avl_node_wrapper<K, V> *_find(const K &key)const;
        V * find(const K &key)const;
        // del
        avl_node_wrapper<K,V> *del(const K &key);
        void del_node(avl_node_wrapper<K,V> *node);
        void del2(const K &key);
};

void avl_fixup_insert(struct avl_node *&r, struct avl_node *x);
void avl_del_node(struct avl_node *&r, struct avl_node *n);

    template <typename K, typename V>
struct avl_node *avl_delete(struct avl_node *&r, const K &key)
{
    struct avl_node *n;
    n = _find(key);
    if (!n)
        return nullptr;
    avl_del_node(r, n);
    return n;
}

    template <typename K, typename V>
avl_node_wrapper<K,V> *avl_root<K,V>::del(const K &key)
{
    return avl_delete(root, key);
}

    template <typename K, typename V>
void avl_root<K,V>::del2(const K &key)
{
    auto x = del(key);
    if(x)
        del_node(x);
}

    template <typename K, typename V>
int avl_root<K,V>::_insert(struct avl_node *x)
{
    struct avl_node *parent = nullptr; //this ensures the parent of the root node is NULL;
    struct avl_node **n = &root;
    x->l = x->r = x->p = nullptr;
    x->balance = 0;

    auto X = static_cast<avl_node_wrapper<K, V>*>(x);

    while ((*n)) {
        parent = *n;
        auto a = static_cast<avl_node_wrapper<K,V>*>(parent);
        if(a->key < X->key)
            n = &parent->r;
        else if(X->key < a->key)
            n = &parent->l;
        else
            return -1;
    }
    *n = x;
    x->p = parent;
    avl_fixup_insert(root, x);
    return 0;
}

    template <typename K, typename V>
int avl_root<K,V>::insert(K &key, V &val)
{
    auto n = new avl_node_wrapper<K, V>(key, val);
    return _insert(n);
}

template <typename K, typename V>
avl_node_wrapper<K,V> * avl_root<K,V>::_find(const K &key)const 
{
    auto p = root;
    while(p){
        auto item = static_cast<avl_node_wrapper<K, V>*>(p);
        if(*item < key)
            p = p->r;
        else if(key < *item)
            p = p->l;
        else
            return item;
    }
    return nullptr;
}
template <typename K, typename V>
V *avl_root<K,V>::find(const K &key)const {
    auto node = static_cast<avl_node_wrapper<K, V>*>(_find(key));
    return node ? node->val : nullptr;
}

#endif
