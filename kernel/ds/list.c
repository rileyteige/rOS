#include <list.h>
#include <system.h>

void list_delete(list_t* list, node_t* node);

/*
 * Creates a list.
 */
list_t* list_create()
{
    list_t* list = kmalloc(sizeof(list_t));
    
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    
    return list;
}

void list_free(list_t* list)
{
    node_t* n = list->head;
    while (n) {
        node_t* next = n->next;
        kfree(n);
        n = next;
    }
    kfree(list);
}

void list_free_elements(list_t* list)
{
    foreach(node, list) {
        kfree(node->data);
    }
}

void list_destroy(list_t* list)
{
    list_free_elements(list);
    list_free(list);
}

void list_empty(list_t* list)
{
    if (!list->length)
        return;
    
    void* data = NULL;
    while ((data = list_pop(list)))
    {
        kfree(data);
    }
}

void* list_pop(list_t* list)
{
    if (!list->tail)
        return NULL;
    
    void* data = list->tail->data;
    list_delete(list, list->tail);
    return data;
}

void list_push(list_t* list, void* data)
{
    list_append(list, data);
}

void list_enqueue(list_t* list, void* data)
{
    list_append(list, data);
}

void* list_dequeue(list_t* list)
{
    if (!list->head)
        return NULL;

    void* data = list->head->data;
    list_delete(list, list->head);
    return data;
}

/*
 * Appends data to the end of the list.
 */
void list_append(list_t* list, void* data)
{
    assert(list != NULL);
    
    node_t* node = kmalloc(sizeof(node_t));
    
    node->next = NULL;
    node->prev = NULL;
    node->data = data;
    
    if (!list->tail) {
        list->head = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
    }
    
    list->tail = node;
    list->length++;
}

/*
 * Prepends data to the front of the list.
 */
void list_prepend(list_t* list, void* data)
{
    assert(list != NULL);
    
    node_t* node = kmalloc(sizeof(node_t));
    
    node->next = NULL;
    node->prev = NULL;
    node->data = data;
    
    if (!list->head) {
        list->tail = node;
    } else {
        list->head->prev = node;
        node->next = list->head;
    }
    
    list->head = node;
    list->length++;
}

/*
 * Deletes a node from the list
 */
void list_delete(list_t* list, node_t* node)
{
    if (node == list->head)
        list->head = node->next;
    if (node == list->tail)
        list->tail = node->prev;
    if (node->next)
        node->next->prev = node->prev;
    if (node->prev)
        node->prev->next = node->next;
    
    node->prev = NULL;
    node->next = NULL;
    kfree(node);
    list->length--;
}

/*
 * Finds the provided data in the list and
 * removes the respective node.
 */
void list_remove(list_t* list, void* data)
{
    foreach(node, list) {
        if (node->data == data) {
            list_delete(list, node);
            break;
        }
    }
}
