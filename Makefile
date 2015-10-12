CPPFLAGS = -std=c++11 -Wall -g -gstabs
.PHONY : test clean
test : test.o DBPool.o
	g++ $(CPPFLAGS) -o test test.o DBPool.o -lmysqlclient -lpthread

test.o : test.cpp DBDefine.h
	g++ $(CPPFLAGS) -c test.cpp

DBPool.o : DBPool.cpp DBPool.h MyTime.h MyLock.h
	g++ $(CPPFLAGS) -c DBPool.cpp 

clean:
	rm -rf test
	rm -rf test.o
	rm -rf DBPool.o
