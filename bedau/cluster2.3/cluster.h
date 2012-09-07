/*
 * cluster.h -- declarations for clustering routines
 *
 * $Header: cluster.h,v 1.1 91/07/14 01:09:48 stolcke Exp Locker: stolcke $
 * $Log:	cluster.h,v $
 * Revision 1.1  91/07/14  01:09:48  stolcke
 * Initial revision
 * 
 *
 */

typedef struct _tree {
    FLOAT  *pat;
    int     size;
    int     root;
    int     leaf;
    FLOAT   y;
    FLOAT   distance;
    struct _tree *r_tree, *l_tree;
}       BiTree;

extern BiTree *new_tree();

#define LEAF -1

#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif


