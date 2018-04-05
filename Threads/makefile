CC = g++ $(CPPFLAGS)
CPPFLAGS = -std=c++11 -g -pthread -O0
all: Test
Test: integral.o 
	$(CC) -o test integral.o
clean:
	rm -f *.o test