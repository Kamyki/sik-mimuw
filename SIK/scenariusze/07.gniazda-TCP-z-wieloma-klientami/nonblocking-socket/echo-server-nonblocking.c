/*
 Ten program odsyła klientom dane, które od nich otrzymał. Używa poll(),
 aby równocześnie obsługiwać wielu klientów. Gniazda są w trybie
 nieblokującym, dzięki czemu brak możliwości zapisu do jednego gniazda nie
 blokuje obsługi pozostałych klientów.
*/

#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0
#define BUF_SIZE 50000

static int finish = FALSE;

/* Obsługa sygnału kończenia */
static void catch_int (int sig) {
  finish = TRUE;
  fprintf(stderr,
          "Signal %d catched. No new connections will be accepted.\n", sig);
}

int main () {
  struct pollfd client[_POSIX_OPEN_MAX];
  char buf[_POSIX_OPEN_MAX][BUF_SIZE];
  ssize_t buf_len[_POSIX_OPEN_MAX], buf_pos[_POSIX_OPEN_MAX];
  struct sockaddr_in server;
  ssize_t rval, length;
  int msgsock, activeClients, i, ret;

  fprintf(stderr,"_POSIX_OPEN_MAX = %d\n", _POSIX_OPEN_MAX);

  /* Po Ctrl-C kończymy */
  if (signal(SIGINT, catch_int) == SIG_ERR) {
    perror("Unable to change signal handler\n");
    exit(EXIT_FAILURE);
  }

  /* Inicjujemy tablicę z gniazdkami klientów, client[0] to gniazdko centrali */
  for (i = 0; i < _POSIX_OPEN_MAX; ++i) {
    client[i].fd = -1;
    client[i].events = POLLIN;
  }
  activeClients = 0;

  /* Tworzymy gniazdko centrali */
  client[0].fd = socket(PF_INET, SOCK_STREAM, 0);
  if (client[0].fd < 0) {
    perror("Opening stream socket");
    exit(EXIT_FAILURE);
  }

  /* Co do adresu nie jesteśmy wybredni */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = 0;
  if (bind(client[0].fd, (struct sockaddr*)&server,
           (socklen_t)sizeof(server)) < 0) {
    perror("Binding stream socket");
    exit(EXIT_FAILURE);
  }

  /* Dowiedzmy się, jaki to port i obwieśćmy to światu */
  length = sizeof(server);
  if (getsockname (client[0].fd, (struct sockaddr*)&server,
                   (socklen_t*)&length) < 0) {
    perror("Getting socket name");
    exit(EXIT_FAILURE);
  }
  printf("Socket port #%u\n", (unsigned)ntohs(server.sin_port));

  /* Zapraszamy klientów */
  if (listen(client[0].fd, 5) == -1) {
    perror("Starting to listen");
    exit(EXIT_FAILURE);
  }

  /* Do pracy */
  do {
    for (i = 0; i < _POSIX_OPEN_MAX; ++i)
      client[i].revents = 0;
      
    /* Po Ctrl-C zamykamy gniazdko centrali */
    if (finish == TRUE && client[0].fd >= 0) {
      if (close(client[0].fd) < 0)
        perror("close");
      client[0].fd = -1;
    }

    /* Czekamy przez 5000 ms */
    ret = poll(client, _POSIX_OPEN_MAX, 5000);
    if (ret < 0)
      perror("poll");
    else if (ret > 0) {
    
      if (finish == FALSE && (client[0].revents & POLLIN)) {
        /* Przyjmuję nowe połączenie */
        msgsock = accept(client[0].fd, (struct sockaddr*)0, (socklen_t*)0);
        if (msgsock == -1)
          perror("accept");
        else {
          if (fcntl(msgsock, F_SETFL, O_NONBLOCK)<0) /* tryb nieblokujący */
            perror("fcntl");
          for (i = 1; i < _POSIX_OPEN_MAX; ++i) {
            if (client[i].fd == -1) {
              fprintf(stderr, "Received new connection (%d)\n", i);
              client[i].fd = msgsock;
              client[i].events = POLLIN;
              activeClients += 1;
              break;
            }
          }
          if (i >= _POSIX_OPEN_MAX) {
            fprintf(stderr, "Too many clients\n");
            if (close(msgsock) < 0)
              perror("close");
          }
        }
      }
      
      for (i = 1; i < _POSIX_OPEN_MAX; ++i) {
        if (client[i].fd != -1 && (client[i].revents & (POLLIN | POLLERR))) {
          /* Czytam od klienta. */
          rval = read(client[i].fd, buf[i], BUF_SIZE);
          if (rval < 0) {
            perror("Reading stream message");
            if (close(client[i].fd) < 0)
              perror("close");
            client[i].fd = -1;
            activeClients -= 1;
          }
          else if (rval == 0) {
            fprintf(stderr, "Ending connection (%d)\n", i);
            if (close(client[i].fd) < 0)
              perror("close");
            client[i].fd = -1;
            activeClients -= 1;
          }
          else {
            fprintf(stderr, "Received %zd bytes (connection %d)\n", rval, i);
            buf_len[i] = rval;
            buf_pos[i] = 0;
            client[i].events = POLLOUT; /* Teraz chcę pisać, a już nie czytać. */
          }
        }

        if (client[i].fd != -1 && (client[i].revents & POLLOUT)) {
          /* Piszę do klienta. */
          rval = write(client[i].fd, buf[i] + buf_pos[i], buf_len[i] - buf_pos[i]);
          if (rval < 0) {
            perror("write");
            if (close(client[i].fd) < 0)
              perror("close");
            client[i].fd = -1;
            activeClients -= 1;
          }
          else {
            fprintf(stderr, "Sent %zd bytes (connection %d)\n", rval, i);
            buf_pos[i] += rval;
            if (buf_pos[i] == buf_len[i])
              client[i].events = POLLIN; /* Przełączam się na czytanie. */
            
          }
        }
      }
    }
    else
      fprintf(stderr, "Do something else\n");
  } while (finish == FALSE || activeClients > 0);

  if (client[0].fd >= 0)
    if (close(client[0].fd) < 0)
      perror("Closing main socket");
  exit(EXIT_SUCCESS);
}
