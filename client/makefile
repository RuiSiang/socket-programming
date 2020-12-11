all: client server clean

#variables
CC = g++
INCLUDE = .
CFLAGS = -Wall -Wextra -ansi -g -std=c++14
PFLAGS = -O3 -lpthread

#runner
client: client.o listener_thread.o handler_thread.o
	$(CC) client.o listener_thread.o handler_thread.o -o client $(PFLAGS)

client.o: client.cpp listener_thread.h handler_thread.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c client.cpp

listener_thread.o: listener_thread.cpp listener_thread.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c listener_thread.cpp

handler_thread.o: handler_thread.cpp handler_thread.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c handler_thread.cpp

clean:
	rm -f client.o
	rm -f listener_thread.o
	rm -f handler_thread.o