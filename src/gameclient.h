#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <stdbool.h>

bool serverLogin(char username[], char password[], bool isRegister);
bool updateBalance(int balDiff);
int getBalance();

#endif
