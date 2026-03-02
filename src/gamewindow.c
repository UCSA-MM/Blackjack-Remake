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
float suit_xFromBorder = 0.f;
float suit_yFromBorder = 0.f;

Font cardFont;
Font suitFont;

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

  cardFont = LoadFontEx("../assets/JQKAs Wild.otf", 128, 0, 250);
  suitFont = LoadFontEx("../assets/JQKAs Wild.otf", 32, 0, 250);
  
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
  
  char suitStr[2] = "";
  Color textColor;
  
  if (strcmp(card.suit, "Clubs") == 0) {
    strcpy(suitStr, CLUBS);
    textColor = BLACK;
  }
  else if (strcmp(card.suit, "Diamonds") == 0) {
    strcpy(suitStr, DIAMONDS);
    textColor = RED;
  }
  else if (strcmp(card.suit, "Hearts") == 0) {
    strcpy(suitStr, HEARTS);
    textColor = RED;
  }
  else if (strcmp(card.suit, "Spades") == 0) {
    strcpy(suitStr, SPADES);
    textColor = BLACK;
  }

  DrawRectangleRec(target, RAYWHITE);
  
  Vector2 suit_textSize = MeasureTextEx(suitFont, suitStr, (float)suitFont.baseSize, 0);

  float suit_leftX = target.x + suit_xFromBorder - (suit_textSize.x / 2.f);
  float suit_rightX = target.x + target.width - suit_xFromBorder - (suit_textSize.x / 2.f);
  float suit_botY = target.y + suit_yFromBorder - (suit_textSize.y / 2.f);
  float suit_topY = target.y + target.height - suit_yFromBorder - (suit_textSize.y / 2.f);

  Vector2 suit_botLeft = (Vector2){suit_leftX, suit_botY};
  Vector2 suit_botRight = (Vector2){suit_rightX, suit_botY};
  Vector2 suit_topLeft = (Vector2){suit_leftX, suit_topY};
  Vector2 suit_topRight = (Vector2){suit_rightX, suit_topY};

  DrawTextEx(suitFont, suitStr, suit_botLeft, (float)suitFont.baseSize, 0, textColor);
  DrawTextEx(suitFont, suitStr, suit_botRight, (float)suitFont.baseSize, 0, textColor);
  DrawTextEx(suitFont, suitStr, suit_topLeft, (float)suitFont.baseSize, 0, textColor);
  DrawTextEx(suitFont, suitStr, suit_topRight, (float)suitFont.baseSize, 0, textColor);

  char rankStr[3] = "";
  char arr_rankStr[][3] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  
  strcpy(rankStr, arr_rankStr[card.rank - 1]);

  Vector2 rank_textSize = MeasureTextEx(cardFont, rankStr, (float)cardFont.baseSize, 2);

  float rank_x = target.x + (target.width / 2.f) - (rank_textSize.x / 2.f);
  float rank_y = target.y + (target.height / 2.f) - (rank_textSize.y / 2.f);

  Vector2 rank_pos = (Vector2){rank_x, rank_y};

  DrawTextEx(cardFont, rankStr, rank_pos, (float)cardFont.baseSize, 2, textColor);

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
    
    //we are basically finding the diagonal of a smaller rectangle with the same ratio
    //if x = y * ratio, the diagonal will be sqrtf(y^2 * (1+ratio^2)).
    //we can find y from this result and consequentially find x to get the values to position the suit symbols
    //we are using this on a segment of the diagonal equal to 10% of its size to have a relative position on the card.
    //all the calculations are done relative to the card, not absolute to the window.
    //if this isn't clear, idk how to explain it better without making this too long so have fun
    float cWidth = CARD_WIDTH * screenWidth;
    float cHeight = CARD_HEIGHT * screenHeight;
    float ratio = cWidth / cHeight;
    float coeff2 = 1 + (ratio * ratio);
    float diagonalSize = sqrtf((cWidth * cWidth) + (cHeight * cHeight));
    float segmentSize = diagonalSize * 0.1f;
    suit_yFromBorder = sqrtf((segmentSize * segmentSize) / coeff2);
    suit_xFromBorder = suit_yFromBorder * ratio;

  }
}

// TODO:
// - add correct scaling to fonts
// - finish adding UI elements to the game screen 
// - all the other things related to the game logic
