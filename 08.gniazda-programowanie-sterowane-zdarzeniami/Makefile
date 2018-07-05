CC = gcc
CFLAGS = -Wall 
TARGETS = client server 

all: $(TARGETS) 

server.o client.o err.o: err.h

client: client.o err.o
	$(CC) $(CFLAGS) $^ -o $@ -levent

server: server.o err.o
	$(CC) $(CFLAGS) $^ -o $@ -levent

clean:
	rm -f *.o $(TARGETS) 
