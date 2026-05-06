#include "gamewindow.h"
#include "cards.h"
#include "gameclient.h"
#include "raygui.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// game options not tied to the interface (and that can be changed without
// causing issues, at least i think) are located as define statements in the
// header.
#define MAX_CARD_NUM 5
// values here can be seen as percentages of the screen
#define CARD_WIDTH 0.156f
#define CARD_X_START 0.03f
#define CARD_X_STEP 0.171f
#define DEALER_CARD_Y 0.1f
#define PLAYER_CARD_Y 0.55f
#define CARD_HEIGHT 0.35f
#define BUTTON_RELX 0.91f
#define BUTTON_RELW 0.08f
#define BUTTON_RELH 0.05f
#define BUTTON_BG_RELX 0.9f
#define BUTTON_BG_RELY 0
#define BUTTON_BG_RELW 0.1f
#define BUTTON_BG_RELH 1.f
#define HIT_BUTTON_RELY 0.01f
#define STAND_BUTTON_RELY HIT_BUTTON_RELY + 0.07f
#define SURREND_BUTTON_RELY STAND_BUTTON_RELY + 0.07f
#define DOUBLEDOWN_BUTTON_RELY SURREND_BUTTON_RELY + 0.07f
#define BET_BUTTON_RELY DOUBLEDOWN_BUTTON_RELY + 0.07f
#define END_MESSAGE_RELX 0.2f
#define END_MESSAGE_RELY 0.3f
#define END_MESSAGE_RELW 0.5f
#define END_MESSAGE_RELH 0.4f
#define END_RETRY_RELX 0.4f
#define END_RETRY_RELY 0.5f
#define END_RETRY_RELW 0.1f
#define END_RETRY_RELH 0.05f
#define END_TEXT_RELX 0.45f
#define END_TEXT_RELY 0.4f

// these numbers have no real meaning aside from the fact that they didn't look
// bad. The usage is that size * defined value = screen height.
// This was obtained from other arbitrary values that were initially defined on
// a window with height of 625px, then they were converted for my current laptop
// screen height. It is one of the worst ways to implement this i can think of
// but it works.
#define FONT_SUIT_MULT 19.5f
#define FONT_CARD_MULT 4.8f
#define FONT_DEFAULT_MULT 30.f
#define FONT_ENDMSG_MULT 10.f

// the card font used has this characters as suits or other special characters.
// NARROW10 is a 10 not composed of the sequence of 1 and 0 but made of a
// standalone glyph. all the values are strings because i think them being
// strings is needed for the text drawing functions from raylib.
#define CLUBS "]"
#define DIAMONDS "["
#define HEARTS "{"
#define SPADES "}"
#define NARROW10 "="

typedef enum endgame_state {
  NO = 0,
  WIN,
  LOSE,
  BUST,
  CHARLIE,
  BLACKJACK,
  DRAW
} endgame_state;

void game_DrawCard(card card, Rectangle target);
// function used to check for updates to the size of the window and to resize
// the elements accordingly
void game_UpdateSizes();
void game_UpdateCursor();
bool DrawEndgameScreen(endgame_state flag_endgame_state);

Rectangle arr_recPlayerCards[MAX_CARD_NUM], arr_recDealerCards[MAX_CARD_NUM];

Rectangle button_bg, rec_hitButton, rec_standButton, rec_surrendButton,
    rec_doubledownButton, rec_betButton;
Rectangle rec_endMessage_bg, rec_retryButton;

Vector2 vec_endgameText;

float game_screenWidth, game_screenHeight;
float suit_xFromBorder, suit_yFromBorder;
float font_suitSize, font_cardSize, font_defaultSize, font_endmsgSize;

// buffer size is meaningless, current maximum: 18 + terminal
char str_endMessage[32] = "";

Font game_defaultFont, game_cardFont;

