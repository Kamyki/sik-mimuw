CC = gcc
CFLAGS = -Wall -O2
TARGETS = sikradio-sender sikradio-receiver

SENDER = fifo.o set.o
RECEIVER = menu.o buffor.o
SHARE = err.o argument_parser.o

all: $(TARGETS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

sikradio-sender: nadajnik.c $(SENDER) $(SHARE)
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

sikradio-receiver: odbiornik.c $(RECEIVER) $(SHARE)
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

clean:
	rm -f *.o $(TARGETS)
