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
//all of the values defined for card size were done with math i forgot about, but they work
#define CARD_WIDTH 0.156f
#define CARD_X_START 0.03f
#define CARD_X_STEP 0.171f
#define DEALER_CARD_Y 0.1f
#define PLAYER_CARD_Y 0.55f
#define CARD_HEIGHT 0.35f
//size * defined value = screen height
//initially it was a window with height 625 and the two values were 32 and 128
#define FONT_SUIT_MULT 19.5f
#define FONT_CARD_MULT 4.8f
#define FONT_DEFAULT_MULT 35.f

void game_DrawCard(card card, Rectangle target);
void game_UpdateSizes();

Rectangle arr_recPlayerCards[MAX_CARD_NUM] = {0};
Rectangle arr_recDealerCards[MAX_CARD_NUM] = {0};

float game_screenWidth = 0.f;
float game_screenHeight = 0.f;
float suit_xFromBorder = 0.f;
float suit_yFromBorder = 0.f;
float font_suitSize = 0.f;
float font_cardSize = 0.f;
float font_defaultSize = 0.f;

Font cardFont;
Font defaultFont;

void GameStart(bool is_logged_in) {

  bool hitButtonPressed = false;
  bool standButtonPressed = false;
  bool surrendButtonPressed = false;
  bool doubledownButtonPressed = false;
  bool betButtonPressed = false;

  Rectangle button_bg;
  Rectangle rec_hitButton;
  Rectangle rec_standButton;
  Rectangle rec_surrendButton;
  Rectangle rec_doubledownButton;
  Rectangle rec_betButton;

  SetConfigFlags(FLAG_MSAA_4X_HINT);

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
  
  //font size inserted here is just a relatively safe size to not have weird stuff.
  //weird stuff can still (and will) happen on high resolution monitors
  cardFont = LoadFontEx("../assets/JQKAs Wild.otf", 128, 0, 250);
  defaultFont = GetFontDefault();
  SetTextureFilter(cardFont.texture, TEXTURE_FILTER_BILINEAR);
  //default font is not set because it is a pixel type font
  
  //start of program
  
  while (!WindowShouldClose()) {

    game_UpdateSizes();

    BeginDrawing();
    
    //draw initial table state

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

    //setup of other UI elements
    
    GuiSetStyle(DEFAULT, TEXT_SIZE, font_defaultSize);
   
    //magic numbers in buttons can just be seen as percentages of the window

    const float button_x = game_screenWidth * 0.91f, button_width = game_screenWidth * 0.08f, button_height = game_screenHeight * 0.05f;

    button_bg.x = game_screenWidth * 0.9f; button_bg.y = 0; button_bg.width = game_screenWidth * 0.1f; button_bg.height = game_screenHeight;
    rec_hitButton.x = rec_standButton.x = rec_surrendButton.x = rec_doubledownButton.x = rec_betButton.x = button_x;
    rec_hitButton.width = rec_standButton.width = rec_surrendButton.width = rec_doubledownButton.width = rec_betButton.width = button_width;
    rec_hitButton.height = rec_standButton.height = rec_surrendButton.height = rec_doubledownButton.height = rec_betButton.height = button_height;
    rec_hitButton.y = game_screenHeight * 0.01f;
    rec_standButton.y = game_screenHeight * 0.08f;
    rec_surrendButton.y = game_screenHeight * 0.15f;
    rec_doubledownButton.y = game_screenHeight * 0.22f;
    rec_betButton.y = game_screenHeight * 0.29f;

    DrawRectangleRec(button_bg, BROWN);
    hitButtonPressed = GuiButton(rec_hitButton, "HIT!");
    standButtonPressed = GuiButton(rec_standButton, "STAND");
    surrendButtonPressed = GuiButton(rec_surrendButton, "SURREND");
    doubledownButtonPressed = GuiButton(rec_doubledownButton, "DOUBLE!");
    betButtonPressed = GuiButton(rec_betButton, "BET!");

    EndDrawing();
  }

  free(gameDeck.cards);
  UnloadFont(cardFont);
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
  
  Vector2 suit_textSize = MeasureTextEx(cardFont, suitStr, font_suitSize, 0);

  float suit_leftX = target.x + suit_xFromBorder - (suit_textSize.x / 2.f);
  float suit_rightX = target.x + target.width - suit_xFromBorder - (suit_textSize.x / 2.f);
  float suit_botY = target.y + suit_yFromBorder - (suit_textSize.y / 2.f);
  float suit_topY = target.y + target.height - suit_yFromBorder - (suit_textSize.y / 2.f);

  Vector2 suit_botLeft = (Vector2){suit_leftX, suit_botY};
  Vector2 suit_botRight = (Vector2){suit_rightX, suit_botY};
  Vector2 suit_topLeft = (Vector2){suit_leftX, suit_topY};
  Vector2 suit_topRight = (Vector2){suit_rightX, suit_topY};

  DrawTextEx(cardFont, suitStr, suit_botLeft, font_suitSize, 0, textColor);
  DrawTextEx(cardFont, suitStr, suit_botRight, font_suitSize, 0, textColor);
  DrawTextEx(cardFont, suitStr, suit_topLeft, font_suitSize, 0, textColor);
  DrawTextEx(cardFont, suitStr, suit_topRight, font_suitSize, 0, textColor);

  char rankStr[3] = "";
  char arr_rankStr[][3] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  
  strcpy(rankStr, arr_rankStr[card.rank - 1]);

  Vector2 rank_textSize = MeasureTextEx(cardFont, rankStr, font_cardSize, 2);

  float rank_x = target.x + (target.width / 2.f) - (rank_textSize.x / 2.f);
  float rank_y = target.y + (target.height / 2.f) - (rank_textSize.y / 2.f);

  Vector2 rank_pos = (Vector2){rank_x, rank_y};

  DrawTextEx(cardFont, rankStr, rank_pos, font_cardSize, 2, textColor);

}

