CC = g++ $(CPPFLAGS)
CPPFLAGS = -std=c++11 -g -pthread -O0
all: Test
Test: multiply.o 
	$(CC) -o test multiply.cpp
clean:
	rm -f *.o test