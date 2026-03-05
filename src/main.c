#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
//#include "blackjackclient.h"
#include "gamewindow.h"
#include "gamemenu.h"

// currently prototypes defined just to avoid errors, is part of another file


int main(void) {
    
    bool isLoggedIn = StartMenu();
    GameStart(isLoggedIn);
    return 0;
}

