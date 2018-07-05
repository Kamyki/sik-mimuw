#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <poll.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdatomic.h>

#include "err.h"
#include "argument_parser.h"
#include "menu.h"
#include "buffor.h"

#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))
#define BACKLOG 10

atomic_bool finish = false;
atomic_bool stop = false;
atomic_int menu_changed = 0;

parameters params;

char* LOOKUP = "ZERO_SEVEN_COME_IN";
char* REPLY = "BOROWICZ_HERE";
char* REXMIT = "LOUDER_PLEASE ";

pthread_mutex_t mutex_menu;
pthread_mutex_t mutex_buffor;
pthread_mutex_t mutex_ctrl;

Menu* menu;
struct pollfd disc_fd;
struct ip_mreq ip_mreq;
struct sockaddr_in recieve_addr;
Buffor* data_buffor;


void* printing_loop(void* nil);

int parse_recv(char* buffer, char* name, uint16_t* port, char* address);


static void catch_interput(int sig) {
    finish = true;
    fprintf(stderr,
            "Signal %d catched. Closing...\n", sig);
}

int setTcp() {
    struct sockaddr_in address;
    int socket_val = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_val < 0) {
        syserr("Error when invoking socket");
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(params.UI_PORT);

    int yes = 1;
    if (setsockopt(socket_val, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
        syserr("Error when  invoking socketopt");

    if (bind(socket_val, (struct sockaddr*) &address, sizeof(address)) < 0) {
        syserr("Error when invoking bind");
    }

    if (listen(socket_val, BACKLOG) < 0) {
        syserr("Error when invoking listen");
    }
    return socket_val;
}

int handleClient(int server_socket) {
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_address_len);
    if (client_socket < 0) {
        syserr("Error when accpeting client connection"); // TODO change to stderr
    }

    return client_socket;
}


void write_to_tcp(int sock, const uint8_t buffer[], size_t len) {
    ssize_t written = send(sock, buffer, len, 0);
    if (written == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        dprintf(STDERR_FILENO, "Block in writing to tcp client");
    } else if (written != len) {
        syserr("TCP write");
    }
}

void* loop_terminal(void* nil) {
    int ret;
    nfds_t size = 1;
    struct pollfd* sockets = malloc(sizeof(struct pollfd));
    sockets[0].fd = setTcp();
    sockets[0].events = POLLIN;
    sockets[0].revents = 0;

    u_int8_t in_buffer[128];

    do {
        //if( ret > 0) {
        ret = poll(sockets, size, 0);
        if (ret > 0) {
            if (finish == false && (sockets[0].revents & POLLIN)) {
                int client = handleClient(sockets[0].fd);
                size++;
                sockets = realloc(sockets, sizeof(struct pollfd) * size);
                sockets[size - 1].fd = client;
                sockets[size - 1].events = POLLIN & POLLHUP;
                sockets[size - 1].revents = 0;
                sockets[0].revents = 0;
                write_to_tcp(client, init_seq(), 6);
                uint8_t out_buffer[64 * 1024];
                size_t len = draw_menu(menu, out_buffer);
                write_to_tcp(client, out_buffer, len);
            }

            for (int i = 1; i < size; ++i) {
                if (finish == false && (sockets[i].revents & POLLIN)) {
                    ssize_t len_red = read(sockets[i].fd, in_buffer, 3);
                    if (len_red < 0)
                        syserr("reading");

                    printf("LINE: ");
                    for (int k = 0; k < len_red; ++k) {
                        printf("%x ", in_buffer[k]);
                    }
                    printf("\n");
                    if (len_red == 3) {
                        pthread_mutex_lock(&mutex_menu);
                        int should_refresh = transform(menu, in_buffer);
                        menu_changed |= should_refresh;
                        pthread_mutex_unlock(&mutex_menu);
                        if (should_refresh) {

                            uint8_t out_buffer[64 * 1024];
                            pthread_mutex_lock(&mutex_menu);
                            size_t len = draw_menu(menu, out_buffer);
                            pthread_mutex_unlock(&mutex_menu);

                            for (int j = 1; j < size; ++j) {
                                write_to_tcp(sockets[j].fd, out_buffer, len);
                                sockets[j].revents = 0;
                            }
                            break;
                        }
                    }
                    sockets[i].revents = 0;
                }
                if ((finish == false && (sockets[i].revents & POLLHUP))) {
                    close(sockets[i].fd);
                    sockets[i] = sockets[size - 1];
                    size--;
                    sockets = realloc(sockets, sizeof(struct pollfd) * size);
                }
            }
        } else if (menu_changed) {
            uint8_t out_buffer[64 * 1024];
            pthread_mutex_lock(&mutex_menu);
            size_t len = draw_menu(menu, out_buffer);
            pthread_mutex_unlock(&mutex_menu);

            for (int j = 1; j < size; ++j) {
                write_to_tcp(sockets[j].fd, out_buffer, len);
                sockets[j].revents = 0;
            }
            menu_changed = 0;
        }
        pthread_mutex_lock(&mutex_menu);
        menu_changed |= check_time(menu, time(NULL));
        pthread_mutex_unlock(&mutex_menu);
    } while (finish == false);
    return NULL;
}


int set_connection(struct sockaddr_in* address) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        syserr("socket");

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = AF_INET;
    addr.sin_port = address->sin_port;

    /* podpięcie się do grupy rozsyłania (ang. multicast) */
    ip_mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    ip_mreq.imr_multiaddr = address->sin_addr;
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*) &ip_mreq, sizeof(struct ip_mreq)) < 0)
        syserr("setsockopt");


    if (bind(sock, (struct sockaddr*) &addr, sizeof(addr)) < 0)
        syserr("bind");
    return sock;
}

