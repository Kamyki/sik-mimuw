CC = gcc
CFLAGS = -Wall
TARGETS = client proc_server thread_server poll_server preforked_server 

all: $(TARGETS) 

err.o: err.c err.h

client.o: client.c err.h

client: client.o err.o

proc_server: proc_server.o

thread_server: thread_server.c
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

poll_server: poll_server.o

preforked_server: preforked_server.o

clean:
	rm -f *.o $(TARGETS) 
