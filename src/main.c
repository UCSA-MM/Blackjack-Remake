#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
//#include "blackjackclient.h"
#include "gamewindow.h"
//#include "gamemenu.h"

// currently prototypes defined just to avoid errors, is part of another file
bool startMenu() {return true;}

int main(void) {
    
    bool isLoggedIn = startMenu();
    GameStart(isLoggedIn);
    return 0;
}