/*
 * this function handles the game logic and drawing the game window.
 *
 * params:
 *    is_logged_in    used to know if the current user is online or not.
 *                    this impacts the call of functions for bet handling.
 *
 * return:
 *    returns if the app should go back to the main menu or the app should close
 */
bool GameStart(bool is_logged_in) {

  bool hitButtonPressed, standButtonPressed, surrendButtonPressed,
      doubledownButtonPressed, betButtonPressed;
  bool flag_end;
  bool flag_close;
  deck gameDeck;
  card playerHand[5], dealerHand[5];
  int playerCardNum, dealerCardNum;
  int playerHandScore, dealerHandScore;

  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

  // default font is pixelated, which makes filtering less effective than
  // leaving as is. size of the imported font is just what seemed to look better
  // on my device because idk
  InitWindow(0, 0, "Blackjack");
  SetWindowSize(GetMonitorWidth(GetCurrentMonitor()) / 2,
                GetMonitorHeight(GetCurrentMonitor()) / 2);
  SetTargetFPS(30);

  game_defaultFont = GetFontDefault();
  GuiSetFont(
      game_defaultFont); // necessary due to issues with how raygui handles
                         // fonts unloading after closing a window
  game_cardFont = LoadFontEx("../assets/cardcharacters.ttf", 256, 0, 250);
  SetTextureFilter(game_cardFont.texture, TEXTURE_FILTER_BILINEAR);

// when pressing retry, the program jumps to this label to avoid doing tasks
// like loading fonts again and to not redeclare variables.
start:

  hitButtonPressed = standButtonPressed = surrendButtonPressed =
      doubledownButtonPressed = betButtonPressed = false;
  flag_end = false;   // draw the end screen?
  flag_close = false; // go back to menu?

  gameDeck = FillDeck(DECK_NUM);
  ShuffleDeck(&gameDeck);

  for (int i = 0; i < START_CARD_NUM; i++) {
    playerHand[i] = DrawCard(&gameDeck);
    dealerHand[i] = DrawCard(&gameDeck);
  }

  playerCardNum = dealerCardNum = START_CARD_NUM;
  playerHandScore = CalcScore(playerHand, playerCardNum);
  dealerHandScore = CalcScore(dealerHand, dealerCardNum);

  // Game Loop Start

  while (!flag_close) {

    if (WindowShouldClose()) {

      free(gameDeck.cards);
      UnloadFont(game_cardFont);

      // the idea of this is, when pressing escape it is seen as wanting to go
      // back to the starting menu (to login again or smth), while manually
      // closing the window means you just want to close the app
      // this would break if you managed to only keep the key down for 1/60th of
      // a second, but it shouldnt be possible
      if (IsKeyDown(KEY_ESCAPE)) {
        CloseWindow();
        return true;
      } else {
        CloseWindow();
        return false;
      }
    }

    if (playerCardNum < MAX_CARD_NUM && playerHandScore < 21) {
      if (hitButtonPressed || doubledownButtonPressed) {

        playerHand[playerCardNum++] = DrawCard(&gameDeck);
        playerHandScore = CalcScore(playerHand, playerCardNum);
      }
    }
    if (standButtonPressed || doubledownButtonPressed || surrendButtonPressed ||
        playerCardNum >= MAX_CARD_NUM || playerHandScore >= 21) {

      flag_end = true;

      while (dealerHandScore < AI_STAND_SCORE && dealerCardNum < MAX_CARD_NUM) {

        dealerHand[dealerCardNum++] = DrawCard(&gameDeck);
        dealerHandScore = CalcScore(dealerHand, dealerCardNum);
      }
    }

    game_UpdateSizes();

    BeginDrawing();

    ClearBackground(DARKGREEN);

    // draws the card in hand and then draws all empty spaces in green since
    // they are empty.

    for (int i = 0; i < playerCardNum; i++) {
      game_DrawCard(playerHand[i], arr_recPlayerCards[i]);
    }
    for (int i = playerCardNum; i < MAX_CARD_NUM; i++) {
      DrawRectangleRec(arr_recPlayerCards[i], GREEN);
    }

    for (int i = 0; i < MAX_CARD_NUM; i++) {

      if (i < playerCardNum) {
        game_DrawCard(playerHand[i], arr_recPlayerCards[i]);
      } else {
        DrawRectangleRec(arr_recPlayerCards[i], GREEN);
      }
    }

    if (!flag_end) {

      // only the first card is shown to the player while the others are face
      // down. Drawing generally works as above.
      game_DrawCard(dealerHand[0], arr_recDealerCards[0]);

      for (int i = 1; i < MAX_CARD_NUM; i++) {

        if (i < dealerCardNum) {
          DrawRectangleRec(arr_recDealerCards[i], DARKBLUE);
        } else {
          DrawRectangleRec(arr_recDealerCards[i], GREEN);
        }
      }
    } else {

      for (int i = 0; i < MAX_CARD_NUM; i++) {

        if (i < dealerCardNum) {
          game_DrawCard(dealerHand[i], arr_recDealerCards[i]);
        } else {
          DrawRectangleRec(arr_recDealerCards[i], GREEN);
        }
      }
    }

    GuiSetStyle(DEFAULT, TEXT_SIZE, font_defaultSize);

    DrawRectangleRec(button_bg, BROWN);
    hitButtonPressed = GuiButton(rec_hitButton, "HIT!");
    standButtonPressed = GuiButton(rec_standButton, "STAND");
    surrendButtonPressed = GuiButton(rec_surrendButton, "SURREND");
    doubledownButtonPressed = GuiButton(rec_doubledownButton, "DOUBLE!");
    betButtonPressed = GuiButton(rec_betButton, "BET!");

    game_UpdateCursor();

    if (flag_end) {

      endgame_state endResult;

      if (playerHandScore > 21) {
        endResult = BUST;
      } else if (playerHandScore == 21) {
        endResult = BLACKJACK;
      } else if (playerCardNum == MAX_CARD_NUM) {
        endResult = CHARLIE;
      } else if (playerHandScore > dealerHandScore || dealerHandScore > 21) {
        endResult = WIN;
      } else if (dealerHandScore > playerHandScore) {
        endResult = LOSE;
      } else {
        endResult = DRAW;
      }

      if (DrawEndgameScreen(endResult)) {
        free(gameDeck.cards);
        goto start;
      }
    }

    EndDrawing();
  }
  return false;
}

