#ifndef __LIST_H
#define __LIST_H

#include <system.h>

typedef struct node {
    struct node* prev;
    struct node* next;
    void* data;
} __attribute__((packed)) node_t;

typedef struct {
    node_t* head;
    node_t* tail;
    size_t length;
} __attribute__((packed)) list_t;

#define foreach(i, list) node_t* i = NULL; for (i = list->head; i != NULL; i = i->next)

list_t* list_create();
void list_append(list_t* list, void* data);
void list_prepend(list_t* list, void* data);

void list_push(list_t* list, void* data);
void* list_pop(list_t* list);
void list_enqueue(list_t* list, void* data);
void* list_dequeue(list_t* list);

void list_free(list_t* list);
void list_destroy(list_t* list);

void list_remove(list_t* list, void* data);

#endif // __LIST_H
