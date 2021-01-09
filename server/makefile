all: server clean

#variables
CC = g++
INCLUDE = -I. -I./include
CFLAGS = -Wall -Wextra -ansi -g -std=c++14
PFLAGS = -O3 -L${CURDIR}/lib -lssl -lcrypto -lz -ldl -lpthread

#runner
server: server.o listener_thread.o handler_thread.o ssl_handler.o base64.o
	$(CC) server.o listener_thread.o handler_thread.o ssl_handler.o base64.o -o bin/server $(PFLAGS)

server.o: server.cpp listener_thread.h handler_thread.h
	$(CC) $(INCLUDE) $(CFLAGS) -c server.cpp

listener_thread.o: listener_thread.cpp listener_thread.h
	$(CC) $(INCLUDE) $(CFLAGS) -c listener_thread.cpp

handler_thread.o: handler_thread.cpp handler_thread.h
	$(CC) $(INCLUDE) $(CFLAGS) -c handler_thread.cpp

ssl_handler.o: ssl_handler.cpp ssl_handler.h
	$(CC) $(INCLUDE) $(CFLAGS) -c ssl_handler.cpp

base64.o: base64.cpp base64.h
	$(CC) $(INCLUDE) $(CFLAGS) -c base64.cpp

clean:
	rm -f server.o
	rm -f listener_thread.o
	rm -f handler_thread.o
	rm -f ssl_handler.o
	rm -f base64.o