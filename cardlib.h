#pragma once

// header file for cardlib
// RJM 06/01/24


#include <string>

enum Suit { hearts, clubs, diamonds, spades };      // define suits

struct aCard {                          // defines a card
    int cardVal;                        // number 1..13
    Suit cardSuit;                      // suit
};

std::string cardToStr(aCard c);			// declare function to represent a card as a two character string

aCard getCard(std::string stdno);        // declares function to get a card from stdno string

void printPack(std::string msg, aCard Pack[]);

int compareCards(aCard c1, aCard c2);

void swapCards(aCard Pack[], int c1, int c2);

void bubbleSort(aCard Pack[]);

void quickSort(aCard Pack[], int hIndex, int lIndex);