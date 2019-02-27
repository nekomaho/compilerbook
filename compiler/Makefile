9cc: 9cc.o vector.o test/test.o

9cc.o: 9cc.h test/test.h vector.h
vector.o: vector.h
test/test.o: vector.h test/test.h

run_test: 9cc
			./9cc -test
	        ./test.sh

clean:
	        rm -f 9cc *.o *~ tmp*
	        rm -f test/*.o