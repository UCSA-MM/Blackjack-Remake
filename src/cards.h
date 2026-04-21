#ifndef CARDS_H
#define CARDS_H

typedef struct card {
  int rank;
  char* suit;
} card;

typedef struct deck {
  card* cards;
  int cardNum;
} deck;

deck FillDeck(int n_decks);
void ShuffleDeck(deck* p_deck);
card DrawCard(deck* p_deck);
int CalcScore(card hand[], int n_cards);

#endif
