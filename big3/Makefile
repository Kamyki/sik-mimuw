CC = g++
CFLAGS = -Wall -O2
TARGETS = sikradio-sender sikradio-receiver

SENDER = fifo.o set.o
RECEIVER = menu.o buffor.o
SHARE = err.o argument_parser.o

all: $(TARGETS)

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

sikradio-sender: nadajnik.cpp $(SENDER) $(SHARE)
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

sikradio-receiver: odbiornik.cpp $(RECEIVER) $(SHARE)
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

clean:
	rm -f *.o $(TARGETS)
