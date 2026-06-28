#ifndef DOUBLY_LIST
#define DOUBLY_LIST

#include <stdlib.h>
#include <stdbool.h>

typedef struct DBNode {
    void *data;
    struct DBNode *prev;
    struct DBNode *next;
} DBNode;

typedef struct DoublyList {
    DBNode *head;
    DBNode *end;
    size_t elem_size;
} DoublyList;

DoublyList *createDoublyList(size_t elem_size);
void appendDoublyItem(DoublyList *list, void *data);
void *popDoublyItem(DoublyList *list, size_t index);
long findDoublyItem(DoublyList *list, void *value, bool (*cmp)(void *, void *));
unsigned int doublyListLength(DoublyList *list);
void freeDoublyList(DoublyList *list);

#endif