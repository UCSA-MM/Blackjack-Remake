#include "gamemenu.h"
#include "raygui.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
// #include "blackjackclient.h"

#define WHITE_32 0xffffffff
#define BLUE_TRANSP_32 0x97E8FF80

bool AccountInterface(bool isRegister);

float menu_screenWidth = 0.f;
float menu_screenHeight = 0.f;
Font menu_defaultFont, menu_titleFont;

bool StartMenu() {

  Rectangle rec_PlayButton, rec_LoginButton, rec_RegisterButton;

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  // the window is technically not resizable but on WSL it is not working for
  // some reason, cool stuff
  GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
  GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, WHITE_32);
  GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, WHITE_32);
  GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, WHITE_32);
  GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED, BLUE_TRANSP_32);

  InitWindow(0, 0, "Blackjack Login");
  SetWindowSize(GetMonitorWidth(GetCurrentMonitor()) / 2,
                GetMonitorHeight(GetCurrentMonitor()) / 2);
  SetTargetFPS(30);

  menu_defaultFont = GetFontDefault();
  GuiSetFont(menu_defaultFont);

  menu_screenWidth = (float)GetScreenWidth();
  menu_screenHeight = (float)GetScreenHeight();

  Vector2 titleVec;
  titleVec.x = menu_screenWidth * 0.03f;
  titleVec.y = menu_screenHeight * 0.03f;

  menu_titleFont = LoadFontEx("../assets/cardcharacters.ttf", 128, 0, 250);

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
  bool isOnline = false;

  while (!WindowShouldClose()) {

    BeginDrawing();

    ClearBackground(DARKGREEN);

    DrawTextEx(menu_titleFont, "BLACKJACK", titleVec, 128, 5, WHITE);

    playButtonPressed = GuiButton(rec_PlayButton, "PLAY");
    loginButtonPressed = GuiButton(rec_LoginButton, "LOGIN");
    registerButtonPressed = GuiButton(rec_RegisterButton, "REGISTER");

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

    if (playButtonPressed) {
      UnloadFont(menu_titleFont);
      CloseWindow();
      return isOnline;
    } else if (loginButtonPressed) {
      isOnline = AccountInterface(false);
    } else if (registerButtonPressed) {
      isOnline = AccountInterface(true);
    }
  }

  UnloadFont(menu_titleFont);
  CloseWindow();
  exit(0);
}

bool AccountInterface(bool isRegister) {

  bool usernameTextEdit = false, passwordTextEdit = false;
  bool actionButtonPressed = false, backButtonPressed = false;
  Rectangle rec_UserInput, rec_PassInput;
  Rectangle rec_ActionButton, rec_BackButton;
  char str_Username[32] = "";
  char str_Password[32] = "";
  char str_ActionText[10] = "";
  Vector2 vec_UsernameLabelPos =
              (Vector2){menu_screenWidth * 0.1f, menu_screenHeight * 0.15f},
          vec_PasswordLabelPos =
              (Vector2){vec_UsernameLabelPos.x, menu_screenHeight * 0.4f};
  rec_UserInput.x = rec_PassInput.x = menu_screenWidth * 0.1f;
  rec_UserInput.y = menu_screenHeight * 0.2f;
  rec_PassInput.y = menu_screenHeight * 0.45f;
  rec_UserInput.width = rec_PassInput.width = menu_screenWidth * 0.5f;
  rec_UserInput.height = rec_PassInput.height = menu_screenHeight * 0.15f;
  rec_ActionButton.x = menu_screenWidth * 0.12f;
  rec_BackButton.x = menu_screenWidth * 0.40f;
  rec_ActionButton.y = rec_BackButton.y = menu_screenHeight * 0.75f;
  rec_ActionButton.width = rec_BackButton.width = menu_screenWidth * 0.25f;
  rec_ActionButton.height = rec_BackButton.height = menu_screenHeight * 0.1f;
  if (isRegister) {
    strcpy(str_ActionText, "REGISTER");
  } else {
    strcpy(str_ActionText, "LOGIN");
  }

  while (!WindowShouldClose()) {

    BeginDrawing();

    ClearBackground(DARKGREEN);

    if (GuiTextBox(rec_UserInput, str_Username, 32 - 1, usernameTextEdit)) {
      usernameTextEdit = !usernameTextEdit;
    }
    if (GuiTextBox(rec_PassInput, str_Password, 32 - 1, passwordTextEdit)) {
      passwordTextEdit = !passwordTextEdit;
    }

    DrawTextEx(menu_defaultFont, "USERNAME", vec_UsernameLabelPos, 32, 5,
               WHITE);
    DrawTextEx(menu_defaultFont, "PASSWORD", vec_PasswordLabelPos, 32, 5,
               WHITE);

    actionButtonPressed = GuiButton(rec_ActionButton, str_ActionText);
    backButtonPressed = GuiButton(rec_BackButton, "BACK");

    if (CheckCollisionPointRec(GetMousePosition(), rec_UserInput) ||
        CheckCollisionPointRec(GetMousePosition(), rec_PassInput)) {
      SetMouseCursor(MOUSE_CURSOR_IBEAM);
    } else if (CheckCollisionPointRec(GetMousePosition(), rec_ActionButton) ||
               CheckCollisionPointRec(GetMousePosition(), rec_BackButton)) {
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    EndDrawing();

    if (backButtonPressed) {
      return false;
    } else if (actionButtonPressed) {
      return false;
    }
  }

  return false;
}

// TODO:
// [X] Add a button to start playing the game to replace the current window
// closing system
// [X] create a button to start a login or registration procedure
// [X] create the login/register UI
// [ ] add functionality to the buttons
// [X] fix the size of buttons in the register UI

// NOTE:
// login/register UI *must* be able to store data inserted by the user so that
// it can be later sent to the server
