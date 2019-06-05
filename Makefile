xcc: xcc.c

test: xcc
		./xcc -test
		./test.sh

clean:
		rm -f xcc *.o *~ tmp*