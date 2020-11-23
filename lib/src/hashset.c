#include <stdlib.h>
#include "hashset.h"

struct hashset_element
{
    int hash;
    void* data;
    struct hashset_element* next;
};


struct hashset {
    cmp_func cmp;
    hash_func hash;
    del_func delete;
    hashset_element_t** table;
    size_t table_size;
};

hashset_t * hashset_new(cmp_func cmp, hash_func hash, del_func delete,
                        size_t table_size)
{
    hashset_t * set = malloc(sizeof(hashset_t));
    set->cmp = cmp;
    set->hash = hash;
    set->delete = delete;
    set->table_size = table_size;
    set->table = calloc(table_size,sizeof(hashset_element_t*));
    return set;
}

void free_list_of_hashset_elements(hashset_element_t * first, del_func delete)
{
    if (first == 0) return;
    if (delete != 0) delete(first->data);
    free_list_of_hashset_elements(first->next, delete);
    free(first);
}

void hashset_delete(hashset_t* set)
{
    int i;
    for (i = 0; i < set->table_size; i++)
    {
        hashset_element_t * n = set->table[i];
        if (n == 0) continue;
        else
        {
            free_list_of_hashset_elements(n,set->delete);            
        }
    }
    free(set->table);
    free(set);
}

/*hashset_element_t* append_node(hashset_element_t* n, void* data) 
{
    hashset_element_t* node = malloc(sizeof(hashset_element_t));
    node->data = data;
    node->next = 0;
    node->hash = n->hash;
    n->next = node;
    return node;
}*/

int hashset_contains(hashset_t* set, const void* data)
{
    int hash = set->hash(data);
    hashset_element_t* i = set->table[hash];
    while (i != 0)
    {
        if (set->cmp(data, i->data)) return 1;
        i = i->next;
    }
    return 0;
}

hashset_element_t* hashset_insert(hashset_t* set, const void* data)
{
    hashset_element_t* node = 0;
    if (hashset_contains(set, data)) return 0;
    
    int h = set->hash(data);

    node = malloc(sizeof(hashset_element_t));
    node->data = (void*)data;
    node->next = set->table[h];
    node->hash = h;
    set->table[h] = node;
    return node;
}


hashset_element_t * find_first_node(hashset_t* set)
{
    hashset_element_t * ret = 0;
    int i;
    for (i = 0; i < set->table_size; i++)
    {
        hashset_element_t * header = set->table[i];
        if (header == 0) continue;
        ret = header;
        break;
    }
    return ret;
}

hashset_element_t * hashset_find_next_element(hashset_t * set,
                                              hashset_element_t * cont)
{
    if (cont == 0) return find_first_node(set);
    if (cont->next != 0) return cont->next;
    
    int i;
    for (i = cont->hash+1; i<set->table_size; i++)
    {
        hashset_element_t* header = set->table[i];
        if (header == 0) continue;
        return header;
    }
    
    return 0;
}

hashset_element_t * hashset_find_element(hashset_t * set, const void * data)
{
    int hash = set->hash(data);
    hashset_element_t* i = set->table[hash];
    while (i != 0)
    {
        if (set->cmp(data, i->data)) break;
        i = i->next;
    }
    return i;
}

void* hashset_get_data(hashset_element_t* node)
{
    if (node == 0) return 0;
    return node->data;
}

