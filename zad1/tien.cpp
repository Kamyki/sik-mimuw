include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "err.h"

#define BUFFER_SIZE 256
#define BACKLOG 5

#define IAC 255
#define DO 253
#define WILL 251
#define SB 250
#define SE 240
#define ECHO 1
#define LINEMODE 34
#define ERASE_LINE 248
#define ERASE_CHARACTER 247
#define LF 10
#define CR 13

#define CSI(C) "\033["#C
#define HIDE_CURSOR  CSI(?25l)

char buffer[BUFFER_SIZE];

static const char ENTER_BYTES[] = {13};
static const char UP_BYTES[] = {27, 91, 65};
static const char DOWN_BYTES[] = {27, 91, 66};

typedef enum Command {
    NONE,
    ENTER,
    UP,
    DOWN,
    END,
} command;

command recv_command(int msg_sockfd) {
    ssize_t bytes_recv = recv(msg_sockfd, &buffer, sizeof(buffer), 0);
    if (bytes_recv < 0)
        syserr("recv");
    int i;
    for (i = 0; i < bytes_recv; ++i) {
        printf("%d ", (uint8_t) buffer[i]);
    }
    if (bytes_recv == 0)
        return END;
    else if ((bytes_recv >= 1) && (memcmp(ENTER_BYTES, buffer, 1) == 0))
        return ENTER;
    else if ((bytes_recv >= 3) && (memcmp(UP_BYTES, buffer, 3) == 0))
        return UP;
    else if ((bytes_recv >= 3) && (memcmp(DOWN_BYTES, buffer, 3) == 0))
        return DOWN;
    else
        return NONE;
}

void send_safely(int msg_sockfd, char* msg, ssize_t len) {
    ssize_t bytes_sent;
    bytes_sent = send(msg_sockfd, msg, len, 0);
    if (bytes_sent != len)
        syserr("send");
}

void clear_lines(int msg_sockfd, int n) {
    char buffer[BUFFER_SIZE];
    int i;
    for (i = 0; i < n-1; ++i) {
        snprintf(buffer, BUFFER_SIZE, CSI(2K) CSI(A));
        send_safely(msg_sockfd, buffer, strlen(buffer));
    }
    snprintf(buffer, BUFFER_SIZE, CSI(2K));
    send_safely(msg_sockfd, buffer, strlen(buffer));
}

void render_menu(int msg_sockfd, char *options[3], int select) {
    int i;
    for (i = 0; i < 3; ++i) {
        if (select == i)
            snprintf(buffer, BUFFER_SIZE, CSI(7m) "%s" CSI(0m) "%c%c", options[i], CR, LF);
        else
            snprintf(buffer, BUFFER_SIZE, "%s%c%c", options[i], CR, LF);
        send_safely(msg_sockfd, buffer, strlen(buffer));
    }
}

void b_menu(int msg_sockfd) {
    static char *options[3] = {"Opcja B1", "Opcja B2", "Wstecz"};
    int select = 0;
    render_menu(msg_sockfd, options, select);
    for (;;) {
        switch (recv_command(msg_sockfd)) {
            case END:
                return;
            case ENTER:
                switch (select) {
                    case 0:
                        snprintf(buffer, BUFFER_SIZE, "B1" CSI(2D));
                        send_safely(msg_sockfd, buffer, strlen(buffer));
                        break;
                    case 1:
                        snprintf(buffer, BUFFER_SIZE, "B2" CSI(2D));
                        send_safely(msg_sockfd, buffer, strlen(buffer));
                        break;
                    case 2:
                        clear_lines(msg_sockfd, 4);
                        return;
                }
                break;
            case UP:
                select = (select+2) % 3;
                clear_lines(msg_sockfd, 4);
                render_menu(msg_sockfd, options, select);
                break;
            case DOWN:
                select = (select+1) % 3;
                clear_lines(msg_sockfd, 4);
                render_menu(msg_sockfd, options, select);
                break;
            default:
                break;
        }
    }
}

void main_menu(int msg_sockfd) {
    static char *options[3] = {"Opcja A", "Opcja B", "Koniec"};
    int select = 0;
    render_menu(msg_sockfd, options, select);
    for (;;) {
        switch (recv_command(msg_sockfd)) {
            case END:
                return;
            case ENTER:
                switch (select) {
                    case 0:
                        snprintf(buffer, BUFFER_SIZE, "A" CSI(1D));
                        send_safely(msg_sockfd, buffer, strlen(buffer));
                        break;
                    case 1:
                        clear_lines(msg_sockfd, 4);
                        b_menu(msg_sockfd);
                        render_menu(msg_sockfd, options, select);
                        break;
                    case 2:
                        clear_lines(msg_sockfd, 4);
                        return;
                }
                break;
            case UP:
                select = (select+2) % 3;
                clear_lines(msg_sockfd, 4);
                render_menu(msg_sockfd, options, select);
                break;
            case DOWN:
                select = (select+1) % 3;
                clear_lines(msg_sockfd, 4);
                render_menu(msg_sockfd, options, select);
                break;
            default:
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fatal("Usage: %s port\n", argv[0]);
    }

    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, msg_sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, argv[1], &hints, &res))
        syserr("getaddrinfo");

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0)
        syserr("socket");

    int yes=1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
        syserr("setsockupt");

    if(bind(sockfd, res->ai_addr, res->ai_addrlen))
        syserr("bind");

    if (listen(sockfd, BACKLOG) < 0)
        syserr("listen");

    for(;;) {
        addr_size = sizeof client_addr;
        msg_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
        if (msg_sockfd < 0)
            syserr("accept");
        printf("Client has connected.\n");
        snprintf(buffer, 256, "%c%c%c%c%c%c", IAC, WILL, ECHO, IAC, DO, LINEMODE);
        send_safely(msg_sockfd, buffer, strlen(buffer));
        main_menu(msg_sockfd);
        snprintf(buffer, 256, CSI(?25h));
        send_safely(msg_sockfd, buffer, strlen(buffer));
        if (close(msg_sockfd) < 0)
            syserr("close");
        printf("Client has been disconnected.\n");
    }

    return 0;
}
