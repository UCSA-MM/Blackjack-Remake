#include "gameclient.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SOCKERR -1
#define SOCKOK 0
#define BUFSIZE 128

int activeSocket;

bool startConnection();

// returns true if the connection was successful, otherwise returns false
bool startConnection() {

  int returnVal;

  struct addrinfo *addrinfoResult = NULL, *ptr = NULL, hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  returnVal = getaddrinfo(NULL, "27015", &hints, &addrinfoResult);
  if (returnVal != SOCKOK) {
    perror("error in getaddrinfo()");
    freeaddrinfo(addrinfoResult);
    return false;
  }

  activeSocket = socket(addrinfoResult->ai_family, addrinfoResult->ai_socktype,
                        addrinfoResult->ai_protocol);
  if (activeSocket == SOCKERR) {
    perror("error in socket()");
    freeaddrinfo(addrinfoResult);
    return false;
  }

  returnVal = connect(activeSocket, addrinfoResult->ai_addr,
                      (int)addrinfoResult->ai_addrlen);
  if (returnVal == SOCKERR) {
    perror("error in connect()");
    freeaddrinfo(addrinfoResult);
    close(activeSocket);
    return false;
  }

  freeaddrinfo(addrinfoResult);
  return true;
}

/*
 * this function deals with the login query for the server. if isRegister is
 * true then starts the registration process.
 *
 * return:
 *    boolean indicating if the operation was successful
 */
bool serverLogin(char username[], char password[], bool isRegister) {

  int returnVal;

  if (!startConnection()) {
    return false;
  }

  char qbuf[BUFSIZE] = "";
  char recvbuf[BUFSIZE] = "";

  sprintf(qbuf, "usr:%s", username);
  returnVal = send(activeSocket, qbuf, BUFSIZE, 0);
  if (returnVal == SOCKERR) {
    perror("error at send()");
    close(activeSocket);
    return false;
  }

  close(activeSocket);
  return true;
}

// TODO:
// [ ] finish adding server queries
// [ ] add handling of answer from server
