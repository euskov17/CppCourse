#include <stdio.h>
#include <assert.h>
#include "clist.h"
#include "point_list.h"

void add_point(intrusive_list *list, int x, int y)
{
    point_t *new_point = malloc(sizeof(point_t));
    assert(new_point && "memory not allocate for new_point");
    new_point->x = x;
    new_point->y = y;
    add_node(list, &new_point->node);
}

void print_point(intrusive_node *print_node, void *format)
{
    point_t *point = container_of(print_node, point_t, node);
    printf(format, point->x, point->y);
}

void show_all_points(intrusive_list *list, char *format)
{
    apply_data_rev(list, print_point, format);
    printf("\n");
}

int cmp_node(intrusive_node *cur_node, int x, int y)
{
    point_t *del_p = container_of(cur_node, point_t, node);
    return ((del_p->x == x && del_p->y == y) ? 0 : 1);
}

void check_del_p(intrusive_node *cur_node, void *xy)
{
    if (!cmp_node(cur_node, *(int *)xy, *((int *)xy + 1)))
        del_point(cur_node);
}

void remove_point(intrusive_list *list, int x, int y)
{
    int xy[2] = {x, y};
    apply_data(list, check_del_p, xy);
}

void del_point(intrusive_node *del_node)
{

    if (!del_node || !del_node->prev)
        return;
    point_t *del_p = container_of(del_node, point_t, node);
    remove_node(del_node);
    free(del_p);
}

void remove_all_points(intrusive_list *list)
{
    apply_rev(list, del_point);
}