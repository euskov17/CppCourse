#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "clist.h"
#include "point_list.h"

unsigned char *make_bytes(int x, unsigned char *buffer)
{
    for (int i = 0; i < 3; i++)
        buffer[i] = (unsigned char)((x >> 8 * i) & 0xFF);
    return buffer;
}

int make_int(unsigned char *buffer)
{
    int n = 0;
    for (int i = 0; i < 3; i++)
        n |= ((int)buffer[i]) << 8 * i;
    if (buffer[2] & 0x80)
        n |= 0xFF000000;
    return n;
}

void load_text(FILE *fin, intrusive_list *list)
{
    int x, y;
    while (fscanf(fin, "%d %d", &x, &y) == 2)
        add_point(list, x, y);
}

void load_bin(FILE *fin, intrusive_list *list)
{
    unsigned char buffer[6];
    while (fread(buffer, sizeof(unsigned char), 6, fin) == 6)
        add_point(list, make_int(&buffer[0]), make_int(&buffer[3]));
}

void save_point_bin(intrusive_node *print_node, void *f)
{
    FILE *file = (FILE *)f;
    point_t *point = container_of(print_node, point_t, node);
    unsigned char buffer[3];
    fwrite(make_bytes(point->x, buffer), sizeof(unsigned char), 3, file);
    fwrite(make_bytes(point->y, buffer), sizeof(unsigned char), 3, file);
}

void save_bin(FILE *file, intrusive_list *l)
{
    apply_data_rev(l, save_point_bin, file);
}

void save_point_text(intrusive_node *print_node, void *f)
{
    FILE *file = (FILE *)f;
    point_t *point = container_of(print_node, point_t, node);
    fprintf(file, "%d %d\n", point->x, point->y);
}
void save_text(FILE *file, intrusive_list *l)
{
    apply_data_rev(l, save_point_text, file);
}

int main(int argc, char *argv[])
{
    (void)argc;
    intrusive_list *list = malloc(sizeof(intrusive_list));
    assert(list && "list not allocate\n");
    init_list(list);
    if (!strcmp(argv[1], "loadbin"))
    {
        FILE *fin = fopen(argv[2], "r");
        assert(fin && "file not open");
        load_bin(fin, list);
        fclose(fin);
    }
    else if (!strcmp(argv[1], "loadtext"))
    {
        FILE *fin = fopen(argv[2], "r");
        assert(fin && "file not open");
        load_text(fin, list);
        fclose(fin);
    }
    if (!strcmp(argv[3], "savebin"))
    {
        FILE *fout = fopen(argv[4], "w");
        assert(fout && "file not open");
        save_bin(fout, list);
        fclose(fout);
    }
    else if (!strcmp(argv[3], "savetext"))
    {
        FILE *fout = fopen(argv[4], "wb");
        assert(fout && "file not open");
        save_text(fout, list);
        fclose(fout);
    }
    else if (!strcmp(argv[3], "count"))
    {
        printf("%d\n", get_length(list));
    }
    else if (!strcmp(argv[3], "print"))
    {
        show_all_points(list, argv[4]);
    }
    remove_all_points(list);
    free(list);
    return 0;
}