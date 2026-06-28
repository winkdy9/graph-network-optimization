#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "generic.h"
#include "../lab3/comparators.h"


void test1() {
    //createDoublyList 

    DoublyList *list = createDoublyList(sizeof(int));

    assert(list != NULL);
    assert(list->head == NULL);
    assert(list->end == NULL);

    printf("test create list - OK\n");
    freeDoublyList(list);
}


void test2() {
    //append

    DoublyList *list = createDoublyList(sizeof(int));

    int x = 10;
    appendDoublyItem(list, &x);

    assert(list->head != NULL);
    assert(list->end != NULL);
    assert(*(int *)list->head->data == 10);
    assert(*(int *)list->end->data == 10);

    printf("test append 1 - OK\n");

    freeDoublyList(list);
}


void test3() {
    // append several

    DoublyList *list = createDoublyList(sizeof(int));

    int a = 1;
    int b = 2;
    int c = 3;

    appendDoublyItem(list, &a);
    appendDoublyItem(list, &b);
    appendDoublyItem(list, &c);

    assert(*(int *)list->head->data == 1);
    assert(*(int *)list->end->data == 3);
    printf("test append several - OK\n");
    freeDoublyList(list);
}


void test4() {
    // find

    DoublyList *list = createDoublyList(sizeof(int));
    int a = 5;
    int b = 10;
    int c = 15;

    appendDoublyItem(list, &a);
    appendDoublyItem(list, &b);
    appendDoublyItem(list, &c);

    int target = 10;

    long pos = findDoublyItem(list, &target, intEquals_bool);

    assert(pos == 1);

    printf("test find - OK\n");
    freeDoublyList(list);
}


void test5() {
    // pop middle

    DoublyList *list = createDoublyList(sizeof(int));

    int a = 1;
    int b = 2;
    int c = 3;

    appendDoublyItem(list, &a);
    appendDoublyItem(list, &b);
    appendDoublyItem(list, &c);
    int *value = (int *)popDoublyItem(list, 1);

    assert(value != NULL);
    assert(*value == 2);
    free(value);

    assert(doublyListLength(list) == 2);

    printf("test pop middle - OK\n");
    freeDoublyList(list);
}


void test6() {
    //pop head

    DoublyList *list = createDoublyList(sizeof(int));
    int a = 11;
    int b = 22;

    appendDoublyItem(list, &a);
    appendDoublyItem(list, &b);

    int *value = (int *)popDoublyItem(list, 0);
    assert(value != NULL);
    assert(*value == 11);

    free(value);

    assert(*(int *)list->head->data == 22);
    printf("test pop head - OK\n");
    freeDoublyList(list);
}


void test7() {
    //length

    DoublyList *list = createDoublyList(sizeof(int));
    int a = 1;
    int b = 2;
    int c = 3;

    appendDoublyItem(list, &a);
    appendDoublyItem(list, &b);
    appendDoublyItem(list, &c);

    assert(doublyListLength(list) == 3);

    printf("test length - OK\n");
    freeDoublyList(list);
}


int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();

    printf("eeee\n");

    return 0;
}