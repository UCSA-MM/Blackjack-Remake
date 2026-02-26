#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "raylib.h"
#include "raygui.h"
//#include "cards.h"
//#include "blackjackclient.h"
#include "gamewindow.h"

//temporary declarations to avoid errors
typedef struct deck {} deck;
typedef struct card {} card;
deck FillDeck(int n_decks){return 0;}
void ShuffleDeck(deck* p_deck){}
card DrawCard(deck* p_deck){return 0;}
int CalcScore(card cards[], int n_cards){return 0;}

void GameStart(bool is_logged_in) {
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
    if(!IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE)) {
        SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
    }
}
