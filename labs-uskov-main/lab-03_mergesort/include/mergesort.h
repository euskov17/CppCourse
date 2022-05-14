#ifndef _MEGRESORT_H_
#define _MERGESORT_H
#include <stddef.h>

typedef int (*comp_t)(const void *, const void *);
int mergesort(void *array, size_t elements, size_t element_size, comp_t cmp);
void my_memcpy(void *dest, void *start, size_t count);

#endif