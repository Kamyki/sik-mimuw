extern "C" {
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
}

#include <netdb.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <cctype>
#include <poll.h>
#include <csignal>
#include <cerrno>
#include <atomic>
#include <string>
#include <vector>

#include "err.h"
#include "argument_parser.h"
#include "menu.h"
#include "buffor.h"

#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))
#define BACKLOG 10


std::atomic_bool finish;
std::atomic_bool stop;
std::atomic_bool finished_printing;
std::atomic_bool finished_requesting;
std::atomic_bool menu_changed;

parameters params;

std::string LOOKUP = "ZERO_SEVEN_COME_IN\n";
std::string REPLY = "BOREWICZ_HERE";
std::string REXMIT = "LOUDER_PLEASE ";

pthread_mutex_t mutex_menu;
pthread_mutex_t mutex_buffor;
pthread_mutex_t mutex_ctrl;
pthread_t printing_thread, requesting_thread;


Menu menu;
struct pollfd disc_fd;
struct ip_mreq ip_mreq;
struct sockaddr_in recieve_addr;
Buffer data_buffor;


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

    int b = bind(socket_val, (struct sockaddr*) &address, sizeof(address));
    if (b < 0) {
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
        return -1;
    }

    return client_socket;
}


int write_to_tcp(int sock, std::vector<uint8_t>& buffer, size_t len) {
    ssize_t written = send(sock, &buffer[0], len, 0);
    if (written == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        return 0;
    } else if (written != static_cast<ssize_t>(len)) {
        return -1;
    }
    return 1;
}


void* loop_terminal(void* nil) {
    int ret;
    std::vector<pollfd> sockets;
    struct pollfd temp{setTcp(), POLL_IN, 0};

    sockets.push_back(temp);

    u_int8_t in_buffer[128];

    do {
        ret = poll(&sockets[0], static_cast<nfds_t>(sockets.size()), 100);
        if (ret > 0) {
            if (finish == false && (sockets[0].revents & POLLIN)) {
                int client = handleClient(sockets[0].fd);
                if (client > 0) {
                    temp.fd = client;
                    temp.events = POLL_IN;
                    temp.revents = 0;
                    sockets.push_back(temp);
                    sockets[0].revents = 0;
                    std::vector<uint8_t> init = init_seq();
                    write_to_tcp(client, init, 6);
                    std::vector<uint8_t> out_buffer = menu.draw_menu();
                    write_to_tcp(client, out_buffer, out_buffer.size());
                }
            }

            for (auto sock = sockets.begin() + 1; sock != sockets.end(); ++sock) {
                if (finish == false && (sock->revents & POLLIN)) {
                    ssize_t len_red = read(sock->fd, in_buffer, 3);
                    if (len_red < 0)
                        syserr("reading");
                    else if (len_red == 0) {
                        close(sock->fd);
                        sockets.erase(sock);
                        break;
                    }

                    if (len_red == 3) {
                        pthread_mutex_lock(&mutex_menu);
                        bool should_refresh = menu.transform(in_buffer);
                        menu_changed = menu_changed || should_refresh;
                        pthread_mutex_unlock(&mutex_menu);
                        if (should_refresh) {

                            pthread_mutex_lock(&mutex_menu);
                            std::vector<uint8_t> out_buffer = menu.draw_menu();
                            pthread_mutex_unlock(&mutex_menu);

                            for (unsigned j = 1; j < sockets.size(); ++j) {
                                write_to_tcp(sockets[j].fd, out_buffer, out_buffer.size());
                            }
                            break;
                        }
                    }
                }
            }
            for (auto sock = sockets.begin() + 1; sock != sockets.end(); ++sock) {
                sock->revents = 0;
            }
        } else if (menu_changed) {
            pthread_mutex_lock(&mutex_menu);
            std::vector<uint8_t> out_buffer = menu.draw_menu();
            pthread_mutex_unlock(&mutex_menu);

            for (unsigned j = 1; j < sockets.size(); ++j) {
                write_to_tcp(sockets[j].fd, out_buffer, out_buffer.size());
                sockets[j].revents = 0;
            }
            menu_changed = 0;
        }
        pthread_mutex_lock(&mutex_menu);
        menu_changed = menu_changed || menu.check_time(time(nullptr));
        pthread_mutex_unlock(&mutex_menu);
    } while (finish == false);
    for (auto sock : sockets) {
        close(sock.fd);
    }
    return nullptr;
}


