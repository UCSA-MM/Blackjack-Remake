#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui.h"

// currently prototypes defined just to avoid errors, is part of another file
bool startMenu() {return true;}
void gameStart(bool is_logged_in) {}

int main(void) {
    InitWindow(1200, 625, "Blackjack");
    SetTargetFPS(30);
    bool isLoggedIn = startMenu();
    gameStart(isLoggedIn);
    return 0;
}


