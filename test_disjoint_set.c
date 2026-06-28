#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "disjoint_set.h"
#include "HashTable/hash_table/generic.h"
#include "Vector+List/comparators.h"
#include "spanning_tree.c"

void test1() {
    //make_node

    DSNode *node = make_node(10);
    assert(node != NULL);
    assert(node->value == 10);
    assert(node->parent == node);

    printf("test make_node - OK\n");
    free(node);
}
void test2() {
    //dsu_create

    DisjointSet *dsu = dsu_create();
    assert(dsu != NULL);
    assert(dsu->trees != NULL);
    assert(dsu->trees->size == 0);

    printf("test dsu_create - OK\n");
    freeDisjointSet(dsu);
}

void test3() {
    //find_set for single node

    DSNode *node = make_node(100);
    DSNode *root = find_set(node);

    assert(root != NULL);
    assert(root == node);
    printf("test find_set single - OK\n");
    free(node);
}

void test4() {
    //union_sets

    DSNode *a = make_node(1);
    DSNode *b = make_node(2);
    union_sets(a, b);
    assert(find_set(a) == find_set(b));

    printf("test union_sets - OK\n");
    free(a);
    free(b);
}


void test5() {
    //transitivity

    DSNode *a = make_node(1);
    DSNode *b = make_node(2);
    DSNode *c = make_node(3);

    union_sets(a, b);
    union_sets(b, c);
    assert(find_set(a) == find_set(c));
    printf("test transitivity - OK\n");

    free(a);
    free(b);
    free(c);
}

void test6() {
    // idempotency

    DSNode *a = make_node(1);
    DSNode *b = make_node(2);

    union_sets(a, b);
    DSNode *root1 = find_set(a);
    union_sets(a, b);
    DSNode *root2 = find_set(a);

    assert(root1 == root2);
    assert(find_set(a) == find_set(b));
    printf("test idempotency - OK\n");

    free(a);
    free(b);
}
void test7() {
    // dsu_create_from_hash_table

    HashTable *table = createHashTable(sizeof(long long), sizeof(int));

    long long k1 = 1;
    long long k2 = 2;
    long long k3 = 3;
    int v = 0;

    setItemHashTable(table, &k1, &v, HashLL, longLongEquals);
    setItemHashTable(table, &k2, &v, HashLL, longLongEquals);
    setItemHashTable(table, &k3, &v, HashLL, longLongEquals);

    DisjointSet *dsu = dsu_create_from_hash_table(table);
    assert(dsu != NULL);

    DSNode *n1 = find_set_from_key(dsu, 1);
    DSNode *n2 = find_set_from_key(dsu, 2);
    DSNode *n3 = find_set_from_key(dsu, 3);

    assert(n1 != NULL);
    assert(n2 != NULL);
    assert(n3 != NULL);
    assert(n1->value == 1);
    assert(n2->value == 2);
    assert(n3->value == 3);

    printf("test dsu_create_from_hash_table - OK\n");

    freeDisjointSet(dsu);
    freeHashTable(table);
}

void test8() {
    // make_node + find_set

    DSNode *a = make_node(42);
    assert(a != NULL);
    assert(find_set(a) == a);

    printf("test make + find - OK\n");
    free(a);
}


void test9() {
    // simple union

    DSNode *a = make_node(1);
    DSNode *b = make_node(2);
    DSNode *c = make_node(3);

    union_sets(a, b);
    union_sets(b, c);

    DSNode *ra = find_set(a);
    DSNode *rc = find_set(c);
    assert(ra == rc);

    printf("test chain union - OK\n");

    free(a);
    free(b);
    free(c);
}


void test10() {
    // union twice

    DSNode *a = make_node(1);
    DSNode *b = make_node(2);

    union_sets(a, b);
    DSNode *r1 = find_set(a);

    union_sets(a, b);
    DSNode *r2 = find_set(a);

    assert(r1 == r2);

    printf("test union idempotency - OK\n");

    free(a);
    free(b);
}


void test11() {
    // separate components

    DSNode *a = make_node(1);
    DSNode *b = make_node(2);
    DSNode *c = make_node(3);

    union_sets(a, b);

    assert(find_set(a) == find_set(b));
    assert(find_set(a) != find_set(c));

    printf("test separate components - OK\n");

    free(a);
    free(b);
    free(c);
}


void test12() {
    const int N = 1000;
    DSNode* nodes[N];

    for (int i = 0; i < N; i++) {
        nodes[i] = make_node(i);
    }

    for (int i = 0; i < N - 1; i++) {
        union_sets(nodes[i], nodes[i + 1]);
    }

    DSNode* root = find_set(nodes[0]);
    for (int i = 1; i < N; i++) {
        assert(find_set(nodes[i]) == root);
    }

    printf("test stress union - OK\n");

    for (int i = 0; i < N; i++) {
        free(nodes[i]);
    }
}

int main() {

    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();
    test11();
    test12();

    printf("eeee\n");

    return 0;
}