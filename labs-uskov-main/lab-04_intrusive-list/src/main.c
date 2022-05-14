#include <stdio.h>
#include <string.h>
#include "clist.h"
#include "point_list.h"

int main()
{
    intrusive_list *l = malloc(sizeof(intrusive_list));
    init_list(l);
    char str[239], first_arg[239], second_arg[239];
    scanf("%239s", str);
    while (strcmp(str, "exit"))
    {
        if (!strcmp(str, "add"))
        {
            scanf("%239s", first_arg);
            scanf("%239s", second_arg);
            add_point(l, atoi(first_arg), atoi(second_arg));
        }
        else if (!strcmp(str, "rm"))
        {
            scanf("%239s", first_arg);
            scanf("%239s", second_arg);
            remove_point(l, atoi(first_arg), atoi(second_arg));
        }
        else if (!strcmp(str, "len"))
        {
            printf("%d\n", get_length(l));
        }
        else if (!strcmp(str, "print"))
        {
            show_all_points(l, "(%d %d)");
        }
        else if (!strcmp(str, "rma"))
        {
            remove_all_points(l);
        }
        else
        {
            printf("Unknown command\n");
        }
        scanf("%239s", str);
    }
    remove_all_points(l);
    free(l);
    return 0;
}
