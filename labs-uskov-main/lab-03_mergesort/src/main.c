#include "mergesort.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
typedef void (*modifier_t)(char *, const char *);
typedef void (*printer_t)(void *);

void into_int(char *dst, const char *a)
{
    *(int *)dst = atoi(a);
}
void into_char(char *dst, const char *a)
{
    *dst = *a;
}
int my_int_cmp(const void *num1, const void *num2)
{
    return *(int *)num1 - *(int *)num2;
}
int my_char_cmp(const void *ch1, const void *ch2)
{
    return *(char *)ch1 - *(char *)ch2;
}
int str_cmp(const void *a, const void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}
char *make_elements_array(char **str, size_t elements, size_t element_size, modifier_t modifier)
{
    if (!modifier)
        return (char *)str;
    char *data = malloc(elements * element_size);
    assert(data && "Error: memory allocation failed.\n");
    for (size_t i = 0; i < elements; ++i)
    {
        modifier((char *)(data + i * element_size), str[i]);
    }
    return data;
}

void int_printer(void *data)
{
    printf("%d", *((int *)data));
}
void char_printer(void *data)
{
    printf("%c", *((char *)data));
}
void str_printer(void *data)
{
    printf("%s", *((char **)data));
}

void print_result(void *data, size_t elements, size_t element_size, printer_t printer)
{
    for (size_t i = 0; i < elements - 1; ++i)
    {
        printer((char *)data + i * element_size);
        printf(" ");
    }
    printer((char *)data + (elements - 1) * element_size);
    printf("\n");
}
int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("\n");
        return 0;
    }
    size_t element_size, elements = argc - 2;
    comp_t comparator;
    char *data;
    printer_t printer;
    modifier_t modifier;
    if (!strcmp(argv[1], "int"))
    {
        element_size = sizeof(int);
        comparator = my_int_cmp;
        modifier = into_int;
        printer = int_printer;
    }
    else if (!strcmp(argv[1], "char"))
    {
        element_size = sizeof(char);
        comparator = my_char_cmp;
        modifier = into_char;
        printer = char_printer;
    }
    else if (!strcmp(argv[1], "str"))
    {
        element_size = sizeof(char *);
        comparator = str_cmp;
        modifier = NULL;
        printer = str_printer;
    }
    data = make_elements_array(argv + 2, elements, element_size, modifier);
    assert(!mergesort(data, elements, element_size, comparator) &&
           "Error: memory allocation failed.\n");
    print_result(data, elements, element_size, printer);
    if (strcmp(argv[1], "str"))
        free(data);
    return 0;
}
