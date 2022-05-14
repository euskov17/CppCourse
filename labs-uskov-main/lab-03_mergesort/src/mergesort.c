#include "mergesort.h"
#include <stdlib.h>
void my_memcpy(void *dest, void *start, size_t count)
{
    for (char *p_start = (char *)start, *p_dest = (char *)dest;
         (size_t)(p_start - (char *)start) < count; *p_dest++ = *p_start++)
        ;
}

int merge(void *array, size_t separator, size_t elements, size_t element_size, comp_t cmp)
{
    char *new_array = malloc(elements * element_size);
    if (!new_array)
        return -1;
    char *p1 = array, *p2 = array + separator * element_size, *p_cur = new_array;
    char *array_c = (char *)array;
    while ((p1 - array_c < (long int)(separator * element_size)) &&
           (p2 - array_c < (long int)(elements * element_size)))
    {
        my_memcpy(p_cur, (cmp(p1, p2) < 0) ? p1 : p2, element_size);
        p_cur += element_size;
        if (cmp(p1, p2) < 0)
            p1 += element_size;
        else
            p2 += element_size;
    }
    my_memcpy(p_cur, p1, (long int)(separator * element_size) - (p1 - array_c));
    my_memcpy(p_cur, p2, (long int)(elements * element_size) - (p2 - array_c));
    my_memcpy(array, new_array, elements * element_size);
    free(new_array);
    return 0;
}

int mergesort(void *array, size_t elements, size_t element_size, comp_t cmp)
{
    if (elements < 2)
        return 0;
    size_t mid = elements / 2;
    if (mergesort(array, mid, element_size, cmp))
        return -1;
    if (mergesort(array + element_size * mid, elements - mid, element_size, cmp))
        return -1;
    if (merge(array, mid, elements, element_size, cmp))
        return -1;
    return 0;
}
