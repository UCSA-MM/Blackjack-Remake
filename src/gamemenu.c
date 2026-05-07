#include "gamemenu.h"
#include "gameclient.h"
#include "raygui.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define WHITE_32 0xffffffff
#define BLUE_TRANSP_32 0x97E8FF80

// floats can be seen as percentages of screen size
#define TITLEVEC_RELX 0.03f
#define TITLEVEC_RELY 0.03f
#define TITLE_FONTSIZE 128
#define FONT_CHARNUM 250
#define START_BUTTON_RELX 0.05f
#define START_BUTTON_RELW 0.3f
#define START_BUTTON_RELH 0.1f
#define PLAY_RELY 0.3f
#define LOGIN_RELY (PLAY_RELY + 0.15f)
#define REGISTER_RELY (LOGIN_RELY + 0.15f)

#define FIELD_MAX_SIZE 16
#define ACTION_STRSIZE 10
#define ACC_LABEL_RELX 0.1f
#define ACC_ULABEL_RELY 0.15f
#define ACC_PLABEL_RELY (ACC_ULABEL_RELY + 0.25f)
#define INPUT_RELX 0.1f
#define INPUT_USER_RELY 0.2f
#define INPUT_PASS_RELY (INPUT_USER_RELY + 0.25f)
#define INPUT_RELW 0.5f
#define INPUT_RELH 0.15f
#define ACTION_RELX 0.12f
#define BACK_RELX 0.40f
#define ACC_BUTTON_RELY 0.75f
#define ACC_BUTTON_RELW 0.25f
#define ACC_BUTTON_RELH 0.1f
#define INPUT_FONTSIZE 32

bool AccountInterface(bool isRegister);

float menu_screenWidth = 0.f;
float menu_screenHeight = 0.f;
Font menu_defaultFont, menu_titleFont;

/*
 * this function handles drawing the main menu and operations related to it.
 * on button presses, it can call function for login screen creation or return a
 * value to main for game start.
 *
 * return:
 *    if a login operation was successful (so if the player is connected to the
 *    server)
 */
bool StartMenu() {

  Rectangle rec_PlayButton, rec_LoginButton, rec_RegisterButton;
  bool playButtonPressed, loginButtonPressed, registerButtonPressed;
  bool isCollidingWithMenuButtons;
  bool isOnline = false;

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
  titleVec.x = menu_screenWidth * TITLEVEC_RELX;
  titleVec.y = menu_screenHeight * TITLEVEC_RELY;

  menu_titleFont = LoadFontEx("../assets/cardcharacters.ttf", TITLE_FONTSIZE, 0,
                              FONT_CHARNUM);

  rec_PlayButton.x = rec_LoginButton.x = rec_RegisterButton.x =
      menu_screenWidth * START_BUTTON_RELX;
  rec_PlayButton.width = rec_LoginButton.width = rec_RegisterButton.width =
      menu_screenWidth * START_BUTTON_RELW;
  rec_PlayButton.height = rec_LoginButton.height = rec_RegisterButton.height =
      menu_screenHeight * START_BUTTON_RELH;
  rec_PlayButton.y = menu_screenHeight * PLAY_RELY;
  rec_LoginButton.y = menu_screenHeight * LOGIN_RELY;
  rec_RegisterButton.y = menu_screenHeight * REGISTER_RELY;

  while (!WindowShouldClose()) {

    BeginDrawing();

    ClearBackground(DARKGREEN);

    DrawTextEx(menu_titleFont, "BLACKJACK", titleVec, TITLE_FONTSIZE, 5, WHITE);

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

/*
 * this function draws the login and register interface and handles the calls to
 * operations from gameclient.h
 *
 * return:
 *    if a login operation was successful or not
 */
bool AccountInterface(bool isRegister) {

  bool usernameTextEdit = false, passwordTextEdit = false;
  bool actionButtonPressed = false, backButtonPressed = false;
  bool qResult;
  Rectangle rec_UserInput, rec_PassInput;
  Rectangle rec_ActionButton, rec_BackButton;
  char str_Username[FIELD_MAX_SIZE] = "";
  char str_Password[FIELD_MAX_SIZE] = "";
  char str_ActionText[ACTION_STRSIZE] = "";
  Vector2 vec_UsernameLabelPos = (Vector2){menu_screenWidth * ACC_LABEL_RELX,
                                           menu_screenHeight * ACC_ULABEL_RELY},
          vec_PasswordLabelPos = (Vector2){vec_UsernameLabelPos.x,
                                           menu_screenHeight * ACC_PLABEL_RELY};
  rec_UserInput.x = rec_PassInput.x = menu_screenWidth * INPUT_RELX;
  rec_UserInput.y = menu_screenHeight * INPUT_USER_RELY;
  rec_PassInput.y = menu_screenHeight * INPUT_PASS_RELY;
  rec_UserInput.width = rec_PassInput.width = menu_screenWidth * INPUT_RELW;
  rec_UserInput.height = rec_PassInput.height = menu_screenHeight * INPUT_RELH;
  rec_ActionButton.x = menu_screenWidth * ACTION_RELX;
  rec_BackButton.x = menu_screenWidth * BACK_RELX;
  rec_ActionButton.y = rec_BackButton.y = menu_screenHeight * ACC_BUTTON_RELY;
  rec_ActionButton.width = rec_BackButton.width =
      menu_screenWidth * ACC_BUTTON_RELW;
  rec_ActionButton.height = rec_BackButton.height =
      menu_screenHeight * ACC_BUTTON_RELH;
  if (isRegister) {
    strcpy(str_ActionText, "REGISTER");
  } else {
    strcpy(str_ActionText, "LOGIN");
  }

  while (!WindowShouldClose()) {

    BeginDrawing();

    ClearBackground(DARKGREEN);

    if (GuiTextBox(rec_UserInput, str_Username, FIELD_MAX_SIZE - 1,
                   usernameTextEdit)) {
      usernameTextEdit = !usernameTextEdit;
    }
    if (GuiTextBox(rec_PassInput, str_Password, FIELD_MAX_SIZE - 1,
                   passwordTextEdit)) {
      passwordTextEdit = !passwordTextEdit;
    }

    DrawTextEx(menu_defaultFont, "USERNAME", vec_UsernameLabelPos,
               INPUT_FONTSIZE, 5, WHITE);
    DrawTextEx(menu_defaultFont, "PASSWORD", vec_PasswordLabelPos,
               INPUT_FONTSIZE, 5, WHITE);

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
      qResult = serverLogin(str_Username, str_Password, isRegister);
      return qResult;
    }
  }

  return false;
}

// TODO:
// [ ] add functionality to the buttons
// [ ] add a good starting position for the window
