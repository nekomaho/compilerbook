9cc: 9cc.o vector.o gen.o node.o program.o tokenize.o test/test.o test/vector_test.o

9cc.o: 9cc.h program.h node.h test/test.h vector.h map.h
gen.o: gen.h tokenize.h node.h program.h
program.o: program.h node.h tokenize.h
node.o: node.h
tokenize.o: tokenize.h vector.h map.h
vector.o: vector.h
map.o: map.h
test/test.o: test/vector_test.h test/test.h
test/vector_test.o: vector.h test/vector_test.h

run_test: 9cc
			./9cc -test
	        ./test.sh

clean:
	        rm -f 9cc *.o *~ tmp*
	        rm -f test/*.o