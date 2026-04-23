#include "gamemenu.h"
#include "raygui.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
// #include "blackjackclient.h"

bool AccountInterface(bool isRegister);

float menu_screenWidth = 0.f;
float menu_screenHeight = 0.f;
Rectangle rec_PlayButton, rec_LoginButton, rec_RegisterButton;
Font defaultFont, titleFont;

bool StartMenu() {

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(0, 0, "Blackjack Login");
  SetWindowSize(GetMonitorWidth(GetCurrentMonitor()) / 2,
                GetMonitorHeight(GetCurrentMonitor()) / 2);
  SetTargetFPS(30);

  defaultFont = GetFontDefault();
  GuiSetFont(defaultFont);

  menu_screenWidth = (float)GetScreenWidth();
  menu_screenHeight = (float)GetScreenHeight();

  Vector2 titleVec;
  titleVec.x = menu_screenWidth * 0.03f;
  titleVec.y = menu_screenHeight * 0.03f;

  titleFont = LoadFontEx("../assets/cardcharacters.ttf", 128, 0, 250);

  rec_PlayButton.x = rec_LoginButton.x = rec_RegisterButton.x =
      menu_screenWidth * 0.05f;
  rec_PlayButton.width = rec_LoginButton.width = rec_RegisterButton.width =
      menu_screenWidth * 0.3f;
  rec_PlayButton.height = rec_LoginButton.height = rec_RegisterButton.height =
      menu_screenHeight * 0.1f;
  rec_PlayButton.y = menu_screenHeight * 0.3f;
  rec_LoginButton.y = menu_screenHeight * 0.45f;
  rec_RegisterButton.y = menu_screenHeight * 0.6f;

  bool playButtonPressed, loginButtonPressed, registerButtonPressed;
  bool isCollidingWithMenuButtons;
  bool isOnline;

  while (!WindowShouldClose()) {

    BeginDrawing();

    ClearBackground(DARKGREEN);

    DrawTextEx(titleFont, "BLACKJACK", titleVec, 128, 5, WHITE);

    playButtonPressed = GuiButton(rec_PlayButton, "PLAY");
    loginButtonPressed = GuiButton(rec_LoginButton, "LOGIN");
    registerButtonPressed = GuiButton(rec_RegisterButton, "REGISTER");

    if (playButtonPressed) {
      UnloadFont(titleFont);
      CloseWindow();
      return isOnline;
    } else if (loginButtonPressed) {
      isOnline = AccountInterface(false);
    } else if (registerButtonPressed) {
      isOnline = AccountInterface(true);
    }

    Vector2 mousePos = GetMousePosition();
    isCollidingWithMenuButtons =
        CheckCollisionPointRec(mousePos, rec_PlayButton) ||
        CheckCollisionPointRec(mousePos, rec_LoginButton) ||
        CheckCollisionPointRec(mousePos, rec_RegisterButton);
    if (isCollidingWithMenuButtons) {
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    EndDrawing();
  }

  UnloadFont(titleFont);
  CloseWindow();
  exit(0);
}

bool AccountInterface(bool isRegister) { return false; }

// TODO:
// [X] Add a button to start playing the game to replace the current window
// closing system
// [X] create a button to start a login or registration procedure
// [ ] create the login/register UI

// NOTE:
// login/register UI *must* be able to store data inserted by the user so that
// it can be later sent to the server
