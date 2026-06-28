#include <stdio.h>
#include <stdlib.h>

#include "disjoint_set.h"
#include "HashTable/hash_table/generic.h"
#include "Vector+List/vector/generic.h"
#include "Vector+List/comparators.h"

#include "Vector+List/vector/generic.c"
#include "Vector+List/list/generic.c"
#include "HashTable/hash_table/generic.c"
#include "Graph/graph.c"

DSNode *make_node(long long value) {
    DSNode *node = malloc(sizeof(DSNode));
    if (!node) {
        printf("error: create node in make_node");
        return NULL;
    }
    node->value = value;
    node->parent = node;
    node->rank = 0;
    return node;
}

DisjointSet *dsu_create() {
    DisjointSet *dsu = malloc(sizeof(DisjointSet));
    if (!dsu) {
        printf("error: create dsu in dsu_create");
        return NULL;
    }

    dsu->trees = createHashTable(sizeof(long long), sizeof(DSNode *));
    if (!dsu->trees) {
        printf("eror: create dsu->trees in dsu_create");
        free(dsu);
        return NULL;
    }

    return dsu;
}

DisjointSet *dsu_create_from_hash_table(HashTable *nodes)
{
    if (!nodes) {
        printf("error: NULL nodes in dsu_create_from_hash_table");
        return NULL;
    }

    DisjointSet *dsu = dsu_create();
    if (!dsu) {
        printf("error: create dsu in dsu_create_from_hash_table");
        return NULL;
    }

    for (size_t i = 0; i < nodes->capacity; ++i) {
        Vector **bucket_ptr = get_bucket_ptr(nodes, i);
        Vector *bucket = *bucket_ptr;

        if (!bucket) {
            continue;
        }

        for (size_t j = 0; j < bucket->size; ++j) {
            void *elem = getVectorItem(bucket, j);

            long long key = *(long long *)elem;

            DSNode *node = make_node(key);
            if (!node) {
                continue;
            }
            setItemHashTable(dsu->trees, &key, &node, HashLL,longLongEquals);
        }
    }

    return dsu;

}

DSNode *find_set(DSNode *node) {
    if (!node) {
        printf("error: NULL dsu in find_set");
        return NULL;
    }

    if (node->parent != node) {
        node->parent = find_set(node->parent);
    }
    return node->parent;
}

DSNode *find_set_from_key(DisjointSet *dsu, long long key)
{
    if (!dsu) {
        printf("error: NULL dsu in find_set_from_key");
        return NULL;
    }

    DSNode **node_ptr = (DSNode **)getItemHashTable(dsu->trees, &key, HashLL,longLongEquals);

    if (!node_ptr) {
        printf("error: create node_ptr in find_set_from_key");
        return NULL;
    }
    return find_set(*node_ptr);
}

void union_sets(DSNode *a, DSNode *b)
{
    if (!a || !b) {
        printf("error: NULL sets in union_sets");
        return;
    }

    DSNode *root_a = find_set(a);
    DSNode *root_b = find_set(b);

    if (root_a == root_b) {
        return;
    }
    // root_b->parent = root_a; было в базовом задании
    //стало:
    if (root_a->rank < root_b->rank) {
        DSNode *tmp = root_a;
        root_a = root_b;
        root_b = tmp;
    }
    root_b->parent = root_a;
    if (root_a->rank == root_b->rank) {
        root_a->rank++;
    }
}

void freeDisjointSet(DisjointSet *dsu)
{
    if (!dsu) {
        return;
    }

    if (dsu->trees) {
        for (size_t i = 0; i < dsu->trees->capacity; ++i) {
            Vector **bucket_ptr = get_bucket_ptr(dsu->trees, i);
            Vector *bucket = *bucket_ptr;

            if (!bucket) {
                continue;
            }

            for (size_t j = 0; j < bucket->size; ++j) {
                void *elem = getVectorItem(bucket, j);

                DSNode **node_ptr =
                    (DSNode **)((char *)elem + sizeof(long long));

                if (node_ptr && *node_ptr) {
                    free(*node_ptr);
                }
            }
        }

        freeHashTable(dsu->trees);
    }

    free(dsu);
}