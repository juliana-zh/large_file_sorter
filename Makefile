CC=g++
CFLAGS=-c -Wall

all: sort

sort: main.o sorter.o
	$(CC) main.o sorter.o -o sort

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

sorter.o: sorter.cpp
	$(CC) $(CFLAGS) sorter.cpp

clean:
	rm -rf *.o sort