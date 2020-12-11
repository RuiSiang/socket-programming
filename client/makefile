all: client clean

#variables
CC = g++
INCLUDE = .
CFLAGS = -Wall -Wextra -ansi -g -std=c++14
PFLAGS = -O3 -lpthread

#runner
client: client.o listener_thread.o handler_thread.o socket_control.o
	$(CC) client.o listener_thread.o handler_thread.o socket_control.o -o client $(PFLAGS)

client.o: client.cpp listener_thread.h handler_thread.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c client.cpp

listener_thread.o: listener_thread.cpp listener_thread.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c listener_thread.cpp

handler_thread.o: handler_thread.cpp handler_thread.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c handler_thread.cpp

socket_control.o: socket_control.cpp socket_control.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c socket_control.cpp

clean:
	rm -f client.o
	rm -f listener_thread.o
	rm -f handler_thread.o
	rm -f socket_control.o
