#ifndef  _AVL_H
#define  _AVL_H

#ifndef NULL
#define NULL ((void *)0)
#endif

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

static inline void avl_init(struct avl_root *r) {
        r->root = 0;
        //r->root = NULL;
}

int avl_insert(struct avl_root *root, struct avl_node *x);
struct avl_node *avl_search(struct avl_root *r, int key);
void avl_del_node(struct avl_root *r, struct avl_node *n) ;
struct avl_node *avl_delete(struct avl_root *r, int val);
#endif
