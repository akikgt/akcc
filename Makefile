CFLAGS=-Wall -std=c11 -g
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

xcc: $(OBJS)
		$(CC) -o xcc $(OBJS) $(LDFLAGS)

$(OBJS): xcc.h

self: xcc
		gcc -c -o util.o util.c $(LDFLAGS)
		gcc -c -o token.o token.c $(LDFLAGS)
		gcc -c -o preprocess.o preprocess.c $(LDFLAGS)
		# gcc -c -o parse.o parse.c $(LDFLAGS)
		./xcc -file "sample/demo_parse.c" > tmp_parse.s
		gcc -c -o sema.o sema.c $(LDFLAGS)
		gcc -c -o codegen.o codegen.c $(LDFLAGS)
		gcc -c -o main.o main.c $(LDFLAGS)
		gcc -static -o self_xcc main.o util.o token.o preprocess.o tmp_parse.s sema.o codegen.o

		# ./self_xcc 'int main() { if (1) printf("yattaze%d", 1); }'
		# ./self_xcc 'int main() { if (1) "aaaa";  }'
		./self_xcc -file "sample/debug.c"
		# ./self_xcc -file "xcc.h"

		#### n-queen self-compile test
		# ./self_xcc -file "sample/sample.c" > tmp2.s
		# gcc -static -o sample2 tmp2.s
		# ./sample2
		#### n-queen self-compile test

		#  ./self_xcc -file "sample/demo_parse.c"
test: xcc test/test.c
		./xcc -test

		# ./test.sh

		@./xcc "$$(gcc -E -P test/test.c)" > tmp-test.s
		@echo 'int global_arr[1] = {5};' | gcc -xc -c -o tmp-test2.o -
		@gcc -static -o tmp-test tmp-test.s tmp-test2.o -g
		@./tmp-test
clean:
		rm -f xcc *.o *~ tmp* test/*~