void game_UpdateSizes() {
  
  game_screenWidth = (float)GetScreenWidth();
  game_screenHeight = (float)GetScreenHeight();
  
  for (int i = 0; i < MAX_CARD_NUM; i++) {

    arr_recPlayerCards[i].x = (CARD_X_START + (i * CARD_X_STEP)) * game_screenWidth;
    arr_recDealerCards[i].x = arr_recPlayerCards[i].x;

    arr_recPlayerCards[i].width = CARD_WIDTH * game_screenWidth;
    arr_recDealerCards[i].width = arr_recPlayerCards[i].width;

    arr_recPlayerCards[i].y = PLAYER_CARD_Y * game_screenHeight;
    arr_recDealerCards[i].y = DEALER_CARD_Y * game_screenHeight;

    arr_recPlayerCards[i].height = CARD_HEIGHT * game_screenHeight;
    arr_recDealerCards[i].height = arr_recPlayerCards[i].height;
    
    //we are basically finding the diagonal of a smaller rectangle with the same ratio
    //if x = y * ratio, the diagonal will be sqrtf(y^2 * (1+ratio^2)).
    //we can find y from this result and consequentially find x to get the values to position the suit symbols
    //we are using this on a segment of the diagonal equal to 10% of its size to have a relative position on the card.
    //all the calculations are done relative to the card, not absolute to the window.
    //if this isn't clear, idk how to explain it better without making this too long so have fun
    float cWidth = CARD_WIDTH * game_screenWidth;
    float cHeight = CARD_HEIGHT * game_screenHeight;
    float ratio = cWidth / cHeight;
    float coeff2 = 1 + (ratio * ratio);
    float diagonalSize = sqrtf((cWidth * cWidth) + (cHeight * cHeight));
    float segmentSize = diagonalSize * 0.1f;
    suit_yFromBorder = sqrtf((segmentSize * segmentSize) / coeff2);
    suit_xFromBorder = suit_yFromBorder * ratio;

    font_cardSize = game_screenHeight / FONT_CARD_MULT;
    font_suitSize = game_screenHeight / FONT_SUIT_MULT;
    font_defaultSize = game_screenHeight / FONT_DEFAULT_MULT;

  }
}

// TODO:
// [X] add correct scaling to fonts
// [ ] finish adding UI elements to the game screen 
// [ ] all the other things related to the game logic
