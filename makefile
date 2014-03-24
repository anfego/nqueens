#
CC = g++
CFLAGS = -g -std=c++0x
LIBS = -lm -lpthread


make all: monitor.o main.cpp 
	$(CC) $(CFLAGS) monitor.o main.cpp -o p4 $(LIBS)
monitor:
	$(CC) $(CFLAGS) -c monitor.cpp
clean:
	rm -f *.o 
