all: server clean

#variables
CC = g++
INCLUDE = .
CFLAGS = -Wall -Wextra -ansi -g -std=c++14
PFLAGS = -O3 -lpthread

#runner
server: server.o listener_thread.o handler_thread.o
	$(CC) server.o listener_thread.o handler_thread.o -o server $(PFLAGS)

server.o: server.cpp listener_thread.h handler_thread.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c server.cpp

listener_thread.o: listener_thread.cpp listener_thread.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c listener_thread.cpp

handler_thread.o: handler_thread.cpp handler_thread.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c handler_thread.cpp

clean:
	rm -f server.o
	rm -f listener_thread.o
	rm -f handler_thread.o