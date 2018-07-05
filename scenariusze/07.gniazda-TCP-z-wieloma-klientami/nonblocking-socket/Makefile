CC = gcc
CFLAGS = -Wall
TARGETS = echo-client-timeout echo-server-nonblocking

all: $(TARGETS) 

echo-client-timeout.o err.o: err.h

echo-client-timeout: echo-client-timeout.o err.o

echo-server-nonblocking: echo-server-nonblocking.o

clean:
	rm -f *.o $(TARGETS) 
