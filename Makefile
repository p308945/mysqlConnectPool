test : test.o DBPool.o
	g++ -std=c++11 -g -o test test.o DBPool.o -lmysqlclient -lpthread

test.o : test.cpp
	g++ -c -g test.cpp

DBPool.o : DBPool.cpp
	g++ -c -g DBPool.cpp 

clean:
	rm test test.o DBPool.o