int set_connection(struct sockaddr_in* address) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        syserr("socket");

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = AF_INET;
    addr.sin_port = address->sin_port;

    /* podpięcie się do grupy rozsyłania */
    ip_mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    ip_mreq.imr_multiaddr = address->sin_addr;
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*) &ip_mreq, sizeof(struct ip_mreq)) < 0)
        syserr("setsockopt");


    int b = bind(sock, (struct sockaddr*) &addr, sizeof(addr));
    if (b < 0)
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

    while (finish == false && stop == false) {
        poll(&fake, 1, static_cast<int>(params.RTIME));
        pthread_mutex_lock(&mutex_buffor);
        std::map<uint64_t, std::list<Package>::iterator> copy_set = data_buffor.set;
        pthread_mutex_unlock(&mutex_buffor);

        if (!copy_set.empty()) {
            int i = 0;
            std::string msg;
            msg += REXMIT;
            for (auto& elem : copy_set) {
                msg += std::to_string(elem.second->num);
                msg += ",";
                i++;
                if (i == 10) {
                    pthread_mutex_lock(&mutex_ctrl);
                    sendto(disc_fd.fd, msg.c_str(), msg.length() - 1, 0,
                           (const struct sockaddr*) &disc_addr,
                           sizeof(struct sockaddr_in));
                    pthread_mutex_unlock(&mutex_ctrl);
                    msg = REXMIT;
                    i = 0;
                }
            }
            pthread_mutex_lock(&mutex_ctrl);
            sendto(disc_fd.fd, msg.c_str(), msg.length() - 1, 0, (const struct sockaddr*) &disc_addr,
                   sizeof(struct sockaddr_in));
            pthread_mutex_unlock(&mutex_ctrl);
        } else {
            pthread_mutex_unlock(&mutex_buffor);
        }
    }
    return nullptr;
}

void try_close_threads(bool& printing, bool& requesting) {
    if (finished_printing) {
        pthread_join(printing_thread, nullptr);
        finished_printing = false;
        printing = false;
    }
    if (finished_requesting) {
        pthread_join(requesting_thread, nullptr);
        finished_printing = false;
        requesting = false;
    }
}

void* data_loop(void* arg) {
    stop = false;
    Station* active = (Station*) arg;
    uint64_t session = 0;
    uint8_t buffor[64 * 1024];

    bool printing = false;
    bool requesting = false;
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0)
        syserr("attr_init");

    int sock = set_connection(&active->address);


    do {
        if (menu_changed) {

            pthread_mutex_lock(&mutex_menu);
            if (!menu.is_active || menu.active->address.sin_addr.s_addr != active->address.sin_addr.s_addr ||
                menu.active->address.sin_port != active->address.sin_port) {
                pthread_mutex_unlock(&mutex_menu);
                break;
            } else {
                pthread_mutex_unlock(&mutex_menu);
            }
        }
        ssize_t len_recv = recv(sock, buffor, 64 * 1024, MSG_DONTWAIT);
        if (len_recv > 0) {
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
            uint8_t* data = buffor + sizeof(uint64_t) * 2;
            pthread_mutex_lock(&mutex_buffor);
            data_buffor.add_to_buffor(first_byte_num, data, len_recv - 2 * sizeof(uint64_t));
            fprintf(stderr, "%ld\n", first_byte_num);
            pthread_mutex_unlock(&mutex_buffor);
            memset(buffor, '\0', 64 * 1024);
        }
        pthread_mutex_lock(&mutex_buffor);
        if (data_buffor.packages.size() * 4 >= data_buffor.size * 3 && !printing) {
            pthread_create(&printing_thread, &attr, printing_loop, nullptr);
            printing = true;
        }
        if (!data_buffor.set.empty() && !requesting) {
            pthread_create(&requesting_thread, &attr, requesting_loop, nullptr);
            requesting = true;
        }
        pthread_mutex_unlock(&mutex_buffor);
        try_close_threads(printing, requesting);

    } while (finish == false);

    pthread_mutex_lock(&mutex_buffor);
    data_buffor.reset();
    pthread_mutex_unlock(&mutex_buffor);
    stop = true;
    if (printing) {
        pthread_join(printing_thread, nullptr);
        finished_printing = false;
    }
    if (requesting) {
        pthread_join(printing_thread, nullptr);
        finished_requesting = false;
    }
    if (setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*) &ip_mreq, sizeof(struct ip_mreq)) < 0)
        syserr("setsockopt");
    close(sock);
    return nullptr;
}


