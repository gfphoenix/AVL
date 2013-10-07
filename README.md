AVL
===

a simple and clear implementation of AVL tree

the node of avl is simple, which make it hard for general usage.
struct avl_node {
        struct avl_node *l;   // left child
        struct avl_node *r;  // right child
        struct avl_node *p;  // parent
        int balance;  		// balance : -1, 0 or 1, 
        int val;  //key for demo
};
struct avl_root {
        struct avl_node *root;
};

For your special usage, please replace the member `val' with your key and value, or you can remove the member `val' 
and embbed the avl_node into your struct, like :
struct avl_node {
        struct avl_node *l;   // left child
        struct avl_node *r;  // right child
        struct avl_node *p;  // parent
        int balance;  		// balance : -1, 0 or 1, 
};

struct foo {
    struct avl_node node;
    K key;
    V val;
};
so, if you get a point to a struct avl_node, you can easily change it to the pointer `foo`
