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


double* fastlog_lookup  = NULL;
unsigned int fastlog_man_offset = 0;


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

