/* $Header: /disks/disk1/Users/terry/s/me/bedau/RCS/hash.h,v 1.1 1992/09/18 09:23:07 terry Exp terry $ */
#define HASH_SZ 97

typedef struct _node {
    STRING key;
    void *data;
    struct _node *next;
} HASH_NODE;

typedef struct {
    int size;
    HASH_NODE **buckets;
} HASH_TABLE;

extern HASH_TABLE *hash_create();
extern void hash_destroy();
extern void *hash_search();
extern void hash_traverse();
extern void hash_purge();

#ifdef HASH_STATS
extern void hash_stats();
#endif
