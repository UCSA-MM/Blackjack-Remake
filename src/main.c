#include <stdbool.h>
#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui.h"
// #include "blackjackclient.h"
#include "gamemenu.h"
#include "gamewindow.h"

// currently prototypes defined just to avoid errors, is part of another file

int main(void) {

  bool isLoggedIn = StartMenu();
  GameStart(isLoggedIn);
  return 0;
}
