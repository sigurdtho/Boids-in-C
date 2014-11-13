/* Author Sigurd Thomassen <sigurd14@gmail.com> */
#include "boid.h"

struct list;
typedef struct list list_t;

list_t *list_create(void);

void list_destroy(list_t *list);

int list_size(list_t *list);

void list_addfirst(list_t *list, void *elem);

void list_addlast(list_t *list, void *elem);

struct list_iter;
typedef struct list_iter list_iter_t;

list_iter_t *list_createiter(list_t *list);

void list_destroyiter(list_iter_t *iter);

int list_hasnext(list_iter_t *iter);

void *list_next(list_iter_t *iter);

void list_passnext(list_iter_t *iter);

void list_set(list_iter_t *iter, boid_t *boid);
