CFLAGS=-Wall -std=c11 -g
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

xcc: $(OBJS)
		$(CC) -o xcc $(OBJS) $(LDFLAGS)

$(OBJS): xcc.h

self: xcc
		gcc -c -o util.o util.c $(LDFLAGS)
		./xcc -file "util.c" > tmp_self_util.s
		# gcc -o tmp_self_util.o -c tmp_self_util.s 

		# gcc -c -o token.o token.c $(LDFLAGS)
		./xcc -file "token.c" > tmp_self_token.s

		# gcc -c -o preprocess.o preprocess.c $(LDFLAGS)
		./xcc -file "preprocess.c" > tmp_self_cpp.s

		# gcc -c -o parse.o parse.c $(LDFLAGS)
		./xcc -file "parse.c" > tmp_self_parse.s

		# gcc -c -o sema.o sema.c $(LDFLAGS)
		./xcc -file "sema.c" > tmp_self_sema.s

		gcc -c -o codegen.o codegen.c $(LDFLAGS)
		./xcc -file "codegen.c" > tmp_self_codegen.s

		gcc -c -o main.o main.c $(LDFLAGS)
		./xcc -file "main.c" > tmp_self_main.s

		gcc -static -o self_xcc \
		tmp_self_util.s \
		tmp_self_main.s \
		tmp_self_token.s \
		tmp_self_cpp.s \
		tmp_self_parse.s \
		tmp_self_sema.s \
		tmp_self_codegen.s 


		# gcc -static -o self_xcc \
		# util.o \
		# main.o \
		# token.o \
		# preprocess.o \
		# parse.o \
		# sema.o \
		# codegen.o.

		./self_xcc 'int main() { if (1) printf("yattaze%d", 1); }'
		./self_xcc 'int main() { if (1) {printf("yattaze%d", 1);} }'
		# ./self_xcc 'int main() { "aaaa";  }'
		# ./self_xcc -file "sample/debug.c"
		# ./self_xcc -file "xcc.h"

		#### n-queen self-compile test
		# ./self_xcc -file "sample/sample.c" > tmp2.s
		# gcc -static -o sample2 tmp2.s
		# ./sample2
		#### n-queen self-compile test

		# ./self_xcc -file "token.c"

self_test: self test/test.c
		@./self_xcc "$$(gcc -E -P test/test.c)" > tmp_self_test.s
		@echo 'int global_arr[1] = {5};' | gcc -xc -c -o tmp-test3.o -
		@gcc -static -o tmp_self_test tmp_self_test.s tmp-test3.o -g
		@./tmp_self_test

var_test: xcc sample/variadic_test.c
		@./xcc -file "sample/variadic_test.c" > sample/variadic_test.s
		@gcc -S sample/variadic_test.c -o sample/gcc_variadic_test.s
		@gcc -static -o sample/variadic_test sample/variadic_test.s -g
		@./sample/variadic_test

self_self: xcc self
		./self_xcc -file "util.c" > tmp_self_util2.s
		./self_xcc -file "token.c" > tmp_self_token2.s
		./self_xcc -file "preprocess.c" > tmp_self_cpp2.s
		./self_xcc -file "parse.c" > tmp_self_parse2.s
		./self_xcc -file "sema.c" > tmp_self_sema2.s
		./self_xcc -file "codegen.c" > tmp_self_codegen2.s
		./self_xcc -file "main.c" > tmp_self_main2.s

		gcc -static -o self_xcc2 \
		tmp_self_util2.s \
		tmp_self_main2.s \
		tmp_self_token2.s \
		tmp_self_cpp2.s \
		tmp_self_parse2.s \
		tmp_self_sema2.s \
		tmp_self_codegen2.s

		./self_xcc2 -file "sample/sample.c" > tmp3.s
		gcc -static -o sample3 tmp3.s
		./sample3

test: xcc test/test.c
		./xcc -test

		# ./test.sh

		@./xcc "$$(gcc -E -P test/test.c)" > tmp-test.s
		@echo 'int global_arr[1] = {5};' | gcc -xc -c -o tmp-test2.o -
		@gcc -static -o tmp-test tmp-test.s tmp-test2.o -g
		@./tmp-test


clean:
		rm -f xcc *.o *~ tmp* test/*~