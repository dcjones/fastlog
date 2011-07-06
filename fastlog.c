 /*
 *Copyright (C) 2011 by Daniel C. Jones <dcjones@cs.washington.edu>
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in
 *all copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
 */



#include "fastlog.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>


#ifndef M_LN2
#define M_LN2 0.69314718055994530942
#endif


typedef union
{
    double   f;
    uint64_t ui;
    int64_t  si;
} fi_t;

static const uint64_t fastlog_exp_mask = 0x7ff0000000000000;
static const uint64_t fastlog_man_mask = 0x000fffffffffffff;


double* fastlog_lookup  = NULL;
uint64_t fastlog_man_offset = 0;


void fastlog_init(int prec)
{
    if (prec < 1 || prec > 52) {
        abort();
    }

    free(fastlog_lookup);

    uint64_t n = 1 << prec; // 2^prec
    fastlog_lookup = malloc(n * sizeof(double));

    if (fastlog_lookup == NULL) {
        abort();
    }

    fastlog_man_offset = 52 - prec;
    uint64_t x;
    fi_t y;
    for (x = 0; x < n; ++x) {
        y.ui = ((uint64_t) 1023 << 52) | (x << fastlog_man_offset);
        fastlog_lookup[x] = log(y.f);
    }
    
}


void fastlog_free()
{
    free(fastlog_lookup); fastlog_lookup = NULL;
}


double fastlog(double x)
{
    fi_t y;
    y.f = x;
    register const int exp  = ((int) (y.si >> 52)) - 1023;
    register const uint64_t man  = (y.ui & fastlog_man_mask) >> fastlog_man_offset;

    return M_LN2 * (double) exp + fastlog_lookup[man];
}


#if 0
double fastlog_asm(double x)
{
    static const double ln2 = M_LN2;

    /* GCC assembly */
    asm(
        /* extract x's exponent into eax */
        "movq %1, %%rax\n\t"
        "shrq $52, %%rax\n\t"
        "leal -1023(%%rax), %%eax\n\t"

        /* multiply exponent by ln(2) */
        "movsd %2, %%xmm0\n\t"
        "cvtsi2sd %%eax, %%xmm1\n\t"
        "mulsd %%xmm0, %%xmm1\n\t"

        /* extract x's mantissa */
        "movq %1, %%rax\n\t"
        "andq %3, %%rax\n\t"
        "mov %4, %%cl\n\t"
        "shrq %%cl, %%rax\n\t"

        /* lookup */
        "leaq (%5, %%rax, 8), %5\n\t"
        "movq (%5), %%xmm0\n\t"

        /* add and return */
        "addsd %%xmm0, %%xmm1\n\t"
        "movq %%xmm1, %0\n\t"

            : "=m" (x)
            : "m" (x),                   // 1
              "m" (ln2),                 // 2
              "r" (fastlog_man_mask),    // 3
              "m" (fastlog_man_offset),  // 4
              "r" (fastlog_lookup)       // 5
            : "%rax", "%xmm0", "%xmm1");

    return x;
}
#endif