void* requesting_loop(void* nil) {
    struct sockaddr_in disc_addr;
    disc_addr.sin_family = AF_INET;
    if (inet_aton(params.DISCOVER_ADDR, &disc_addr.sin_addr) == 0)
        syserr("inet_aton");
    disc_addr.sin_port = htons((uint16_t) params.CTRL_PORT);


    struct pollfd fake;
    fake.fd = -1;
    fake.events = POLL_IN;
    fake.revents = 0;

    char msg[64 * 1024];
    while (finish == false && stop == false) {
        poll(&fake, 1, params.RTIME);
        strcpy(msg, REXMIT);
        size_t point = strlen(REXMIT);
        pthread_mutex_lock(&mutex_buffor);
        if (data_buffor->set != NULL) {
            SetElem* ele = data_buffor->set;
            sprintf(msg + point, "%ld", ele->package->num);
            while (ele->next != data_buffor->set) {
                ele = ele->next;
                sprintf(msg + point, "%ld", ele->package->num);
            }

            pthread_mutex_unlock(&mutex_buffor);
            pthread_mutex_lock(&mutex_ctrl);
            ssize_t len_send = sendto(disc_fd.fd, msg, strlen(msg), 0, (const struct sockaddr*) &disc_addr,
                                      sizeof(struct sockaddr_in));
            pthread_mutex_unlock(&mutex_ctrl);

            if (len_send != strlen(msg))
                syserr("err lookup send");
        } else {
            pthread_mutex_unlock(&mutex_buffor);
        }
        memset(msg, '\0', 64 * 1024);

    }

    return NULL;
}

