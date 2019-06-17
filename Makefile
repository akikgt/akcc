CFLAGS=-Wall -std=c11 -g
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

# xcc: xcc.c
xcc: $(OBJS)
		$(CC) -o xcc $(OBJS) $(LDFLAGS)

$(OBJS): xcc.h

test: xcc
		./xcc -test
		./test.sh

clean:
		rm -f xcc *.o *~ tmp*