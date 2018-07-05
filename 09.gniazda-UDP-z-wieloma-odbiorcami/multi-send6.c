#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "err.h"

#define BSIZE         256
#define HOPS_VALUE    4
#define REPEAT_COUNT  3
#define SLEEP_TIME    1

int main (int argc, char *argv[]) {
  /* argumenty wywołania programu */
  char *remote_colon_address;
  in_port_t remote_port;

  /* zmienne i struktury opisujące gniazda */
  int sock, optval;
  struct sockaddr_in6 local_address;
  struct sockaddr_in6 remote_address;

  /* zmienne obsługujące komunikację */
  char buffer[BSIZE];
  size_t length;
  time_t time_buffer;
  int i;

  /* parsowanie argumentów programu */
  if (argc != 3)
    fatal("Usage: %s remote_address remote_port\n", argv[0]);
  remote_colon_address = argv[1];
  remote_port = (in_port_t)atoi(argv[2]);

  /* otworzenie gniazda */
  sock = socket(AF_INET6, SOCK_DGRAM, 0);
  if (sock < 0)
    syserr("socket");

  /* IPv6 nie obsługuje rozgłaszania. */
  /*
  optval = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&optval, sizeof optval) < 0)
    syserr("setsockopt broadcast");
  */

  /* ustawienie liczby skoków  dla datagramów rozsyłanych do grupy */
  optval = HOPS_VALUE;
  if (setsockopt(sock, SOL_IPV6, IPV6_MULTICAST_HOPS, (void*)&optval, sizeof optval) < 0)
    syserr("setsockopt multicast hops");

  /* zablokowanie rozsyłania grupowego do siebie */
  /*
  optval = 0;
  if (setsockopt(sock, SOL_IPV6, IPV6_MULTICAST_LOOP, (void*)&optval, sizeof optval) < 0)
    syserr("setsockopt loop");
  */

  /* podpięcie się pod lokalny adres i port */
  local_address.sin6_family = AF_INET6;
  local_address.sin6_flowinfo = 0;
  local_address.sin6_addr = in6addr_any;
  local_address.sin6_port = htons(0);
  if (bind(sock, (struct sockaddr *)&local_address, sizeof local_address) < 0)
    syserr("bind");

  /* ustawienie adresu i portu odbiorcy */
  remote_address.sin6_family = AF_INET6;
  remote_address.sin6_port = htons(remote_port);
  if (inet_pton(AF_INET6, remote_colon_address, &remote_address.sin6_addr) < 0)
    syserr("inet_pton");
  if (connect(sock, (struct sockaddr *)&remote_address, sizeof remote_address) < 0)
    syserr("connect");

  /* radosne rozgłaszanie czasu */
  for (i = 0; i < REPEAT_COUNT; ++i) {
    time(&time_buffer);
    strncpy(buffer, ctime(&time_buffer), BSIZE);
    length = strnlen(buffer, BSIZE);
    if (write(sock, buffer, length) != length)
      syserr("write");
    sleep(SLEEP_TIME);
  }

  /* koniec */
  close(sock);
  exit(EXIT_SUCCESS);
}
