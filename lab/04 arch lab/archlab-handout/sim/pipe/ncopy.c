#include <stdio.h>
#include <stdlib.h>

typedef int word_t;

word_t* src, * dst;

/* $begin ncopy */
/*
 * ncopy - copy src to dst, returning number of positive ints
 * contained in src array.
 */
word_t ncopy(word_t len)
{
    word_t val = 0;
    while (len >= 4) {
        *(dst) = *(src);
        *(dst + 1) = *(src + 1);
        *(dst + 2) = *(src + 2);
        *(dst + 3) = *(src + 3);
        len -= 4;
        src += 4;
        dst += 4;
    }
    switch(len) {
    case 3: *(dst + 2) = *(src + 2); if (*(src + 2) > 0) val++;
    case 2: *(dst + 1) = *(src + 1); if (*(src + 1) > 0) val++;
    case 1: *(dst)     = *(src);     if (*(src) > 0)     val++;
    case 0:
    default:
    }
    return val;
}
/* $end ncopy */

int main()
{
    word_t i, count;

    for (i=0; i<8; i++)
	src[i]= i+1;
    count = ncopy(8);
    printf ("count=%d\n", count);
    exit(0);
}


