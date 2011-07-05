/*
 * fastlog
 * -------
 *  This is a very simple (natural) logorithm function. The 'fastlog' function
 *  here computes the same value as the 'log' function from math.h, but with
 *  adjustable accuaracy, and potentially much faster. This is useful, for
 *  example, in mixture models that involve computing a log-likelihood function
 *  many times.
 *
 *
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
 *
 */

#ifndef FASTLOG_H
#define FASTLOG_H


#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdint.h>


void fastlog_init(int prec);
void fastlog_free();

typedef union
{
    double   f;
    uint64_t ui;
    int64_t  si;
} fi_t;

extern double* fastlog_lookup;
extern unsigned int fastlog_man_offset;
static const uint64_t fastlog_exp_mask = 0x7ff0000000000000;
static const uint64_t fastlog_man_mask = 0x000fffffffffffff;


static inline double fastlog(double x)
{
    register const int exp  = ((int) (((fi_t)x).si >> 52)) - 1023;
    register const uint64_t man  = (((fi_t)x).ui & fastlog_man_mask) >> fastlog_man_offset;

    return M_LN2 * (double) exp + fastlog_lookup[man];
}

#if 0
inline static double fastlog(double x)
{
    int exp;
    fi_t y;
    y.f = frexp(x, &exp);
    //register const int64_t  exp  = (((fi_t)x).si >> 52) - 1023;
    //register const uint64_t man  = (((fi_t)x).ui & fastlog_man_mask) >> fastlog_man_offset;

    return M_LN2 * (double) exp + fastlog_lookup[y.ui];
}
#endif


#ifdef __cplusplus
}
#endif

#endif


