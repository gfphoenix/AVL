#include <stdio.h>
//#include <stdbool.h>
#include "avl.h"

static void __left_rotate(struct avl_node *n, struct avl_root *root)
{
	struct avl_node *parent = n->p;
	struct avl_node *right = n->r;
	if ((n->r = right->l))
		n->r->p = n;

	n->p = right;
	right->l = n;
	right->p = parent;
	if (parent) {
		if (parent->l == n)
			parent->l = right;
		else
			parent->r = right;
	} else
		root->root = right;
}
static void __right_rotate(struct avl_node *n, struct avl_root *root)
{
	struct avl_node *parent = n->p;
	struct avl_node *left = n->l;
	if ((n->l = left->r))
		n->l->p = n;

	n->p = left;
	left->r = n;
	left->p = parent;
	if (parent) {
		if (parent->l == n)
			parent->l = left;
		else
			parent->r = left;
	} else
		root->root = left;
}

static inline void avl_lr_rotate(struct avl_node *parent, struct avl_root *r)
{
	__left_rotate(parent->l, r);
	__right_rotate(parent, r);
}
static inline void avl_rl_rotate(struct avl_node *parent, struct avl_root *r)
{
	__right_rotate(parent->r, r);
	__left_rotate(parent, r);
}

static void avl_fixup_insert(struct avl_root *r, struct avl_node *x)
{
	struct avl_node *parent;
	struct avl_node *child;

	for(; (parent = x->p); x = parent){
		if (parent->l == x) {
			parent->balance++;
			if (parent->balance == 0)
				break;
			if (parent->balance == 1) 
				continue;
			//rotate
			//parent ->balance is 2
			//assert(parent->balance == 2);
			if (x->balance == 1) { //LL
				x->balance = 0;
				parent->balance = 0;
				__right_rotate(parent, r);
				break;
			}
			//LR, x->balance == -1
			child = x->r;
			/*
			   switch (child->balance) {
			   case 1:
			   child->balance = x->balance = 0;
			   parent->balance = -1;
			   break;
			   case -1:
			   child->balance = parent->balance = 0;
			   x->balance = 1;
			   break;
			   case 0:
			   x->balance = parent->balance = 0;
			   break;

			   }
			 */
			parent->balance = x->balance = 0;
			if (child->balance == 1)
				parent->balance = -1;
			else if (child->balance == -1)
				x->balance = 1;
			child->balance = 0;

			avl_lr_rotate(parent, r);
			break;
		} else {
			parent->balance--;
			if (parent->balance == 0)
				break;
			if (parent->balance == -1) 
				continue;
			//rotate
			//assert(parent->balance == -2);
			if (x->balance == -1) {
				x->balance = parent->balance = 0;
				__left_rotate(parent, r);
				break;
			}
			//RL, x->balance = 1
			child = x->l;
			/*
			   switch (child->balance) {
			   case -1:
			   child->balance = x->balance = 0;
			   parent->balance = 1;
			   break;
			   case 1:
			   child->balance = parent->balance = 0;
			   x->balance = -1;
			   break;
			   case 0:
			   x->balance = parent->balance = 0;
			   break;

			   }
			 */
			parent->balance = x->balance = 0;
			if (child->balance == -1)
				parent->balance = 1;
			else if (child->balance == 1)
				x->balance = -1;
			child->balance = 0;

			avl_rl_rotate(parent, r);
			break;
		}
	}
}


