#include <stdbool.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"
// #include "blackjackclient.h"
#include "gamemenu.h"
#include "gamewindow.h"

// currently prototypes defined just to avoid errors, is part of another file

int main(void) {

  bool backToMenu, isLoggedIn;
  do {
    isLoggedIn = StartMenu();
    backToMenu = GameStart(isLoggedIn);
  } while (backToMenu == true);
  return 0;
}
// TODO:
// [X] remove temp interaction between closing menu and starting game
// [X] find what causes unwanted texture unloading