void* data_loop(void* arg) {
    stop = false;
    Station* active = (Station*) arg;
    uint64_t session = 0;
    uint8_t buffor[64 * 1024];

    bool printing = false;
    bool requesting = false;
    pthread_t printing_thread, requesting_thread;
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0)
        syserr("attr_init");

    int sock = set_connection(&active->address);
    do {
        if (menu_changed) {

            pthread_mutex_lock(&mutex_menu);
            if (menu->active == NULL || menu->active->address.sin_addr.s_addr != active->address.sin_addr.s_addr ||
                menu->active->address.sin_port != active->address.sin_port) {
                pthread_mutex_unlock(&mutex_menu);
                break;
            } else {
                pthread_mutex_unlock(&mutex_menu);
            }
        }
        ssize_t len_recv = recv(sock, buffor, 64 * 1024, MSG_DONTWAIT);
        if(len_recv > 0) {
            uint64_t* s_id = (uint64_t*) buffor;
            uint64_t* f_byt_num = (uint64_t*) (buffor + sizeof(uint64_t));
            uint64_t session_id = ntohll(*s_id);
            uint64_t first_byte_num = ntohll(*f_byt_num);

            if (session == 0) {
                session = session_id;
            } else if (session > session_id) {
                continue;
            } else if (session < session_id) {
                break;
            }

            void* data = buffor + sizeof(uint64_t) * 2;
            pthread_mutex_lock(&mutex_buffor);
            add_to_buffor(data_buffor, first_byte_num, data, len_recv - 2 * sizeof(uint64_t));
            if (data_buffor->fill * 4 > data_buffor->size * 3 && printing == false) {
                pthread_create(&printing_thread, &attr, printing_loop, NULL);
                printing = true;
            }
            if (data_buffor->set != NULL && requesting == false) {
                pthread_create(&requesting_thread, &attr, requesting_loop, NULL);
                requesting = true;
            }
            pthread_mutex_unlock(&mutex_buffor);

            memset(buffor, '\0', 64 * 1024);
        }
    } while (finish == false);


    pthread_mutex_lock(&mutex_buffor);
    reset_buffor(data_buffor);
    pthread_mutex_unlock(&mutex_buffor);
    stop = true;
    if (printing) {
        pthread_join(printing_thread, NULL);
    }
    if (setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*) &ip_mreq, sizeof(struct ip_mreq)) < 0)
        syserr("setsockopt");
    close(sock);
    return NULL;
}


void* printing_loop(void* nil) {
    size_t len;
    pthread_mutex_lock(&mutex_buffor);
    while (data_buffor->fill > 0 && finish == false && stop == false) {
        Package* package = pop_from_buffor(data_buffor);
        len = data_buffor->max_data / data_buffor->size;
        if (package->data == NULL) {
            reset_buffor(data_buffor);
            pthread_mutex_unlock(&mutex_buffor);
            return NULL;
        } else {
            pthread_mutex_unlock(&mutex_buffor);
        }
        write(STDOUT_FILENO, package->data, len);
        remove_package(package);
        pthread_mutex_lock(&mutex_buffor);
    }
    pthread_mutex_unlock(&mutex_buffor);

    return NULL;
}

int parse_recv(char* buffer, char* name, uint16_t* port, char* address) {
    sscanf(buffer, "BOROWICZ_HERE %s %hu %[^\0]", address, port, name);
    if (*port < 0 || *port > UINT16_MAX)
        return -1;
    int temp[4];
    sscanf(address, "%d.%d.%d.%d", &temp[0], &temp[1], &temp[2], &temp[3]);
    for (int i = 0; i < 4; ++i) {
        if (temp[i] < 0 || temp[i] > 255)
            return -1;
    }
    return 0;
}


void set_disc_sock() {
    disc_fd.fd = socket(PF_INET, SOCK_DGRAM, 0);
    disc_fd.events = disc_fd.revents = 0;

    /* uaktywnienie rozgłaszania (ang. broadcast) */
    int optval = 1;
    if (setsockopt(disc_fd.fd, SOL_SOCKET, SO_BROADCAST, (void*) &optval, sizeof optval) < 0)
        syserr("setsockopt broadcast");

    /* ustawienie TTL dla datagramów rozsyłanych do grupy */
    optval = 10;
    if (setsockopt(disc_fd.fd, IPPROTO_IP, IP_MULTICAST_TTL, (void*) &optval, sizeof optval) < 0)
        syserr("setsockopt multicast ttl");

    recieve_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    recieve_addr.sin_port = 0;
    recieve_addr.sin_family = AF_INET;
}


