#include <stdbool.h>
#define RAYGUI_IMPLEMENTATION
#include "blackjackclient.h"
#include "gamemenu.h"
#include "gamewindow.h"
#include "raygui.h"
#include "raylib.h"

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
