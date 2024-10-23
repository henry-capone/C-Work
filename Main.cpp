/// SortCard.cpp : This file contains the 'main' function. Program execution begins and ends there.
// RJM 6/2/24

#include "cardlib.h"

#include <string>
#include <iostream>
#include <array>
using namespace std;

const int maxCard = 20;

aCard thePack[maxCard];
aCard thePackCopy[maxCard];


int main()
{
    std::cout << "Card Sorting!\n";

    for (int ct = 0; ct < maxCard; ct++) {
        thePack[ct] = getCard("32018389");   
        thePackCopy[ct] = thePack[ct];
    }

    printPack("The pack unsorted:",thePack);
    bubbleSort(thePack);
    quickSort(thePackCopy,0, maxCard - 1);
}