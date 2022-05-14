#include "clist.h"

#ifndef _POINT_LIST_H_
#define _POINT_LIST_H_

typedef struct point_t
{
    int x, y;
    intrusive_node node;
} point_t;

void add_point(intrusive_list *, int, int);
void print_point(intrusive_node *, void *format);
void show_all_points(intrusive_list *, char *format);
void del_point(intrusive_node *);
int cmp_node(intrusive_node *, int, int);
void remove_point(intrusive_list *, int, int);
void remove_all_points(intrusive_list *);

#endif