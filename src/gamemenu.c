#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "raygui.h"
#include "gamemenu.h"
//#include "blackjackclient.h"

float menu_screenWidth = 0.f;
float menu_screenHeight = 0.f;

bool StartMenu() {
  
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(0, 0, "Blackjack Login");
  SetWindowSize(GetMonitorWidth(GetCurrentMonitor()) / 2, GetMonitorHeight(GetCurrentMonitor()) / 2);
  SetTargetFPS(30);

  menu_screenWidth = (float)GetScreenWidth();
  menu_screenHeight = (float)GetScreenHeight();

  Vector2 titleVec;
  titleVec.x = menu_screenWidth * 0.03f; titleVec.y = menu_screenHeight * 0.03f;

  Font titleFont = LoadFontEx("../assets/cardcharacters.ttf", 128, 0, 250);
  
  while(!WindowShouldClose()) {
    
    BeginDrawing();
    
    ClearBackground(DARKGREEN);

    DrawTextEx(titleFont, "BLACKJACK", titleVec, 128, 5, WHITE);
    
    EndDrawing();
  }


  CloseWindow();
  return true;
}

