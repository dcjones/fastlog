
#include "fastlog.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


double time_log(double (*logfunc)(double), double* xs, size_t n)
{
    clock_t t0 = clock();

    size_t i;
    for (i = 1; i < n; ++i) {
        xs[0] = logfunc(xs[i]);
    }

    clock_t t1 = clock();
    return (double) (t1 - t0) / (double) CLOCKS_PER_SEC;
}


void f(double x)
{
    printf("%0.4f\t%0.4f\t%0.4f\n", x, log(x), fastlog(x));
}


int main(int argc, char* argv[])
{
    size_t n = 100000000;
    if (argc > 1) n = atoi(argv[1]);

    fastlog_init(16);

    /*size_t i;*/
    /*for (i = 0; i < 10; ++i) {*/
        /*f(fabs((double) rand() / (double) RAND_MAX));*/
    /*}*/

    
    double* xs        = malloc(n * sizeof(double));
    /*double* logxs     = malloc(n * sizeof(double));*/
    /*double* fastlogxs = malloc(n * sizeof(double));*/

    size_t i;
    for (i = 0; i < n; ++i) {
        xs[i] = abs(drand48());
    }

    double t_log         = time_log(log,         xs, n);
    double t_fastlog     = time_log(fastlog,     xs, n);
    double t_fastlog_asm = time_log(fastlog_asm, xs, n);


    printf("log         : %0.4f seconds\n", t_log);
    printf("fastlog     : %0.4f seconds\n", t_fastlog);
    printf("fastlog_asm : %0.4f seconds\n", t_fastlog_asm);


    free(xs);
    /*free(logxs);*/
    /*free(fastlogxs);*/
   

    fastlog_free();

    return 0;
}
