#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H


#include <stdbool.h>

//these values can change. This file is basically used to substitute an actual config file.
#define DECK_NUM 8
#define AI_STAND_SCORE 17
#define START_CARD_NUM 2 //cannot be bigger than 5

bool GameStart(bool is_logged_in);

#endif

