#include <netdb.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  char recvbuf[128] = "";
  char username[128] = "";
  char password[128] = "";

  sqlite3 *gameDB;
  int sqlReturnVal = sqlite3_open("Blackjack.db", &gameDB);
  if (sqlReturnVal != SQLITE_OK) {
    perror("error in sqlite3_open");
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
    perror("error in getaddrinfo()");
    freeaddrinfo(addrinfoResult);
    return 1;
  }

  int listenSocket =
      socket(addrinfoResult->ai_family, addrinfoResult->ai_socktype,
             addrinfoResult->ai_protocol);
  if (listenSocket == -1) {
    perror("error in socket()");
    freeaddrinfo(addrinfoResult);
    return 1;
  }

  returnVal = bind(listenSocket, addrinfoResult->ai_addr,
                   (int)addrinfoResult->ai_addrlen);
  if (returnVal == -1) {
    perror("error at bind()");
    freeaddrinfo(addrinfoResult);
    close(listenSocket);
    return 1;
  }

  freeaddrinfo(addrinfoResult);

  int clientSocket;
  int sendRes;

  while (1) {

    if (listen(listenSocket, SOMAXCONN) == -1) {
      perror("error at listen()");
      close(listenSocket);
      return 1;
    }

    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == -1) {
      perror("error at accept()");
      close(listenSocket);
      close(clientSocket);
      return 1;
    }

    do {
      returnVal = recv(clientSocket, recvbuf, 128, 0);
      if (returnVal > 0) {
        perror("received from client");
      }

      if (strncmp("usr:", recvbuf, 4) == 0) {

        for (int i = 0; i < (int)strlen(recvbuf) - 4; i++) {
          username[i] = recvbuf[i + 4];
        }

        sendRes = send(clientSocket, "usr_ok", returnVal, 0);
        if (sendRes == -1) {
          perror("error at send()");
          close(listenSocket);
          close(clientSocket);
          return 1;
        }
      } else if (strncmp("pwd:", recvbuf, 4) == 0) {

        for (int i = 0; i < (int)strlen(recvbuf) - 4; i++) {
          password[i] = recvbuf[i + 4];
        }

        sendRes = send(clientSocket, "pwd_ok", returnVal, 0);
        if (sendRes == -1) {
          perror("error at send()");
          close(listenSocket);
          close(clientSocket);
          return 1;
        }
      }

    } while (returnVal > 0);
  }
}
