#include <stdlib.h>
#include <stddef.h>
#define container_of(ptr, type, member) (type *)((char *)(ptr)-offsetof(type, member))

#ifndef CLIST_H_
#define CLIST_H_

typedef struct intrusive_node
{
    struct intrusive_node *next;
    struct intrusive_node *prev;
} intrusive_node;

typedef struct intrusive_list
{
    struct intrusive_node head;
} intrusive_list;

void init_list(intrusive_list *new_list);
void add_node(intrusive_list *list, intrusive_node *new_node);
void remove_node(intrusive_node *del_node);
int get_length(intrusive_list *list);
void apply_data(intrusive_list *list, void (*foo)(intrusive_node *, void *), void *);
void apply_data_rev(intrusive_list *list, void (*foo)(intrusive_node *, void *), void *);
void apply(intrusive_list *, void (*foo)(intrusive_node *));
void apply_rev(intrusive_list *, void (*foo)(intrusive_node *));

#endif
