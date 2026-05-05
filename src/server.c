#include <netdb.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define FIELD_MAX_SIZE 16

int main() {

  char recvbuf[128] = "";

  sqlite3 *gameDB;
  int sqlReturnVal = sqlite3_open("Blackjack.db", &gameDB);
  if (sqlReturnVal != SQLITE_OK) {
    printf("error in sqlite3_open");
    return 1;
  }

  struct addrinfo *addrinfoResult = NULL, *ptr = NULL, hints;
  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  int returnVal;
  returnVal = getaddrinfo(NULL, "27015", &hints, &addrinfoResult);
  if (returnVal != 0) {
    printf("error in getaddrinfo()");
    freeaddrinfo(addrinfoResult);
    return 1;
  }

  int listenSocket =
      socket(addrinfoResult->ai_family, addrinfoResult->ai_socktype,
             addrinfoResult->ai_protocol);
  if (listenSocket == -1) {
    printf("error in socket()");
    freeaddrinfo(addrinfoResult);
    return 1;
  }

  returnVal = bind(listenSocket, addrinfoResult->ai_addr,
                   (int)addrinfoResult->ai_addrlen);
  if (returnVal == -1) {
    printf("error at bind()");
    freeaddrinfo(addrinfoResult);
    close(listenSocket);
    return 1;
  }

  freeaddrinfo(addrinfoResult);

  int clientSocket;

  while (1) {

    if (listen(listenSocket, SOMAXCONN) == -1) {
      printf("error at listen()");
      freeaddrinfo(addrinfoResult);
      close(listenSocket);
      return 1;
    }

    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == -1) {
      printf("error at accept()");
      close(listenSocket);
      return 1;
    }

    close(listenSocket);

    do {
      returnVal = recv(clientSocket, recvbuf, 128, 0);
      if (returnVal > 0) {
        printf("received from client");
      }
    } while (returnVal > 0);
  }
}
