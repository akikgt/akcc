CFLAGS=-Wall -std=c11 -g
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

xcc: $(OBJS)
		$(CC) -o xcc $(OBJS) $(LDFLAGS)

$(OBJS): xcc.h

self: xcc
		gcc -c -o util.o util.c $(LDFLAGS)

		# gcc -c -o token.o token.c $(LDFLAGS)
		./xcc -file "token.c" > tmp_self_token.s

		# gcc -c -o preprocess.o preprocess.c $(LDFLAGS)
		./xcc -file "preprocess.c" > tmp_self_cpp.s

		gcc -c -o parse.o parse.c $(LDFLAGS)
		# ./xcc -file "sample/demo_parse.c" > tmp_parse.s
		# ./xcc -file "parse.c" > tmp_self_parse.s

		# gcc -c -o sema.o sema.c $(LDFLAGS)
		./xcc -file "sema.c" > tmp_self_sema.s

		gcc -c -o codegen.o codegen.c $(LDFLAGS)
		./xcc -file "codegen.c" > tmp_self_codegen.s

		gcc -c -o main.o main.c $(LDFLAGS)
		./xcc -file "main.c" > tmp_self_main.s

		gcc -static -o self_xcc \
		main.o \
		util.o \
		tmp_self_token.s \
		tmp_self_cpp.s \
		parse.o \
		tmp_self_sema.s \
		tmp_self_codegen.s 
		# gcc -static -o self_xcc main.o util.o token.o preprocess.o parse.o sema.o codegen.o

		./self_xcc 'int main() { if (1) printf("yattaze%d", 1); }'
		./self_xcc 'int main() { if (1) {printf("yattaze%d", 1);} }'
		# ./self_xcc 'int main() { "aaaa";  }'
		# ./self_xcc -file "sample/debug.c"
		# ./self_xcc -file "xcc.h"

		#### n-queen self-compile test
		./self_xcc -file "sample/sample.c" > tmp2.s
		gcc -static -o sample2 tmp2.s
		./sample2
		#### n-queen self-compile test

		# ./self_xcc -file "token.c"

test: xcc test/test.c
		./xcc -test

		# ./test.sh

		@./xcc "$$(gcc -E -P test/test.c)" > tmp-test.s
		@echo 'int global_arr[1] = {5};' | gcc -xc -c -o tmp-test2.o -
		@gcc -static -o tmp-test tmp-test.s tmp-test2.o -g
		@./tmp-test
clean:
		rm -f xcc *.o *~ tmp* test/*~