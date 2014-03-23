#
CC = g++
CFLAGS = -g
LIBS = -lm 

make all: main.cpp monitor.o
	$(CC) monitor.o main.cpp -o p4
monitor:
	$(CC) $(CFLAGS) -c monitor.cpp 
clean:
	rm -f *.o 
