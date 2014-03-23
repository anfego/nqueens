#
CC = g++
CFLAGS = -g -std=c++0x
LIBS = -lm 

make all: main.cpp
	$(CC) main.cpp -o p4
monitor:
	$(CC) $(CFLAGS) -c monitor.cpp
clean:
	rm -f *.o 
