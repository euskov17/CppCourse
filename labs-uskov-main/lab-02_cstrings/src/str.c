#include <stddef.h>

#include "str.h"

size_t my_strlen(char *str)
{
    char *p = str;
    while (*p++)
        ;
    return p - str - 1;
}

char *my_strcat(char *str1, char *str2)
{
    char *p = str1 + my_strlen(str1);
    char *k = str2;
    my_strcpy(p, k);
    return str1;
};
int my_strcmp(char *str1, char *str2)
{
    char *p1 = str1;
    char *p2 = str2;
    if (!*p1)
        return (!*p2) ? 0 : -1;
    while ((*p1++ == *p2++ && *p1))
        ;
    if (*(p1 - 1) == *(p2 - 1) && !*p2 && !*p1)
        return 0;
    return (*(p1 - 1) > *(p2 - 1)) ? 1 : -1;
}
char *my_strcpy(char *str1, char *str2)
{
    char *p1 = str1;
    char *p2 = str2;
    while ((*p1++ = *p2++))
        ;
    *p1 = '\0';
    return str1;
};