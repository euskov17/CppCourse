#include "clist.h"

void init_list(intrusive_list *new_list)
{
    new_list->head.next = NULL;
    new_list->head.prev = NULL;
}

void add_node(intrusive_list *list, intrusive_node *new_node)
{
    intrusive_node *next = list->head.next, *prev = &list->head;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
    if (next)
        next->prev = new_node;
}

void remove_node(intrusive_node *del_node)
{
    if (!del_node || !del_node->prev)
        return;
    intrusive_node *prev = del_node->prev, *next = del_node->next;
    prev->next = del_node->next;
    if (next)
        next->prev = prev;
}

int get_length(intrusive_list *list)
{
    intrusive_node *cur_ptr = &list->head;
    int length = 0;
    while ((cur_ptr = cur_ptr->next))
        length++;
    return length;
};

void apply_data(intrusive_list *list, void (*foo)(intrusive_node *, void *), void *data)
{
    intrusive_node *cur_ptr = list->head.next;
    while (cur_ptr)
    {
        intrusive_node *p = cur_ptr;
        cur_ptr = cur_ptr->next;
        foo(p, data);
    }
}

void apply(intrusive_list *list, void (*foo)(intrusive_node *))
{
    intrusive_node *cur_ptr = list->head.next;
    while (cur_ptr)
    {
        intrusive_node *p = cur_ptr;
        cur_ptr = cur_ptr->next;
        foo(p);
    }
}

void apply_rev(intrusive_list *list, void (*foo)(intrusive_node *))
{
    intrusive_node *cur_ptr = &list->head;
    while (cur_ptr->next)
        cur_ptr = cur_ptr->next;
    while (cur_ptr != &list->head)
    {
        intrusive_node *p = cur_ptr;
        cur_ptr = cur_ptr->prev;
        foo(p);
    }
}

void apply_data_rev(intrusive_list *list, void (*foo)(intrusive_node *, void *), void *data)
{
    intrusive_node *cur_ptr = &list->head;
    while (cur_ptr->next)
        cur_ptr = cur_ptr->next;
    while (cur_ptr != &list->head)
    {
        intrusive_node *p = cur_ptr;
        cur_ptr = cur_ptr->prev;
        foo(p, data);
    }
}
