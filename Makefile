CFLAGS=-Wall -std=c11 -g
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

akcc: $(OBJS)
		$(CC) -o akcc $(OBJS) $(LDFLAGS)

$(OBJS): akcc.h

self: akcc
		# gcc -c -o util.o util.c $(LDFLAGS)
		./akcc -file "util.c" > tmp_self_util.s

		# gcc -c -o token.o token.c $(LDFLAGS)
		./akcc -file "token.c" > tmp_self_token.s

		# gcc -c -o preprocess.o preprocess.c $(LDFLAGS)
		./akcc -file "preprocess.c" > tmp_self_cpp.s

		# gcc -c -o parse.o parse.c $(LDFLAGS)
		./akcc -file "parse.c" > tmp_self_parse.s

		# gcc -c -o sema.o sema.c $(LDFLAGS)
		./akcc -file "sema.c" > tmp_self_sema.s

		# gcc -c -o codegen.o codegen.c $(LDFLAGS)
		./akcc -file "codegen.c" > tmp_self_codegen.s

		# gcc -c -o main.o main.c $(LDFLAGS)
		./akcc -file "main.c" > tmp_self_main.s

		gcc -static -o self_akcc \
		tmp_self_util.s \
		tmp_self_main.s \
		tmp_self_token.s \
		tmp_self_cpp.s \
		tmp_self_parse.s \
		tmp_self_sema.s \
		tmp_self_codegen.s 

		./self_akcc 'int main() { if (1) printf("yattaze%d", 1); }'
		./self_akcc 'int main() { if (1) {printf("yattaze%d", 1);} }'

self_test: self test/test.c
		@./self_akcc "$$(gcc -E -P test/test.c)" > tmp_self_test.s
		@./self_akcc -file "test/global_var_test.c" > tmp_self_gv_test.s
		@echo 'int global_arr[1] = {5};' | gcc -xc -c -o tmp-test3.o -
		@gcc -static -o tmp_self_test tmp_self_test.s tmp-test3.o tmp_self_gv_test.s -g
		@./tmp_self_test

var_test: akcc sample/variadic_test.c
		@./akcc -file "sample/variadic_test.c" > sample/variadic_test.s
		@gcc -S sample/variadic_test.c -o sample/gcc_variadic_test.s
		@gcc -static -o sample/variadic_test sample/variadic_test.s -g
		@./sample/variadic_test

self2: akcc self
		./self_akcc -file "util.c" > tmp_self_util2.s
		./self_akcc -file "token.c" > tmp_self_token2.s
		./self_akcc -file "preprocess.c" > tmp_self_cpp2.s
		./self_akcc -file "parse.c" > tmp_self_parse2.s
		./self_akcc -file "sema.c" > tmp_self_sema2.s
		./self_akcc -file "codegen.c" > tmp_self_codegen2.s
		./self_akcc -file "main.c" > tmp_self_main2.s

		gcc -static -o self_akcc2 \
		tmp_self_util2.s \
		tmp_self_main2.s \
		tmp_self_token2.s \
		tmp_self_cpp2.s \
		tmp_self_parse2.s \
		tmp_self_sema2.s \
		tmp_self_codegen2.s

		./self_akcc2 -file "sample/sample.c" > tmp3.s
		gcc -static -o sample3 tmp3.s

		./sample3
self3: akcc self self2
		./self_akcc2 -file "util.c" > tmp_self_util3.s
		./self_akcc2 -file "token.c" > tmp_self_token3.s
		./self_akcc2 -file "preprocess.c" > tmp_self_cpp3.s
		./self_akcc2 -file "parse.c" > tmp_self_parse3.s
		./self_akcc2 -file "sema.c" > tmp_self_sema3.s
		./self_akcc2 -file "codegen.c" > tmp_self_codegen3.s
		./self_akcc2 -file "main.c" > tmp_self_main3.s

		gcc -static -o self_akcc3 \
		tmp_self_util3.s \
		tmp_self_main3.s \
		tmp_self_token3.s \
		tmp_self_cpp3.s \
		tmp_self_parse3.s \
		tmp_self_sema3.s \
		tmp_self_codegen3.s

		./self_akcc3 -file "sample/sample.c" > tmp4.s
		gcc -static -o sample4 tmp4.s
		./sample4

test: akcc test/test.c
		./akcc -test

		# ./test.sh

		@./akcc "$$(gcc -E -P test/test.c)" > tmp-test.s
		@./akcc -file "test/global_var_test.c" > tmp-test3.s
		@echo 'int global_arr[1] = {5};' | gcc -xc -c -o tmp-test2.o -
		@gcc -static -o tmp-test tmp-test.s tmp-test2.o tmp-test3.s -g
		@./tmp-test


clean:
		rm -f akcc *.o *~ tmp* test/*~ self_akcc*