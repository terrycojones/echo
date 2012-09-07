#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/hash.c,v 1.3 1992/11/16 07:05:45 terry Exp terry $";
#endif


#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>

#include "types.h"
#include "syscalls.h"

#define HASH_NULL    (HASH_TABLE *)0
#define NODE_NULL    (HASH_NODE *)0
#define GENERIC_NULL (void *)0

#define HASH_SZ 97


static int hash();
static HASH_NODE *list_find();


/*
 * hash_create()
 *
 * Malloc room for a new hash table and then room for its
 * bucket pointers. Then set all the buckets to
 * point to 0. Return the address of the new table.
 */
HASH_TABLE *
hash_create(size)
int size;
{
    register int i;  
    HASH_TABLE *new = (HASH_TABLE *)Malloc(sizeof(HASH_TABLE));

	rcsid = rcsid;

    if (!new || size < 0){
	return HASH_NULL;
    }
    
    if (size == 0){
	size = HASH_SZ;
    }
    
    if (!(new->buckets = (HASH_NODE **)Malloc(size * sizeof(HASH_NODE *)))){
	return HASH_NULL;
    }

    for (i = 0; i < size; i++){
	new->buckets[i] = NODE_NULL;
    } 
    new->size = size;
    
    return new;
}


/*
 * list_find()
 *
 * Find the key in the linked list pointed to by head.
 */
static HASH_NODE *
list_find(key, head)
STRING key;
HASH_NODE *head;
{
    while (head){
	if (!strcmp(head->key, key)){
	    return head;
	}
	head = head->next;
    }
    return NODE_NULL;
}


/*
 * hash()
 *
 * Compute the hash value for the given key.
 */
static int
hash(size, key)
int size;
register STRING key;
{
    register int h = 0x0;

    while (*key){
	h = (h << 1) ^ (h ^ *key);
	key++;
    }	

    h %= size;
    return h < 0 ? -1 * h : h;
}

/*
 * hash_destroy()
 *
 * Find the key and (if it's there) remove it entirely.
 * The function (*nukefunc)() is in charge of disposing
 * of the storage help by the data associated with the node.
 */
void 
hash_destroy(table, key, nukefunc)
HASH_TABLE *table;
STRING key;
void (*nukefunc)();
{
    int bucket = hash(table->size, key);
    HASH_NODE *found = table->buckets[bucket];
    HASH_NODE *to_free = NODE_NULL;
    
    if (!found){
	return;
    }
    
    if (!strcmp(found->key, key)){
	/*
	 * It was the head of the list.
	 */
	table->buckets[bucket] = found->next;
	to_free = found;
    }
    else{
	/*
	 * Walk the list, looking one ahead.
	 */
	while (found->next){
	    if (!strcmp(found->next->key, key)){
		to_free = found->next;
		found->next = found->next->next;
		break;
	    }
	    found = found->next;
	}
	
	if (!to_free){
	    return;
	}
    }
    
    if (nukefunc){
	(*nukefunc)(to_free->data);
    }
    Free(to_free);
    return;
}


/*
 * hash_search()
 *
 * Search the table for the given key. Then:
 *
 * 1) If you find it and there is no replacement function, just 
 *    return what you found. (This is a simple search). 
 * 2) If you find it and there is a replacement function, run
 *    the function on the data you found, and replace the old
 *    data with whatever is passed in datum. Return 0.
 * 3) If you don't find it and there is some datum, insert a
 *    new item into the table. Insertions go at the front of
 *    the bucket. Return 0.
 * 4) Otherwise just return 0.
 *
 */
void *
hash_search(table, key, datum, replace_func)
HASH_TABLE *table;
STRING key;
void *datum;
void (*replace_func)(); 
{
    int bucket = hash(table->size, key);
    HASH_NODE *found = list_find(key, table->buckets[bucket]);

    if (found){
	if (!replace_func){
	    return found->data;
	}
	else{
	    (*replace_func)(found->data);
	    found->data = datum;
	}
    }
    else{
	if (datum){
	    
	    static int assign_key();
	    
	    HASH_NODE *new = (HASH_NODE *)Malloc(sizeof(HASH_NODE));
	    
	    if (!new || !assign_key(key, new)){
		return GENERIC_NULL;
	    }
	    new->data = datum;
	    new->next = table->buckets[bucket];
	    table->buckets[bucket] = new;
	}
    }
    return GENERIC_NULL;
}


