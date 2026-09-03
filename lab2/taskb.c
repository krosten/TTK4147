#include <stdio.h>
#include "array.h"

int main(void)
{
    Array a = array_new(4);        /* liten kapasitet med vilje */

    for (long i = 0; i < 100; i++) {
        array_insertBack(&a, i);
        printf("i = %ld, length = %ld, capacity = %ld\n",
               i, array_length(a), a.capacity);
    }

    array_print(a);
    array_destroy(a);
    return 0;
}