void game_DrawCard(card card, Rectangle target) {

  char suitStr[2] = "";
  Color textColor;

  if (strcmp(card.suit, "Clubs") == 0) {
    strcpy(suitStr, CLUBS);
    textColor = BLACK;
  } else if (strcmp(card.suit, "Diamonds") == 0) {
    strcpy(suitStr, DIAMONDS);
    textColor = RED;
  } else if (strcmp(card.suit, "Hearts") == 0) {
    strcpy(suitStr, HEARTS);
    textColor = RED;
  } else if (strcmp(card.suit, "Spades") == 0) {
    strcpy(suitStr, SPADES);
    textColor = BLACK;
  }

  DrawRectangleRec(target, RAYWHITE);

  Vector2 suit_textSize =
      MeasureTextEx(game_cardFont, suitStr, font_suitSize, 0);

  // handling of positioning of card text
  float suit_leftX = target.x + suit_xFromBorder - (suit_textSize.x / 2.f);
  float suit_rightX =
      target.x + target.width - suit_xFromBorder - (suit_textSize.x / 2.f);
  float suit_botY = target.y + suit_yFromBorder - (suit_textSize.y / 2.f);
  float suit_topY =
      target.y + target.height - suit_yFromBorder - (suit_textSize.y / 2.f);

  Vector2 suit_botLeft = (Vector2){suit_leftX, suit_botY};
  Vector2 suit_botRight = (Vector2){suit_rightX, suit_botY};
  Vector2 suit_topLeft = (Vector2){suit_leftX, suit_topY};
  Vector2 suit_topRight = (Vector2){suit_rightX, suit_topY};

  DrawTextEx(game_cardFont, suitStr, suit_botLeft, font_suitSize, 0, textColor);
  DrawTextEx(game_cardFont, suitStr, suit_botRight, font_suitSize, 0,
             textColor);
  DrawTextEx(game_cardFont, suitStr, suit_topLeft, font_suitSize, 0, textColor);
  DrawTextEx(game_cardFont, suitStr, suit_topRight, font_suitSize, 0,
             textColor);

  char rankStr[2] = "";
  char arr_rankStr[][2] = {"A", "2", "3",      "4", "5", "6", "7",
                           "8", "9", NARROW10, "J", "Q", "K"};

  // the value of a card can still be stored as a number, and as long as the
  // rankStr array is ordered the text will match
  strcpy(rankStr, arr_rankStr[card.rank - 1]);

  Vector2 rank_textSize =
      MeasureTextEx(game_cardFont, rankStr, font_cardSize, 2);

  float rank_x = target.x + (target.width / 2.f) - (rank_textSize.x / 2.f);
  float rank_y = target.y + (target.height / 2.f) - (rank_textSize.y / 2.f);

  Vector2 rank_pos = (Vector2){rank_x, rank_y};

  DrawTextEx(game_cardFont, rankStr, rank_pos, font_cardSize, 2, textColor);
}

