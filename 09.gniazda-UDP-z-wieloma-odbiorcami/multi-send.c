#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>

#include "err.h"

#define BSIZE         256
#define TTL_VALUE     4
#define REPEAT_COUNT  3
#define SLEEP_TIME    5

int main(int argc, char *argv[]) {
    /* argumenty wywołania programu */
    char *remote_dotted_address;
    in_port_t remote_port;

    /* zmienne i struktury opisujące gniazda */
    int sock, optval;
//  struct sockaddr_in local_address;
    struct sockaddr_in remote_address;

    /* zmienne obsługujące komunikację */
    char buffer[BSIZE];
    long length;
    int i;

    /* parsowanie argumentów programu */
    if (argc != 3)
        fatal("Usage: %s remote_address remote_port\n", argv[0]);
    remote_dotted_address = argv[1];
    remote_port = (in_port_t) atoi(argv[2]);

    /* otworzenie gniazda */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        syserr("socket");

    /* uaktywnienie rozgłaszania (ang. broadcast) */
    optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &optval, sizeof optval) < 0)
        syserr("setsockopt broadcast");

    /* ustawienie TTL dla datagramów rozsyłanych do grupy */
    optval = TTL_VALUE;
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &optval, sizeof optval) < 0)
        syserr("setsockopt multicast ttl");

    /* zablokowanie rozsyłania grupowego do siebie */
    /*
    optval = 0;
    if (setsockopt(sock, SOL_IP, IP_MULTICAST_LOOP, (void*)&optval, sizeof optval) < 0)
      syserr("setsockopt loop");
    */

    /* podpięcie się pod lokalny adres i port */
//  local_address.sin_family = AF_INET;
//  local_address.sin_addr.s_addr = htonl(INADDR_ANY);
//  local_address.sin_port = htons(0);
//  if (bind(sock, (struct sockaddr *)&local_address, sizeof local_address) < 0)
//    syserr("bind");

    /* ustawienie adresu i portu odbiorcy */
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(remote_port);
    if (inet_aton(remote_dotted_address, &remote_address.sin_addr) == 0)
        syserr("inet_aton");
    // if (connect(sock, (struct sockaddr *)&remote_address, sizeof remote_address) < 0)
    //   syserr("connect");

    struct timeval tv;
    tv.tv_sec = 5;

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval)))
        syserr("SET TIMEOUT");


    struct sockaddr_in server_addr;
    socklen_t sendsize = sizeof(server_addr);
    /* radosne rozgłaszanie czasu */
    for (i = 0; i < REPEAT_COUNT; ++i) {
        //time(&time_buffer);
        strncpy(buffer, "GET_TIME", BSIZE);
        length = strnlen(buffer, BSIZE);
        if (sendto(sock, buffer, length, 0, (struct sockaddr *) &remote_address, sizeof(remote_address)) != length)
            syserr("write");
        //sleep(SLEEP_TIME);

        length = recvfrom(sock, buffer, BSIZE, 0, (struct sockaddr *) &server_addr, &sendsize);
        if (length == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            printf("TRIAL %d FAILURE\n", i);
            continue;
        } else if (length > 0) {
            printf("read %ld bytes: %.*s\n", length, (int) length, buffer);
            close(sock);
            exit(EXIT_SUCCESS);
        } else {
            syserr("read");
        }
    }
    printf("DOESNT RECEIVED RESPONSE FROM SERVER");
    /* koniec */
    close(sock);
    exit(EXIT_SUCCESS);
}
