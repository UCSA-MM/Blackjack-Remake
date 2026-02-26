#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui.h"
//#include "blackjackclient.h"
#include "gamewindow.h"
//#include "gamemenu.h"

// currently prototypes defined just to avoid errors, is part of another file
bool startMenu() {return true;}

int main(void) {
    InitWindow(1200, 625, "Blackjack");
    SetTargetFPS(30);
    bool isLoggedIn = startMenu();
    GameStart(isLoggedIn);
    return 0;
}

// TODO:
// [ ] fix the errors in gamewindow.c (as in, start rewriting that stuff too)

