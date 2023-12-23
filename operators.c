#include "ast.h"

int *mult(int *a, int *b)
{
    int * v = malloc (sizeof(int));
    *v = *a * *b;
    return v;
}

int *add(int *a, int *b) {

    int * v = malloc (sizeof(int));
    *v = *a + *b;
    return v;
}