void game_UpdateSizes() {

  game_screenWidth = (float)GetScreenWidth();
  game_screenHeight = (float)GetScreenHeight();

  for (int i = 0; i < MAX_CARD_NUM; i++) {
    arr_recPlayerCards[i].x =
        (CARD_X_START + (i * CARD_X_STEP)) * game_screenWidth;
    arr_recDealerCards[i].x = arr_recPlayerCards[i].x;

    arr_recPlayerCards[i].width = CARD_WIDTH * game_screenWidth;
    arr_recDealerCards[i].width = arr_recPlayerCards[i].width;

    arr_recPlayerCards[i].y = PLAYER_CARD_Y * game_screenHeight;
    arr_recDealerCards[i].y = DEALER_CARD_Y * game_screenHeight;

    arr_recPlayerCards[i].height = CARD_HEIGHT * game_screenHeight;
    arr_recDealerCards[i].height = arr_recPlayerCards[i].height;

    // After finding the x/y ratio so that x = y * ratio, we can create a
    // rectangle similar to the card rectangle.
    // This rectangle is positioned with one of its vertices in the upper left
    // corner of the card, and another vertex in the position where we want to
    // place the seed symbol.
    // The size of the diagonal of one of these rectangles will be found with
    // the formula sqrtf(y^2 * / (1 + ratio^2)).
    // We can find the size of the diagonal of the smaller rectangle by applying
    // a percentage on the size of the diagonal of the card rectangle.
    // After that, we can substitute the ratio and the size into the previous
    // formula and solve it for the y variable, which we can then use to find
    // the x.
    // This gives us the coordinates of the bottom right angle, which we can use
    // to find the offset to apply to the seed symbol.
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

    // magic numbers can be seen as a percentage of the screen

    const float button_x = game_screenWidth * BUTTON_RELX,
                button_width = game_screenWidth * BUTTON_RELW,
                button_height = game_screenHeight * BUTTON_RELH;

    button_bg.x = game_screenWidth * BUTTON_BG_RELX;
    button_bg.y = BUTTON_BG_RELY;
    button_bg.width = game_screenWidth * BUTTON_BG_RELW;
    button_bg.height = BUTTON_BG_RELH;

    rec_hitButton.x = rec_standButton.x = rec_surrendButton.x =
        rec_doubledownButton.x = rec_betButton.x = button_x;

    rec_hitButton.width = rec_standButton.width = rec_surrendButton.width =
        rec_doubledownButton.width = rec_betButton.width = button_width;

    rec_hitButton.height = rec_standButton.height = rec_surrendButton.height =
        rec_doubledownButton.height = rec_betButton.height = button_height;

    rec_hitButton.y = game_screenHeight * HIT_BUTTON_RELY;
    rec_standButton.y = game_screenHeight * STAND_BUTTON_RELY;
    rec_surrendButton.y = game_screenHeight * SURREND_BUTTON_RELY;
    rec_doubledownButton.y = game_screenHeight * DOUBLEDOWN_BUTTON_RELY;
    rec_betButton.y = game_screenHeight * BET_BUTTON_RELY;

    rec_endMessage_bg.x = game_screenWidth * END_MESSAGE_RELX;
    rec_endMessage_bg.y = game_screenHeight * END_MESSAGE_RELY;
    rec_endMessage_bg.width = game_screenWidth * END_MESSAGE_RELW;
    rec_endMessage_bg.height = game_screenHeight * END_MESSAGE_RELH;

    rec_retryButton.x = game_screenWidth * END_RETRY_RELX;
    rec_retryButton.y = game_screenHeight * END_RETRY_RELY;
    rec_retryButton.width = game_screenWidth * END_RETRY_RELW;
    rec_retryButton.height = game_screenHeight * END_RETRY_RELH;

    vec_endgameText.x = game_screenWidth * END_TEXT_RELX;
    vec_endgameText.y = game_screenHeight * END_TEXT_RELY;
  }
}

