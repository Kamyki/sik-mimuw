#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <poll.h>
#include <arpa/inet.h>
#include <ctime>
#include "err.h"
#include "argument_parser.h"
#include "fifo.h"
#include "set.h"
#include <pthread.h>
#include <cerrno>
#include <csignal>
#include <string>
#include <cstring>

#define TTL_VALUE     4
#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))

std::string LOOKUP = "ZERO_SEVEN_COME_IN";
std::string REPLY = "BOREWICZ_HERE";
std::string REXMIT = "LOUDER_PLEASE";
static bool finish = false;
static size_t CTRL_BUF_SIZE = 64 * 1024;
parameters params;

std::set<uint64_t> retransmision_requests;
Fifo fifo;
pthread_mutex_t mutex_requests;
pthread_mutex_t mutex_mcast_socket;
struct ip_mreq ip_mreq;

// 0 - stdin
// 1 - mcast
// 2 - ctrl
static const nfds_t how_many_channels = 3;
struct pollfd channels[3];
uint64_t session_id;


void sendCtrl(struct sockaddr_in* addr, char* message, size_t len) {

    ssize_t send_len = sendto(channels[2].fd, message, len, MSG_DONTWAIT, (const struct sockaddr*) addr,
                              sizeof(struct sockaddr_in));
    if (send_len != static_cast<int>(strlen(message)) &&
        !(send_len == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)))
        syserr("SEND");
}

void setMcastSocket() {
    struct sockaddr_in mcast_addr;

    mcast_addr.sin_family = AF_INET;
    if (inet_aton(params.MCAST_ADDR, &mcast_addr.sin_addr) == 0)
        syserr("inet_aton");
    mcast_addr.sin_port = htons((uint16_t) params.DATA_PORT);

    channels[1].events = POLLOUT;
    channels[1].revents = 0;
    channels[1].fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (channels[1].fd < 0)
        syserr("socket");

    int optval = 1;
    setsockopt(channels[1].fd, SOL_SOCKET, SO_BROADCAST, (void*) &optval, sizeof optval);

    optval = TTL_VALUE;
    setsockopt(channels[1].fd, IPPROTO_IP, IP_MULTICAST_TTL, (void*) &optval, sizeof optval);


    if (connect(channels[1].fd, (struct sockaddr*) &mcast_addr,
                (socklen_t) sizeof(mcast_addr)) < 0)
        syserr("connect");
}

void setCtrlSocket() {
    struct sockaddr_in ctrl_addr;

    channels[2].events = POLLIN;
    channels[2].revents = 0;
    channels[2].fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (channels[2].fd < 0)
        syserr("socket");


    /* podpięcie się pod lokalny adres i port */
    ctrl_addr.sin_family = AF_INET;
    ctrl_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ctrl_addr.sin_port = htons(params.CTRL_PORT);
    if (bind(channels[2].fd, (struct sockaddr*) &ctrl_addr, sizeof(ctrl_addr)) < 0)
        syserr("bind");
}

void send_Mcast(uint8_t* audio_data, uint64_t byte_num) {
    uint8_t audio_buffer[params.PSIZE + sizeof(uint64_t) * 2];

    uint64_t temp = htonll(session_id);
    memcpy(audio_buffer, &temp, sizeof(uint64_t));
    temp = htonll(byte_num);
    memcpy(audio_buffer + sizeof(uint64_t), &temp, sizeof(uint64_t));
    memcpy(audio_buffer + 2 * sizeof(uint64_t), audio_data, params.PSIZE);

    if (write(channels[1].fd, &audio_buffer, sizeof(audio_buffer)) != static_cast<int>(sizeof(audio_buffer)))
        syserr("Error sending");
}


static void catch_interput(int sig) {
    finish = true;
    fprintf(stderr,
            "Signal %d catched. Closing...\n", sig);
}

void setStdin() {
    channels[0].fd = STDIN_FILENO;
    channels[0].events = POLLIN;
    channels[0].revents = 0;
}

void closeAll() {
    for (unsigned i = 1; i < how_many_channels; ++i) {
        if (close(channels[i].fd) < 0)
            perror("close");
        channels[i].fd = -1;
    }
}

