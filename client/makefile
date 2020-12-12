all: client clean

#variables
CC = g++
INCLUDE = .
CFLAGS = -Wall -Wextra -g -std=c++14
PFLAGS = -O3 -lpthread #-lws2_32 #uncomment that for windows compiler

#runner
client: client.o listener_thread.o handler_thread.o socket_control.o logger.o
	$(CC) client.o listener_thread.o handler_thread.o socket_control.o logger.o -o client $(PFLAGS)

client.o: client.cpp listener_thread.h handler_thread.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c client.cpp

listener_thread.o: listener_thread.cpp listener_thread.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c listener_thread.cpp

handler_thread.o: handler_thread.cpp handler_thread.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c handler_thread.cpp

socket_control.o: socket_control.cpp socket_control.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c socket_control.cpp

logger.o: logger.cpp logger.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c logger.cpp

clean:
	rm -f client.o
	rm -f listener_thread.o
	rm -f handler_thread.o
	rm -f socket_control.o
	rm -f logger.o
