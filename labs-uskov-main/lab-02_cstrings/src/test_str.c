#include <assert.h>
#include "str.h"
#include "test_str.h"
#define BUF_SIZE 255

void test_my_strlen()
{
    assert(my_strlen("Hello") == 5);
    assert(my_strlen("") == 0);
    assert(my_strlen("x") == 1);
    assert(my_strlen("HelloHELLOhello") == 15);
    char long_str[BUF_SIZE];
    for (int i = 0; i < BUF_SIZE - 1; ++i)
        long_str[i] = 'x';
    long_str[BUF_SIZE - 1] = '\0';
    assert(my_strlen(long_str) == BUF_SIZE - 1);
}

void test_my_strcmp()
{
    assert(!my_strcmp("", ""));
    assert(!my_strcmp("a", "a"));
    assert(my_strcmp("", "a") < 0);
    assert(my_strcmp("ads", "") > 0);
    assert(!my_strcmp("sdasd", "sdasd"));
    assert(my_strcmp("sdasd", "sdasdaxonw") < 0);
    assert(my_strcmp("sbdskncoenlgnson.c", "fnosellgbsoclnieofm clme") > 0);
    assert(!my_strcmp("dnksjcsljnsoie.snes", "dnksjcsljnsoie.snes"));
    assert(my_strcmp("dnksjcsljnsoie.snes", "anksjcsljnsoie.snes") > 0);
    char str1[BUF_SIZE], str2[BUF_SIZE];
    for (int i = 0; i < BUF_SIZE - 1; ++i)
    {
        str1[i] = 'x';
        str2[i] = 'x';
    }
    str1[BUF_SIZE - 1] = '\0';
    str2[BUF_SIZE - 1] = '\0';
    assert(!my_strcmp(str1, str2));
}

void test_my_strcpy()
{
    char str1[BUF_SIZE], str2[BUF_SIZE];
    for (int i = 0; i < BUF_SIZE - 1; ++i)
    {
        str2[i] = '\0';
        assert(!my_strcmp(my_strcpy(str1, str2), str2));
        str2[i] = 'a';
    }
    char buf[BUF_SIZE];
    assert(!my_strcmp(my_strcpy(buf, "Hello"), "Hello"));
    assert(!my_strcmp(my_strcpy(buf, "world!"), "world!"));
    assert(!my_strcmp(my_strcpy(buf, "Hello world"), "Hello world"));
    assert(!my_strcmp(my_strcpy(buf, ""), ""));
}

void test_my_strcat()
{
    char empty1[] = {'\0', 'a'}, empty2[] = {'\0'};
    assert(!my_strcmp(my_strcat(empty1, empty2), ""));
    char a[] = {'a', '\0', '*'};
    char b[] = {'b', '\0'};
    assert(!my_strcmp(my_strcat(a, b), "ab"));
    char str1[BUF_SIZE] = "Hello", str2[BUF_SIZE] = "world!";
    assert(!my_strcmp(my_strcat(empty1, str1), str1));
    assert(!my_strcmp(my_strcat(str1, empty2), str1));
    assert(my_strcmp(my_strcat(str1, str2), ""));
}