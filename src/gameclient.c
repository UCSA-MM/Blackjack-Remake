#include "gameclient.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int returnVal;
int activeSocket;

bool startConnection();

bool startConnection() {

  struct addrinfo *addrinfoResult = NULL, *ptr = NULL, hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  returnVal = getaddrinfo(NULL, "27015", &hints, &addrinfoResult);
  if (returnVal != 0) {
    perror("error in getaddrinfo()");
    freeaddrinfo(addrinfoResult);
    return false;
  }

  activeSocket = socket(addrinfoResult->ai_family, addrinfoResult->ai_socktype,
                        addrinfoResult->ai_protocol);
  if (activeSocket == -1) {
    perror("error in socket()");
    freeaddrinfo(addrinfoResult);
    return false;
  }

  returnVal = connect(activeSocket, addrinfoResult->ai_addr,
                      (int)addrinfoResult->ai_addrlen);
  if (returnVal == -1) {
    perror("error in connect()");
    freeaddrinfo(addrinfoResult);
    close(activeSocket);
    return false;
  }

  freeaddrinfo(addrinfoResult);
  return true;
}

bool serverLogin(char username[], char password[], bool isRegister) {

  if (!startConnection()) {
    return false;
  }

  char qbuf[32] = "";
  char recvbuf[32] = "";

  sprintf(qbuf, "Username:%s", username);
  returnVal = send(activeSocket, qbuf, 32, 0);
  if (returnVal == -1) {
    perror("error at send()");
    close(activeSocket);
    return false;
  }

  close(activeSocket);
  return true;
}
