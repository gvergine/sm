#ifndef HASHSET_H
#define HASHSET_H

#include <stddef.h>

typedef int (*cmp_func)(const void*,const void*);
typedef int (*hash_func)(const void*);
typedef void (*del_func)(void*);

typedef struct hashset_element hashset_element_t;
typedef struct hashset hashset_t;

hashset_t * hashset_new(cmp_func, hash_func, del_func, size_t);
void hashset_delete(hashset_t*);
hashset_element_t * hashset_insert(hashset_t*, const void*);
hashset_element_t * hashset_find_next_element(hashset_t*, hashset_element_t*);
hashset_element_t * hashset_find_element(hashset_t*, const void*);
void* hashset_get_data(hashset_element_t*);

#endif
