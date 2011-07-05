
#include "fastlog.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


double time_log(double (*logfunc)(double), double* xs, double* logxs, size_t n)
{
    clock_t t0 = clock();

    size_t i;
    for (i = 0; i < n; ++i) {
        logxs[i] = logfunc(xs[i]);
    }

    clock_t t1 = clock();
    return (double) (t1 - t0) / (double) CLOCKS_PER_SEC;
}


int main(int argc, char* argv[])
{
    size_t n = 100000000;
    if (argc > 1) n = atoi(argv[1]);

    fastlog_init(16);

    double* xs        = malloc(n * sizeof(double));
    double* logxs     = malloc(n * sizeof(double));
    double* fastlogxs = malloc(n * sizeof(double));

    size_t i;
    for (i = 0; i < n; ++i) {
        xs[i] = abs(drand48());
    }

    double t_log     = time_log(log,     xs, logxs,     n);
    double t_fastlog = time_log(fastlog, xs, fastlogxs, n);


    printf("log     : %0.2f seconds\n", t_log);
    printf("fastlog : %0.2f seconds\n", t_fastlog);


    free(xs);
    free(logxs);
    free(fastlogxs);

    fastlog_free();

    return 0;
}
