#include "cards.h"
#include <stdlib.h>
#include <time.h>

void CardSwap(card *p_first, card *p_second);

deck FillDeck(int n_decks) {

  int rank;
  int suitNum;
  deck createdDeck = {0};
  card toAdd = {0};
  createdDeck.cards = calloc(52 * n_decks, sizeof(card));

  if (createdDeck.cards == NULL) {
    exit(14);
  }

  for (int i = 0; i < n_decks; i++) {
    for (int j = 0; j < 52; j++) {
      suitNum = j / 13;
      rank = j + 1;

      while (rank > 13) {
        rank -= 13;
      }

      toAdd.rank = rank;

      switch (suitNum) {
      case 0:
        toAdd.suit = "Clubs";
        break;
      case 1:
        toAdd.suit = "Diamonds";
        break;
      case 2:
        toAdd.suit = "Hearts";
        break;
      case 3:
        toAdd.suit = "Spades";
        break;
      }

      createdDeck.cards[j + (i * 52)] = toAdd;
    }
  }

  createdDeck.cardNum = n_decks * 52;
  return createdDeck;
}

// uses Fisher-Yates shuffle
void ShuffleDeck(deck *p_deck) {

  srand(time(NULL));
  int cardNum = p_deck->cardNum;

  for (int i = (cardNum - 1); i > 0; i--) {
    int j = rand() % (i + 1);
    CardSwap(&p_deck->cards[i], &p_deck->cards[j]);
  }
}

card DrawCard(deck *p_deck) {

  card ret = {0};
  ret.rank = p_deck->cards[0].rank;
  ret.suit = p_deck->cards[0].suit;
  int cardNum = p_deck->cardNum - 1;

  for (int i = 0; i < cardNum; i++) {
    p_deck->cards[i].rank = p_deck->cards[i + 1].rank;
    p_deck->cards[i].suit = p_deck->cards[i + 1].suit;
  }

  return ret;
}

int CalcScore(card hand[], int n_cards) {

  int score = 0;

  for (int i = 0; i < n_cards; i++) {
    if (hand[i].rank == 1) {
      score += 11;
    } else if (hand[i].rank > 10) {
      score += 10;
    } else {
      score += hand[i].rank;
    }
  }

  for (int i = 0; score > 21 && i < n_cards; i++) {
    if (hand[i].rank == 1) {
      score -= 10;
    }
  }

  return score;
}

void CardSwap(card *p_first, card *p_second) {
  card temp = *p_first;
  *p_first = *p_second;
  *p_second = temp;
}
