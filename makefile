#
CC = g++
CFLAGS = -std=c++0x
LIBS = -lm -lpthread


make all: monitor.o nqueens.cpp 
	$(CC) $(CFLAGS) monitor.o nqueens.cpp -o p4 $(LIBS)
monitor.o:
	$(CC) $(CFLAGS) -c monitor.cpp

clean:
	rm -f *.o 