void* request_lookup(void* nil) {
    struct sockaddr_in disc_addr;
    disc_addr.sin_family = AF_INET;
    if (inet_aton(params.DISCOVER_ADDR, &disc_addr.sin_addr) == 0)
        syserr("inet_aton");
    disc_addr.sin_port = htons((uint16_t) params.CTRL_PORT);


    do {
        pthread_mutex_lock(&mutex_ctrl);
        ssize_t len_send = sendto(disc_fd.fd, LOOKUP, strlen(LOOKUP), 0, (const struct sockaddr*) &disc_addr,
                                  sizeof(struct sockaddr_in));
        pthread_mutex_unlock(&mutex_ctrl);

        if (len_send != strlen(LOOKUP))
            syserr("err lookup send");
        poll(&disc_fd, 1, 5000);
    } while (finish == false);

    return NULL;
}

void* recieve_lookup(void* nil) {
    size_t len = 64 * 1024;
    struct sockaddr_in incoming;
    socklen_t addr_len = sizeof(incoming);
    char buffer[len];
    do {
        ssize_t len_recv = recvfrom(disc_fd.fd, buffer, len, 0, (struct sockaddr*) &incoming, &addr_len);
        if (len_recv >= 0 || !(errno == EAGAIN || errno == EWOULDBLOCK)) {
            if (len_recv < 0) {
                syserr("recv");
            } else {
                if (strstr(buffer, REPLY)) {
                    char name[65];
                    uint16_t port;
                    char address[65];
                    if (parse_recv(buffer, name, &port, address) == 0) {
                        struct sockaddr_in addr;
                        addr.sin_family = AF_INET;
                        addr.sin_port = htons(port);
                        if (inet_aton(address, &addr.sin_addr) == 0)
                            syserr("inet_aton");
                        pthread_mutex_lock(&mutex_menu);
                        menu_changed |= add_menu(menu, addr, name, time(NULL));
                        pthread_mutex_unlock(&mutex_menu);
                    }
                }
            }
        }
    } while (finish == false);
    return NULL;
}


int main(int argc, char* argv[]) {
    struct sigaction sig;
    sig.sa_handler = catch_interput;

    if (sigaction(SIGINT, &sig, NULL) != 0) {
        syserr("Unable to change signal handler");
        exit(EXIT_FAILURE);
    }
    parseArgumentsReciver(argc, argv, &params);

    menu = create_menu();
    data_buffor = create_buffor(params.BSIZE);


    pthread_t terminal_thread, request_thread, control_thread, data_thread;
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0)
        syserr("attr_init");

    if (pthread_mutex_init(&mutex_menu, NULL) != 0)
        syserr("create mutex");
    if (pthread_mutex_init(&mutex_buffor, NULL) != 0)
        syserr("create mutex");
    set_disc_sock();


    pthread_create(&terminal_thread, &attr, loop_terminal, NULL);
    pthread_create(&request_thread, &attr, request_lookup, NULL);
    pthread_create(&control_thread, &attr, recieve_lookup, NULL);

    bool running = false;
    while (finish == false) {
        pthread_mutex_lock(&mutex_menu);
        if (menu->active != NULL) {
            Station station = *menu->active;
            pthread_create(&data_thread, &attr, data_loop, &station);
            running = true;
        }
        pthread_mutex_unlock(&mutex_menu);
        if (running) {
            pthread_join(data_thread, NULL);
            running = false;
        }
    }

    pthread_join(control_thread, NULL);
    pthread_join(request_thread, NULL);
    pthread_join(terminal_thread, NULL);


    delete_menu(menu);
    delete_buffor(data_buffor);
    EXIT_SUCCESS;
}