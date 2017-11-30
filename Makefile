all: a.out tests
a.out: monad.cpp
	clang++ -std=c++11 monad.cpp

tests: a.out
	./a.out

clean:
	rm *.out