void* printing_loop(void* nil) {
    size_t len;
    pthread_mutex_lock(&mutex_buffor);
    while (!data_buffor.packages.empty() && finish == false && stop == false) {
        Package package = data_buffor.pop();
        len = package.data.size();
        if (package.data.empty()) {
            data_buffor.reset();
            finished_printing = true;
            pthread_mutex_unlock(&mutex_buffor);
            return nullptr;
        } else {
            pthread_mutex_unlock(&mutex_buffor);
        }
        write(STDOUT_FILENO, &package.data[0], len);
        pthread_mutex_lock(&mutex_buffor);
    }
    pthread_mutex_unlock(&mutex_buffor);
    finished_printing = true;
    return nullptr;
}

int parse_recv(char* buffer, char* name, uint16_t* port, char* address) {
    sscanf(buffer, "BOREWICZ_HERE %s %hu", address, port);
    if (*port < 0 || *port > UINT16_MAX)
        return -1;
    int temp[4];
    sscanf(address, "%d.%d.%d.%d", &temp[0], &temp[1], &temp[2], &temp[3]);
    for (int i = 0; i < 4; ++i) {
        if (temp[i] < 0 || temp[i] > 255)
            return -1;
    }
    int spaces = 0;
    char* p = buffer;
    while (spaces < 3) {
        p++;
        if (*p == ' ')
            spaces++;
    }
    memcpy(name, p + 1, 45);
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
        sendto(disc_fd.fd, LOOKUP.c_str(), LOOKUP.length(), 0, (const struct sockaddr*) &disc_addr,
               sizeof(struct sockaddr_in));
        pthread_mutex_unlock(&mutex_ctrl);


        poll(&disc_fd, 1, 5000);
    } while (finish == false);

    return nullptr;
}

void* recieve_lookup(void* nil) {
    size_t len = 64 * 1024;
    struct sockaddr_in incoming;
    socklen_t addr_len = sizeof(incoming);
    char buffer[len];
    do {
        ssize_t len_recv = recvfrom(disc_fd.fd, buffer, len, MSG_DONTWAIT, (struct sockaddr*) &incoming, &addr_len);
        if (len_recv >= 0 || !(errno == EAGAIN || errno == EWOULDBLOCK)) {
            if (len_recv < 0) {
                syserr("recv");
            } else {
                if (strstr(buffer, REPLY.c_str())) {
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
                        menu_changed = menu_changed || menu.add_menu(addr, name, time(nullptr));
                        pthread_mutex_unlock(&mutex_menu);
                    }
                }
                memset(buffer, '\0', len);
            }
        }
    } while (finish == false);
    return nullptr;
}


int main(int argc, char* argv[]) {
    struct sigaction sig;
    sig.sa_handler = catch_interput;

    if (sigaction(SIGINT, &sig, nullptr) != 0) {
        syserr("Unable to change signal handler");
        exit(EXIT_FAILURE);
    }
    parseArgumentsReciver(argc, argv, &params);

    if (strlen(params.NAZWA) > 0) {
        menu.prefered_set = true;
        menu.prefered_name = std::string(params.NAZWA);

    }

    finish = false;
    stop = false;
    finished_printing = false;
    finished_requesting = false;
    menu_changed = false;

    data_buffor.max_data = params.BSIZE;

    pthread_t terminal_thread, request_thread, control_thread, data_thread;
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0)
        syserr("attr_init");

    if (pthread_mutex_init(&mutex_menu, nullptr) != 0)
        syserr("create mutex");
    if (pthread_mutex_init(&mutex_buffor, nullptr) != 0)
        syserr("create mutex");
    set_disc_sock();


    pthread_create(&terminal_thread, &attr, loop_terminal, nullptr);
    pthread_create(&request_thread, &attr, request_lookup, nullptr);
    pthread_create(&control_thread, &attr, recieve_lookup, nullptr);

    bool running = false;
    while (finish == false) {
        pthread_mutex_lock(&mutex_menu);
        if (menu.is_active) {
            Station station = *menu.active;
            pthread_create(&data_thread, &attr, data_loop, &station);
            running = true;
        }
        pthread_mutex_unlock(&mutex_menu);
        if (running) {
            pthread_join(data_thread, nullptr);
            running = false;
        }
    }

    pthread_join(control_thread, nullptr);
    pthread_join(request_thread, nullptr);
    pthread_join(terminal_thread, nullptr);

    close(disc_fd.fd);


    return 0;
}