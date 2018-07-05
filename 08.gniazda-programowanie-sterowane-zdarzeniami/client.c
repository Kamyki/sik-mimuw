#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/util.h>

#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include "err.h"

#define BUF_SIZE 4

typedef struct {
    char* filename;
} e_data;

struct event_base *base;
struct bufferevent *bev;
int filedesc;
e_data data;

void stdin_cb (evutil_socket_t descriptor, short ev, void *arg) {
  unsigned char buf[BUF_SIZE+1];

  int r = read(descriptor, buf, BUF_SIZE);
  if (r < 0)
    syserr("read (from stdin)");
  if (r == 0) {
    fprintf(stderr, "stdin closed. Exiting event loop.\n");
    if (event_base_loopbreak(base) == -1) syserr("event_base_loopbreak");
    return;
  }
  if (bufferevent_write(bev, buf, r) == -1)
    syserr("bufferevent_write");
}

void a_read_cb (struct bufferevent *bev, void *arg) {
  char buf[BUF_SIZE+1];

  while (evbuffer_get_length(bufferevent_get_input(bev))) {
    int r = bufferevent_read(bev, buf, BUF_SIZE);
    if (r == -1)
      syserr("bufferevent_read");

      if (r == 0) {
          fprintf(stderr, "stdin closed. Exiting event loop.\n");
          if (event_base_loopbreak(base) == -1) syserr("event_base_loopbreak");
          return;
      }
    buf[r] = 0;
    printf("\n--> %s\n", buf);
  }
    close(filedesc);
}

void read_file(struct bufferevent *bev, e_data* data) {
  if (bufferevent_write(bev, data->filename, strlen(data->filename)) == -1)
    syserr("bufferevent_write");

  char *a = "\0";
  if (bufferevent_write(bev, a, 1) == -1)
    syserr("bufferevent_write");

  unsigned char buf[BUF_SIZE+1];
    for(;;) {
        int r = read(filedesc, buf, BUF_SIZE);
        if (r < 0)
            syserr("read (from file)");
        if (r == 0) {
            fprintf(stderr, "file closed. Exiting event loop.\n");
            if (event_base_loopbreak(base) == -1) syserr("event_base_loopbreak");
            return;
        }

        if (bufferevent_write(bev, buf, r) == -1)
            syserr("bufferevent_write");
    }
}

void an_event_cb (struct bufferevent *bev, short what, void *arg) {
  if (what & BEV_EVENT_CONNECTED) {
    fprintf(stderr, "Connection made.\n");
      read_file(bev, (e_data*)arg);
    return;
  }
  if (what & BEV_EVENT_EOF)
    fprintf(stderr, "EOF encountered.\n");
  else if (what & BEV_EVENT_ERROR)
    fprintf(stderr, "Unrecoverable error.\n");
  else if (what & BEV_EVENT_TIMEOUT)
    fprintf(stderr, "A timeout occured.\n");
  if (event_base_loopbreak(base) == -1)
    syserr("event_base_loopbreak");
}

int main(int argc, char *argv[])
{
  /* Kontrola dokumentów ... */
  if (argc < 4)
    fatal("Usage: %s hostname port filename\n", argv[0]);

  filedesc = open(argv[3], O_RDONLY);
  if(filedesc < 0){
    syserr("Open file");
  }
  data.filename = argv[3];

  // Jeśli chcemy, żeby wszystko działało nieco wolniej, ale za to
  // działało dla wejścia z pliku, to należy odkomentować linijki
  // poniżej, a zakomentować aktualne przypisanie do base
  /* struct event_config *cfg = event_config_new(); */
  /* event_config_avoid_method(cfg, "epoll"); */
  /* base = event_base_new_with_config(cfg); */
  /* event_config_free(cfg); */

  base = event_base_new();
  if (!base)
    syserr("event_base_new");

  bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
  if (!bev)
    syserr("bufferevent_socket_new");
  bufferevent_setcb(bev, a_read_cb, NULL, an_event_cb, (void *)&data);

  struct addrinfo addr_hints = {
    .ai_flags = 0,
    .ai_family = AF_INET,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = 0,
    .ai_addrlen = 0,
    .ai_addr = NULL,
    .ai_canonname = NULL,
    .ai_next = NULL
  };
  struct addrinfo *addr;

  if (getaddrinfo(argv[1], argv[2], &addr_hints, &addr))
    syserr("getaddrinfo");

  if (bufferevent_socket_connect(bev, addr->ai_addr, addr->ai_addrlen) == -1)
    syserr("bufferevent_socket_connect");
  freeaddrinfo(addr);
  if (bufferevent_enable(bev, EV_READ | EV_WRITE) == -1)
    syserr("bufferevent_enable");

  struct event *stdin_event =
    event_new(base, 0, EV_READ|EV_PERSIST, stdin_cb, NULL);
  if (!stdin_event)
    syserr("event_new");
  if (event_add(stdin_event,NULL) == -1)
    syserr("event_add");
  
  printf("Entering dispatch loop.\n");
  if (event_base_dispatch(base) == -1)
    syserr("event_base_dispatch");
  printf("Dispatch loop finished.\n");

  bufferevent_free(bev);
  event_base_free(base);

  return 0;
}
