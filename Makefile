CC=g++
CCFLAGS= -Wall -std=c++0x

all: a3search
a3search: a3search.cpp stmr.cpp stmr.h
	$(CC) $(CCFLAGS) a3search.cpp  stmr.cpp -o a3search 

clean:
	rm *.o