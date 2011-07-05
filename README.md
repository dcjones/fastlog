
fastlog
=======

Overview
--------
This is an implementation of the natural logrothim function in C (as in the
`log` function from 'math.h'). It is, in my tests, about three times faster than
the standard library log function on gcc 4.5. The tradeoff is that one must limit
the precision to k bits, and build a lookup table of using 2^(k+2) bytes of
memory.

The idea is really very simple. A floating point number x is represented as
`x = u * 2 ^ k`. So the natural logarithm is,

           ln(x) = ln(u * 2 ^ k) = ln(u) + k * ln(2)

Now, `ln(2)` is just a constant, and ln(u) can be stored in a lookup table, if we
reduce the precision of `u` (i.e., the mantissa) from 52 bits to, say, 16 bits or
so. You may specify the precision, but memory will quickly become an issue if it
gets large.

This idea was taken from a 2007 paper:

"Revisiting a basic function on current CPUs: a fast logarithm implementation
with adjustable accuracy" by Vinyals, Friedland, and Mirghafori.



Usage
-----

The `fastlog` function works essentially like the `log` function.

But, be sure to call `fastlog_init(precision)` before calling `fastlog`. The
`fastlog_free()` function frees the lookup table, when you are finished.



Copying
-------

MIT license. See the source files.



Bugs
----

Please notify me of any bugs: dcjones@cs.washington.edu