/*
 * assign_key()
 *
 * Set the key value of a node to be 'key'. Get some space from 
 * Malloc and copy it in etc. Return 1 if all is well, 0 otherwise.
 */
static int
assign_key(key, node)
STRING key;
HASH_NODE *node;
{
    if (!node || !key){
	return 0;
    }
    
    if (!(node->key = Malloc(strlen(key) + 1))){
	return 0;
    }
    
    node->key[0] = '\0';
    strcat(node->key, key);
    return 1;
}

/*
 * hash_traverse()
 *
 * Traverse the hash table and run the function func on the
 * data found at each node and the argument we're passed for it.
 */
void
hash_traverse(table, func, arg1, arg2)
HASH_TABLE *table;
int (*func)();
void *arg1;
void *arg2;
{
    register int i; 
    register int size = table->size;

    if (!func){
	return;
    }
    
    for (i = 0; i < size; i++){
	HASH_NODE *n = table->buckets[i];
	while (n){
	    if ((*func)(n->data, arg1, arg2) == 0){
		return;
	    }
	    n = n->next;
	}
    } 
    
    return;
}

/*
 * hash_purge()
 *
 * Run through the entire hash table. Call purge_func
 * on the data found at each node, and then free the node.
 * Set all the bucket pointers to 0.
 */
void
hash_purge(table, purge_func)
HASH_TABLE *table;
void (*purge_func)();
{
    register int i; 
    register int size = table->size;
    
    for (i = 0; i < size; i++){
	HASH_NODE *n = table->buckets[i];
	if (n){
	    do {
		HASH_NODE *to_free = n;
		if (purge_func){
		    (*purge_func)(n->data);
		}
		n = n->next;
		Free(to_free);
	    } while (n);
	    
	    table->buckets[i] = NODE_NULL;
	}
    } 
}

#define min(a, b) (a) < (b) ? (a) : (b)

/*
 * hash_stats()
 *
 * Print statistics about the current table allocation to stdout.
 */
void
hash_stats(table, verbose)
HASH_TABLE *table;
STRING verbose;
{
    register int i; 
    int total_elements = 0;
    int non_empty_buckets = 0;
    int max_count = 0;
    int max_repeats = 0;
    int *counts;
    int size = table->size;
    
    if (!(counts = (int *)Malloc(size * sizeof(int)))){
	fprintf(stderr, "Malloc returns 0\n");
	exit(1); 
    }
    
    for (i = 0; i < size; i++){
	int x = 0;
	HASH_NODE *n = table->buckets[i];
	counts[i] = 0;
	while (n){
	    if (!x){
		x = 1;
		non_empty_buckets++;
		if DEBUG("h") printf("bucket %2d: ", i);
	    }
	    if DEBUG("h") {
		STRING tmp = n->key;
		
		while (*tmp){
		    printf("%d ", (int)*tmp);
		    tmp++;
		}

		/* printf(" %s", n->key); */
	    }
	    counts[i]++;
	    n = n->next;
	}
	
	total_elements += counts[i];
	if (counts[i] > max_count){
	    max_count = counts[i];
	    max_repeats = 1;
	}
	else if (counts[i] == max_count){
	    max_repeats++;
	}
	
	if (counts[i]){
	    if DEBUG("h") printf(" (%d)\n", counts[i]);
	}
    } 
    
    printf("\n");
    printf("%d element%s in storage.\n", total_elements, total_elements == 1 ? "" : "s");
    
    if (total_elements){
	printf("%d of %d (%.2f%%) buckets are in use\n", non_empty_buckets, size, 
	       (double)100 * (double)non_empty_buckets / (double)(size));
	printf("the maximum number of elements in a bucket is %d (%d times)\n", max_count, max_repeats);
	printf("average per bucket is %f\n", (double)total_elements / (double)non_empty_buckets);
	printf("optimal would be %f\n", (double)total_elements / (double)(min(size, total_elements)));
    }
    return;
}

