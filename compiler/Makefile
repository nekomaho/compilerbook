9cc: 9cc.o vector.o node.o test/test.o

9cc.o: 9cc.h node.h test/test.h vector.h
node.o: node.h
vector.o: vector.h
test/test.o: vector.h test/test.h

run_test: 9cc
			./9cc -test
	        ./test.sh

clean:
	        rm -f 9cc *.o *~ tmp*
	        rm -f test/*.o