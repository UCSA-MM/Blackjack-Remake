#include <netdb.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 128
#define SOCKERR -1
#define SOCKOK 0
#define USER_QSIZE 4     // size of "usr:"
#define PASSWORD_QSIZE 4 // size of "pwd:"

// all of the server code is executed in an infinite loop that stops only on
// program close or on error
// setup is done in the first part of the main, while the rest is done in a
// while loop inside it.
// 1 is returned in case of unwanted exit
//
// the server gets the following queries in input:
// - usr:[username]
// - pwd:[password]
// the following values are sent in return to those queries:
// - usr_ok
// - pwd_ok

int main() {

  char recvbuf[BUFSIZE] = "", username[BUFSIZE] = "", password[BUFSIZE] = "";
  sqlite3 *gameDB;
  int sqlReturnVal, returnVal, qsize;
  int listenSocket, clientSocket;

  struct addrinfo *addrinfoResult = NULL, *ptr = NULL, hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  sqlReturnVal = sqlite3_open("Blackjack.db", &gameDB);
  if (sqlReturnVal != SQLITE_OK) {
    perror("error in sqlite3_open");
    return 1;
  }

  returnVal = getaddrinfo(NULL, "27015", &hints, &addrinfoResult);
  if (returnVal != SOCKOK) {
    perror("error in getaddrinfo()");
    freeaddrinfo(addrinfoResult);
    return 1;
  }

  listenSocket = socket(addrinfoResult->ai_family, addrinfoResult->ai_socktype,
                        addrinfoResult->ai_protocol);
  if (listenSocket == SOCKERR) {
    perror("error in socket()");
    freeaddrinfo(addrinfoResult);
    return 1;
  }

  returnVal = bind(listenSocket, addrinfoResult->ai_addr,
                   (int)addrinfoResult->ai_addrlen);
  if (returnVal == SOCKERR) {
    perror("error at bind()");
    freeaddrinfo(addrinfoResult);
    close(listenSocket);
    return 1;
  }

  freeaddrinfo(addrinfoResult);

  while (1) {

    if (listen(listenSocket, SOMAXCONN) == SOCKERR) {
      perror("error at listen()");
      close(listenSocket);
      return 1;
    }

    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == SOCKERR) {
      perror("error at accept()");
      close(listenSocket);
      close(clientSocket);
      return 1;
    }

    do {
      qsize = recv(clientSocket, recvbuf, BUFSIZE, 0);
      if (qsize > 0) {
        perror("received from client");
      }

      if (strncmp("usr:", recvbuf, USER_QSIZE) == 0) {

        for (int i = 0; i < (int)strlen(recvbuf) - USER_QSIZE; i++) {
          username[i] = recvbuf[i + USER_QSIZE];
        }

        returnVal = send(clientSocket, "usr_ok", qsize, 0);
        if (returnVal == SOCKERR) {
          perror("error at send()");
          close(listenSocket);
          close(clientSocket);
          return 1;
        }
      } else if (strncmp("pwd:", recvbuf, PASSWORD_QSIZE) == 0) {

        for (int i = 0; i < (int)strlen(recvbuf) - PASSWORD_QSIZE; i++) {
          password[i] = recvbuf[i + PASSWORD_QSIZE];
        }

        returnVal = send(clientSocket, "pwd_ok", qsize, 0);
        if (returnVal == SOCKERR) {
          perror("error at send()");
          close(listenSocket);
          close(clientSocket);
          return 1;
        }
      }

    } while (qsize > 0);
  }
}

// general implementation note:
// i could have made the server handle non string values through void pointers.
// At the time of making the original projects i didn't know how that works, now
// i do but i couldn't be bothered to change it :)

// TODO:
// [ ] finish adding detection for the various inputs
// [ ] finish implementing answers
// [ ] implement database
