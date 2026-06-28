#include "generic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DoublyList *createDoublyList(size_t elem_size)
{
    if (elem_size == 0) {
        printf("Error: NULL elem_size in createDoublyList");
        return NULL;
    }

    DoublyList *list = malloc(sizeof(DoublyList));
    if (!list) {
        return NULL;
    }

    list->head = NULL;
    list->end = NULL;
    list->elem_size = elem_size;

    return list;
}

void appendDoublyItem(DoublyList *list, void *data)
{
    if (!list || !data) {
        printf("Error: NULL list/data in appendDoublyItem");
        return;
    }

    DBNode *new_node = malloc(sizeof(DBNode));
    if (!new_node) {
        printf("Error: create node in appendDoublyItem");
        return;
    }

    new_node->data = malloc(list->elem_size);
    if (!new_node->data) {
        printf("Error: create node->data in appendDoublyItem");
        free(new_node);
        return;
    }

    memcpy(new_node->data, data, list->elem_size);
    new_node->next = NULL;
    new_node->prev = list->end;

    if (list->end) {
        list->end->next = new_node;
    } else {
        list->head = new_node;
    }
    list->end = new_node;

}

void *popDoublyItem(DoublyList *list, size_t index)
{
    if (!list || !list->head) {
        printf("Error: NULL list/list->head in popDoublyItem");
        return NULL;
    }

    DBNode *current = list->head;
    size_t i = 0;

    while (current && i < index) {
        current = current->next;
        i++;
    }

    if (!current) {
        return NULL;
    }

    void *copy = malloc(list->elem_size);
    if (!copy) {
        return NULL;
    }

    memcpy(copy, current->data, list->elem_size);

    if (current->prev) {
        current->prev->next = current->next;
    } else {
        list->head = current->next;
    }

    if (current->next) {
        current->next->prev = current->prev;
    } else {
        list->end = current->prev;
    }

    free(current->data);
    free(current);

    return copy;
}

long findDoublyItem(DoublyList *list, void *value, bool (*cmp)(void *, void *))
{
    if (!list || !value || !cmp) {
        printf("Error: NULL data in findDoublyItem"); 
        return -1;
    }

    DBNode *current = list->head;
    long index = 0;

    while (current) {
        if (cmp(current->data, value)) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}

unsigned int doublyListLength(DoublyList *list)
{
    if (!list) {
        printf("Error: NULL list in doublyListLength"); 
        return 0;
    }

    unsigned int count = 0;
    DBNode *current = list->head;

    while (current) {
        count++;
        current = current->next;
    }

    return count;
}

void freeDoublyList(DoublyList *list) {
    if (!list) {
        return;
    }

    DBNode *current = list->head;

    while (current) {
        DBNode *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }

    free(list);
}