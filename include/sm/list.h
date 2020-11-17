#ifndef LIST_H
#define LIST_H

typedef void (*del_func)(void*);

typedef struct list_element list_element_t;
typedef struct list list_t;

list_t * list_new(del_func);
void list_delete(list_t*);
list_element_t * list_insert(list_t*, void*);
list_element_t * list_find_next_element(list_t*, list_element_t*);
void* list_get_data(list_element_t*);
void list_append(list_t*, list_t*);

#endif
