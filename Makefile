xcc: xcc.c

test: xcc
		./test.sh

clean:
		rm -f xcc *.o *~ tmp*