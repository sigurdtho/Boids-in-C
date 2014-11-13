/* Author: Sigurd Thomassen <sigurd14@gmail.com> */

#include <stdlib.h>
#include "list.h"

typedef struct listnode listnode_t;

struct listnode {
    listnode_t *next;
    listnode_t *prev;
    void *elem;
};

struct list {
    listnode_t *head;
    listnode_t *tail;
    int size;
};

struct list_iter {
    listnode_t *node;
};

static listnode_t *newnode(void *elem) {
    listnode_t *node = malloc(sizeof(listnode_t));
    if (node == NULL) {
        goto error;
    }
    node->next = NULL;
    node->prev = NULL;
    node->elem = elem;

    return node;
error:
    return NULL;
}

list_t *list_create(void) {
    list_t *list = malloc(sizeof(list_t));
    if (list == NULL) {
        goto error;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
error:
    return NULL;
}

void list_destroy(list_t *list) {
    listnode_t *node = list->head;
    while (node != NULL) {
        listnode_t *tmp = node;
        node = node->next;
        free(tmp);
    }
    free(list);
}

int list_size(list_t *list) {
    return list->size;
}

void list_addfirst(list_t *list, void *elem) {
    listnode_t *node = newnode(elem);
    if (list->head == NULL) {
        list->head = list->tail = node;
    }
    else {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
    }
    list->size++;
}

void list_addlast(list_t *list, void *elem) {
    listnode_t *node = newnode(elem);
    if (list->head == NULL) {
        list->head = list->tail = node;
    }
    else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    list->size++;
}

/*
int list_contains(list_t *list, void *elem) {
    listnode_t *node = list->head;
    while (node != NULL) {
        if (list->cmpfunc(elem, node->elem) == 0) {
            return 1;
        }
        node = node->next;
    }
}
*/

list_iter_t *list_createiter(list_t *list) {
    list_iter_t *iter = malloc(sizeof(list_iter_t));
    if (iter == NULL) {
        goto error;
    }
    iter->node = list->head;

    return iter;
error:
    return NULL;
}

void list_destroyiter(list_iter_t *iter) {
    free(iter);
}

int list_hasnext(list_iter_t *iter) {
    if (iter->node == NULL) {
        return 0;
    }
    else {
        return 1;
    }
}

void *list_next(list_iter_t *iter) {
    if (iter->node == NULL) {
        goto error;
    }
    else {
        void *elem = iter->node->elem;
        iter->node = iter->node->next;
        return elem;
    }

error:
    return NULL;
}

void list_passnext(list_iter_t *iter) {
    iter->node = iter->node->next;
}

void list_set(list_iter_t *iter, boid_t *boid) {
    void *tmp;
    tmp = iter->node->elem;
    iter->node->elem = boid;
    destroy_boid(tmp);
}