bool DrawEndgameScreen(endgame_state flag_endgame_state) {
  if (flag_endgame_state != NO) {

    DrawRectangleRec(rec_endMessage_bg, GOLD);

    switch (flag_endgame_state) {
    case WIN:
      strcpy(str_endMessage, "You Win!");
      break;
    case LOSE:
      strcpy(str_endMessage, "You Lose!");
      break;
    case BUST:
      strcpy(str_endMessage, "Bust! You Lose!");
      break;
    case CHARLIE:
      strcpy(str_endMessage, "Charlie! You Win!");
      break;
    case BLACKJACK:
      strcpy(str_endMessage, "Blackjack!");
      break;
    case DRAW:
      strcpy(str_endMessage, "It's a Draw!");
      break;
    }

    font_endmsgSize = game_screenHeight / FONT_ENDMSG_MULT;
    Vector2 endTextSize =
        MeasureTextEx(game_defaultFont, str_endMessage, font_endmsgSize, 2);
    float end_x = vec_endgameText.x - (endTextSize.x / 2.f);
    float end_y = vec_endgameText.y - (endTextSize.y / 2.f);
    Vector2 vec_endmsgPos = (Vector2){end_x, end_y};

    DrawTextEx(game_defaultFont, str_endMessage, vec_endmsgPos, font_endmsgSize,
               2, BROWN);

    bool retryButtonPressed = GuiButton(rec_retryButton, "RETRY");

    game_UpdateCursor();

    if (retryButtonPressed) {
      return true;
    } else {
      return false;
    }
  }
  return false;
}

void game_UpdateCursor() {
  Vector2 mousePos = GetMousePosition();
  bool isCollidingWithButtons =
      CheckCollisionPointRec(mousePos, rec_hitButton) ||
      CheckCollisionPointRec(mousePos, rec_standButton) ||
      CheckCollisionPointRec(mousePos, rec_surrendButton) ||
      CheckCollisionPointRec(mousePos, rec_doubledownButton) ||
      CheckCollisionPointRec(mousePos, rec_betButton) ||
      CheckCollisionPointRec(mousePos, rec_retryButton);

  if (isCollidingWithButtons) {
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  } else {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }
}

// TODO:
// [X] Create a function to correctly handle the cursor aspect change without
// previous flickering in the end screen
// [ ] Add bets support
// [ ] Add a proper implementation of double down
// [ ] Update the cursor update function again to prevent issues on overlaps
