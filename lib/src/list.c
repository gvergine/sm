#include <stdlib.h>
#include <sm/list.h>

struct list_element
{
    void* data;
    list_element_t* next;
};


struct list {
    del_func delete;
    list_element_t* head;
};

list_t * list_new(del_func delete)
{
    list_t * l = malloc(sizeof(list_t));
    l->delete = delete;
    l->head = 0;
    return l;
}

void free_list_of_list_elements(list_element_t * first, del_func delete)
{
    if (first == 0) return;
    if (delete != 0) delete(first->data);
    free_list_of_list_elements(first->next, delete);
    free(first);
}

void list_delete(list_t* l)
{
    free_list_of_list_elements(l->head, l->delete);
    free(l);
}

list_element_t * list_insert(list_t* l, void* data)
{
    list_element_t * new_element = malloc(sizeof(list_element_t));
    new_element->data = data;
    new_element->next = 0;
    
    if (l->head == 0)
    {
        l->head = new_element;
        return new_element;
    }
    
    list_element_t* i = l->head;
    while (i->next != 0)
    {
        i = i->next;
    }
    
    i->next = new_element;
    return new_element;
}

list_element_t * list_find_next_element(list_t* l, list_element_t* node)
{
    if (node == 0) return l->head;
    return node->next;
}

void* list_get_data(list_element_t* node)
{
    return node->data;
}

void list_append(list_t* original, list_t* to_be_appended)
{
    list_element_t* iterator = 0;
    while((iterator = list_find_next_element(to_be_appended,iterator)) != 0)
    {
        void* data = list_get_data(iterator);
        list_insert(original, data);
    }
}

int list_count(list_t* l)
{
    int ret = 1;
    
    if (l->head == 0) return 0;

    list_element_t* i = l->head;
    while (i->next != 0)
    {
        i = i->next;
        ret++;
    }
    
    return ret;
}
