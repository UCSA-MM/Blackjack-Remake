#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "raylib.h"
#include "raygui.h"
#include "cards.h"
//#include "blackjackclient.h"
#include "gamewindow.h"

//game options are in the header file
#define MAX_CARD_NUM 5
#define CARD_WIDTH 0.156f
#define CARD_X_START 0.03f
#define CARD_X_STEP 0.171f
#define DEALER_CARD_Y 0.1f
#define PLAYER_CARD_Y 0.55f
#define CARD_HEIGHT 0.35f

void game_DrawCard(card card, Rectangle target);
void game_UpdateSizes();

Rectangle arr_recPlayerCards[MAX_CARD_NUM] = {0};
Rectangle arr_recDealerCards[MAX_CARD_NUM] = {0};

float screenWidth = 0.f;
float screenHeight = 0.f;

void GameStart(bool is_logged_in) {

  InitWindow(0, 0, "Blackjack");
  SetWindowSize(GetMonitorWidth(GetCurrentMonitor()) / 2, GetMonitorHeight(GetCurrentMonitor()) / 2);
  SetTargetFPS(30);

  deck gameDeck = FillDeck(DECK_NUM);
  ShuffleDeck(&gameDeck);
  card playerHand[5] = {0};
  card dealerHand[5] = {0};

  for (int i = 0; i < START_CARD_NUM; i++) {
      playerHand[i] = DrawCard(&gameDeck);
      dealerHand[i] = DrawCard(&gameDeck);
  }

  int playerCardNum, dealerCardNum;
  playerCardNum = dealerCardNum = START_CARD_NUM;
  int playerHandScore = CalcScore(playerHand, playerCardNum);
  int dealerCardScore = CalcScore(dealerHand, dealerCardNum);

  
  //start of program
  
  while (!WindowShouldClose()) {

    game_UpdateSizes();

    BeginDrawing();

    ClearBackground(DARKGREEN);

    for (int i = 0; i < playerCardNum; i++) {
      game_DrawCard(playerHand[i], arr_recPlayerCards[i]);
    }
    for (int i = playerCardNum; i < MAX_CARD_NUM; i++) {
      DrawRectangleRec(arr_recPlayerCards[i], GREEN);
    }

    for (int i = 0; i < MAX_CARD_NUM; i++) {

      switch (i < playerCardNum) {
        case true:
          game_DrawCard(playerHand[i], arr_recPlayerCards[i]);
          break;
        case false:
          DrawRectangleRec(arr_recPlayerCards[i], GREEN);
          break;
      }
    }

    if (dealerCardNum == START_CARD_NUM) {
      
      //only the first card is shown to the player while the others are face down
      game_DrawCard(dealerHand[0], arr_recDealerCards[0]);

      for (int i = 1; i < MAX_CARD_NUM; i++) {

        switch (i < dealerCardNum) {
          case true:
            DrawRectangleRec(arr_recDealerCards[i], DARKBLUE);
            break;
          case false:
            DrawRectangleRec(arr_recDealerCards[i], GREEN);
            break;
        }
      }

    }

    EndDrawing();
  }

  free(gameDeck.cards);
  exit(0);
    
}

void game_DrawCard(card card, Rectangle target) {
  //card text to be implemented
  DrawRectangleRec(target, RAYWHITE);
}

void game_UpdateSizes() {
  
  screenWidth = (float)GetScreenWidth();
  screenHeight = (float)GetScreenHeight();
  
  for (int i = 0; i < MAX_CARD_NUM; i++) {

    arr_recPlayerCards[i].x = (CARD_X_START + (i * CARD_X_STEP)) * screenWidth;
    arr_recDealerCards[i].x = arr_recPlayerCards[i].x;

    arr_recPlayerCards[i].width = CARD_WIDTH * screenWidth;
    arr_recDealerCards[i].width = arr_recPlayerCards[i].width;

    arr_recPlayerCards[i].y = PLAYER_CARD_Y * screenHeight;
    arr_recDealerCards[i].y = DEALER_CARD_Y * screenHeight;

    arr_recPlayerCards[i].height = CARD_HEIGHT * screenHeight;
    arr_recDealerCards[i].height = arr_recPlayerCards[i].height;
  }
}