// x's balance is to be decreased
// left == true child is left, left == false, child is right
static void avl_fixup_del(struct avl_root *r, struct avl_node *x, bool left)
{
	struct avl_node *parent;
	struct avl_node *child;
	struct avl_node *gc;
	for(;x;x=parent){
		parent = x->p;
		if (left) {
			x->balance--;
			if (x->balance == -1)
				break;
			if (parent && parent->r == x)
				left = false;
			if (x->balance == 0)
				continue;
			// x's balance is -1, now to be -2, so should rotate.
			child = x->r;
			//assert(x->balance == -2 && child);
			if (child->balance == 0) {
				x->balance = -1;
				child->balance = 1;
				__left_rotate(x, r);
				break;
			}
			if (child->balance == -1) {
				x->balance = child->balance = 0;
				__left_rotate(x, r);
				continue;
			}
			gc = child->l;
			//child 's balance == 1
			//assert(child->balance == 1 && gc);

			x->balance = child->balance = 0;
			if(gc->balance == -1)
				x->balance = 1;
			else if(gc->balance == 1)
				child->balance = -1;
			gc->balance = 0;
			avl_rl_rotate(x, r);
		} else {
			x->balance++;
			if (x->balance == 1)
				break;
			if (parent && parent->l == x)
				left = true;
			if (x->balance == 0)
				continue;

			child = x->l;
			//assert(x->balance == 2 && child);
			if (child->balance == 0) {
				x->balance = 1;
				child->balance = -1;
				__right_rotate(x, r);
				break;
			}
			if (child->balance == 1) {
				x->balance = child->balance = 0;
				__right_rotate(x, r);
				continue;
			}
			gc = child->r;
			//assert(child->balance == -1 && gc);

			x->balance = child->balance = 0;
			if(gc->balance == 1)
				x->balance = -1;
			else if(gc->balance == -1)
				child->balance = 1;
			gc->balance = 0;
			avl_lr_rotate(x, r);
		}
	}
}
struct avl_node *avl_search(struct avl_root *r, int key)
{
	struct avl_node *n = r->root;
	while (n) {
		if (n->val < key)
			n = n->r;
		else if (key < n->val)
			n = n->l;
		else
			return n;
	}
	return NULL;
}


int avl_insert(struct avl_root *root, struct avl_node *x)
{
	struct avl_node *parent = NULL; //this ensures the parent of the root node is NULL;
	struct avl_node **n = &root->root;
	x->l = x->r = x->p = NULL;
	x->balance = 0;

	while ((*n)) {
		parent = *n;
		if (parent->val < x->val)
			n = &parent->r;
		else if (parent->val > x->val)
			n = &parent->l;
		else
			return -1;
	}
	*n = x;
	x->p = parent;
	avl_fixup_insert(root, x);
	return 0;
}


struct avl_node *avl_delete(struct avl_root *r, int val)
{
	struct avl_node *n;
	n = avl_search(r, val);
	if (!n)
		return NULL;
	avl_del_node(r, n);
	return n;
}
void avl_del_node(struct avl_root *r, struct avl_node *n)
{
	struct avl_node *parent;
	struct avl_node *child;
	struct avl_node *target;
	bool left;
	target = parent = n->p;
	if (!n->l)
		child = n->r;
	else if (!n->r)
		child = n->l;
	else {
		//both n->l and n->r are not nil
		struct avl_node *x = n->r;
		struct avl_node *xp;
		while (x->l)
			x = x->l;

		child = x->r;
		xp = x->p;
		if (parent) {
			if (parent->l == n)
				parent->l = x;
			else
				parent->r = x;
		} else
			r->root = x;
		x->p = parent; //
		x->l = n->l;
		n->l->p = x;
		x->balance = n->balance;

		if (xp != n) { //n->r has no left child
			if (child)
				child->p = xp;
			xp->l = child;
			x->r = n->r;
			n->r->p = x;
			target = xp;
			left=true;
		} else{
			target = x;
			left = false;
		}
		goto fixup;
	}
	if (parent) {
		if (parent->l == n) {
			parent->l = child;
			left = true;
		} else {
			parent->r = child;
			left = false;
		}
	} else
		r->root = child;
	if (child)
		child->p = parent;

fixup: 
	if (r->root)
		avl_fixup_del(r, target, left);
	n->p=n->l=n->r=NULL;
}

