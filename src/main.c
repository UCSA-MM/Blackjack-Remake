#include <stdbool.h>
#define RAYGUI_IMPLEMENTATION
#include "gameclient.h"
#include "gamemenu.h"
#include "gamewindow.h"
#include "raygui.h"
#include "raylib.h"

int main(void) {

  bool backToMenu, isLoggedIn;
  do {
    isLoggedIn = StartMenu();
    backToMenu = GameStart(isLoggedIn);
  } while (backToMenu == true);
  return 0;
}
