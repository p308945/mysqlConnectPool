test : test.o DBPool.o
	g++ -g -o test test.o DBPool.o -lmysqlclient

test.o : test.cpp
	g++ -c -g test.cpp

DBPool.o : DBPool.cpp
	g++ -c -g DBPool.cpp 

clean:
	rm test test.o DBPool.o
