
CFLAGS=-Wall -Wextra -g -O3 -ffast-math

all : fastlog_test

fastlog_test : fastlog_test.o fastlog.o
	gcc $(CFLAGS) -o $@ $^ -lm

%.o : %.c
	gcc $(CFLAGS) -c -o $@ $<

clean :
	rm -f *.o fastlog_test


