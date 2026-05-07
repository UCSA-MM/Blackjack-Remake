#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <stdbool.h>

bool serverLogin(char username[], char password[], bool isRegister);
bool updateBal(int balDiff);
bool getBal(int* p_bal);

#endif
