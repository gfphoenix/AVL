#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "avl.h"

// n is not nil
int verify_avl(struct avl_node *n, bool *ok) {
	int nl, nr;
	int h = 0;
	bool ook;
	nl = nr = 0;
	if (!n || (!n->l && !n->r))
		goto out_ok;

	if (n->l) {
		nl = verify_avl(n->l, &ook);
		if (!ook)
			goto out;
	}
	if (n->r) {
		nr = verify_avl(n->r, &ook);
		if (!ook)
			goto out;
	}
	h = nl - nr;
	if (h < -1 || h > 1)
		goto out;
	h = (nl < nr ? nr : nl) + 1;
out_ok: *ok = true;
		return h;
out: *ok = false;
	 return -1;
}


#define NR  (10  * 1000)

struct avl_node x[NR];
int main(int ac, const char *av[]) {
	struct avl_root root[1] = { NULL, };
	//    int del_order[] = {0,1,2,3,4,5};
	int i;
	bool ok;
	srand(time(0));
	for (i = 0; i < NR; i++) {
		int e;
again: 
		x[i].val = rand() % 1000000;

		e = avl_insert(root, x + i);
		if (e == -1)
			goto again;

		verify_avl(root->root, &ok);
		if (!ok)
			printf("bad avl\n");
	}
	printf("insert ok\n");
	for (i = 0; i < NR; i++) {
		avl_del_node(root, x+i);
		verify_avl(root->root, &ok);
		if(!ok)
			printf("bad avl = %d\n",i);
	}
	printf("delete ok\n");
	assert(root[0].root == NULL);
	return 0;
}

