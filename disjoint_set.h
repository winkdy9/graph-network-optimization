#ifndef DISJOINT_SET
#define DISJOINT_SET
#include <stdlib.h>
#include <stdbool.h>
#include "HashTable/hash_table/generic.h"

typedef struct DSNode {
    long long value;
    struct DSNode *parent;
    int rank;  // добавляется в хард-версии
} DSNode;

typedef struct {
    HashTable *trees;
} DisjointSet;

DSNode *make_node(long long value);

DisjointSet *dsu_create();
DisjointSet *dsu_create_from_hash_table(HashTable *nodes);

DSNode *find_set(DSNode *node);
DSNode *find_set_from_key(DisjointSet *dsu, long long key);

void union_sets(DSNode *a, DSNode *b);

void freeDisjointSet(DisjointSet *dsu);
#endif