#include <stdbool.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"
// #include "blackjackclient.h"
#include "gamemenu.h"
#include "gamewindow.h"

// currently prototypes defined just to avoid errors, is part of another file

int main(void) {

  bool isLoggedIn = StartMenu();
  bool repeat;
  do {
    repeat = GameStart(isLoggedIn);
  } while (repeat == true);
  return 0;
}