void* ctrl_loop(void* nill) {
    char ctrl_buffer[CTRL_BUF_SIZE];
    char replay[256];
    sprintf(replay, "%s %s %ld %s", REPLY.c_str(), params.MCAST_ADDR, params.DATA_PORT, params.NAZWA);
    do {
        poll(&channels[2], 1, 100);
        if (finish == false && (channels[2].revents & POLLIN)) {
            struct sockaddr_in sender;
            socklen_t sender_len = sizeof(sender);

            recvfrom(channels[2].fd, ctrl_buffer, CTRL_BUF_SIZE, 0, (struct sockaddr*) &sender,
                     &sender_len);
            if (strcmp(ctrl_buffer, LOOKUP.c_str()) == 0) {
                sendCtrl(&sender, replay, strlen(replay));
            } else if (strstr(ctrl_buffer, REXMIT.c_str())) {
                char* ptr = ctrl_buffer;
                while ((*ptr) != ' ')
                    ptr += 1;
                pthread_mutex_lock(&mutex_requests);
                add_to_set(retransmision_requests, ptr + 1);
                pthread_mutex_unlock(&mutex_requests);
            }
            memset(ctrl_buffer, '\0', CTRL_BUF_SIZE);
            channels[2].revents = 0;
        }
    } while (finish == false);

    return NULL;
}

void* replay_loop(void* nill) {
    struct pollfd fake;
    fake.fd = -1;
    fake.revents = 0;
    fake.events = POLLOUT;
    do {
        poll(&fake, 1, (int) params.RTIME);
        pthread_mutex_lock(&mutex_requests);
        std::set<uint64_t> copy_requests = retransmision_requests;
        retransmision_requests.clear();
        pthread_mutex_unlock(&mutex_requests);

        pthread_mutex_lock(&mutex_mcast_socket);
        for (auto temp: copy_requests) {
            std::vector<uint8_t> audio_data = fifo.retrieve(params.PSIZE, temp);
            if (audio_data.size() == static_cast<uint64_t>(params.PSIZE)) {
                send_Mcast(&audio_data[0], temp);
            }
        }
        pthread_mutex_unlock(&mutex_mcast_socket);
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
    parseArgumentsTransmiter(argc, argv, &params);
    setCtrlSocket();
    setMcastSocket();
    setStdin();

    fifo.one_pack_size = (uint64_t) params.PSIZE;
    fifo.max_count = (uint64_t) params.FSIZE / params.PSIZE;

    if (pthread_mutex_init(&mutex_requests, NULL) != 0)
        syserr("create mutex");
    if (pthread_mutex_init(&mutex_mcast_socket, NULL) != 0)
        syserr("create mutex");


    session_id = htonl((uint64_t) time(NULL));
    uint64_t byte_num = 0;
    uint8_t audio_data[params.PSIZE];
    pthread_t ctrl_thread, ctrl_replay_thread;
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0)
        syserr("attr_init");

    pthread_create(&ctrl_thread, &attr, ctrl_loop, NULL);
    pthread_create(&ctrl_replay_thread, &attr, replay_loop, NULL);

    srand(time(NULL));


    int ret;
    do {
        /* Czekamy przez RTIME ms */
        ret = poll(channels, 1, 100);
        if (ret < 0 && errno != EINTR)
            syserr("error poll");
        else if (ret > 0) {

            if (finish == false && (channels[0].revents & POLLIN)) {
                ssize_t len_read = read(channels[0].fd, audio_data, (size_t) params.PSIZE);
                if (len_read < 0)
                    syserr("ERR");
                if (len_read == 0) {
                    finish = true;
                }

                if (len_read == params.PSIZE) {
                    pthread_mutex_lock(&mutex_mcast_socket);
                    send_Mcast(audio_data, byte_num);
                    fifo.add_to_queue(audio_data, params.PSIZE, byte_num);
                    byte_num += params.PSIZE;
                    pthread_mutex_unlock(&mutex_mcast_socket);

                }
                memset(audio_data, '\0', params.PSIZE);
                channels[1].revents = 0;
            }

        }
    } while (finish == false);


    pthread_join(ctrl_thread, NULL);
    pthread_join(ctrl_replay_thread, NULL);

    closeAll();

    exit(EXIT_SUCCESS);
}

