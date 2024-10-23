// RJM's file for getting a random card
// 6/2/24

#include <iostream>
#include "cardlib.h"
using namespace std;

int maxCard = 20;
int compCount = 0;
int swapCount = 0;
int depth = 0;

string cardToStr(aCard c) {

    string res, value, suit;        // create string variables for result, value and suit
    if (c.cardVal > 10 || c.cardVal < 2) { // if the value of card passed in is over 10 or under 2. i.e King/Queen/Jack/Ace
        if (c.cardVal == 11) { value = "J"; } // if that value is 11 set the value to represent Jack
        else if (c.cardVal == 12) { value = "Q"; } // if that value is 12 set the value to represent Queen
        else if (c.cardVal == 13) { value = "K"; } // if that value is 13 set the value to represent King
        else { value = "A"; }       // otherwise that value is 1 so set the value to reprsent Ace
    }
    else { value = to_string(c.cardVal); } // otherwise that value is the same in a card deck, so convert to a string

    if (c.cardSuit == 0) {          // if card suit is 0 set suit to represent hearts
        suit = "H";
    }
    else if (c.cardSuit == 1) { suit = "C"; } // if card suit is 1 set suit to represent clubs
    else if (c.cardSuit == 2) { suit = "D"; } // if card suit is 2 set suit to represent diamonds
    else { suit = "S"; }            // if card suit is 3 set suit to represent spades

    res = value + "_" + suit;       // append the value and suit together

    return res;                     // return that appened string
}

void printPack(string msg, aCard Pack[]) {
    std::cout << msg << " " << endl; // display the message user provided
    for (int ct = 0; ct < maxCard; ct++) { // loops through the pack
        std::cout << cardToStr(Pack[ct]); // call the cardToStr function on each card in the pack
        if (ct != maxCard - 1) {    // if the current card is not the last
            std::cout << ", ";      // display a comma between cards printed.
        }
    }
    std::cout << endl;              // once complete start a new line
}

aCard getCard(string stdno) {  // function returns a card  - using student number stdno (8 numerical chars)
    aCard ans;
    string rcardstr = stdno.substr(rand() % 8, 1) + stdno.substr(rand() % 8, 1);    // two random characters from stdno
    int rcard = stoi(rcardstr) % 52;          // get integer in range 0..51
    string res = "  ";
    ans.cardVal = 1 + rcard % 13;                          // give cardVal 1..13 
    ans.cardSuit = static_cast<Suit>(rcard / 13);         // and for suit
    return ans;
}

int compareCards(aCard c1, aCard c2) {      // method compares by suit as a first index and value as a second: H<C<D<S & A<1<2..<J<Q<K
                                            // return 1 if c1>c2 / 0 if c1=c2 / -1 if c1<c2
    compCount++;                            // increase comparison count - will be used when counting comparisons in sort methods

    if (c1.cardSuit != c2.cardSuit) {       // checking that the two cards do not have the same suit
        if (c1.cardSuit > c2.cardSuit) { return 1; } // check whether c1 is of a higher suit than c2 if so we can say c1 is greater
        else { return -1; }                 // otherwise, we know c2 is greater
    }
    else {                                  // otherwise, if cards are of the same suit
        if (c1.cardVal > c2.cardVal) { return 1; } // if c1 has a greater value, we know it's the higher card
        else if (c1.cardVal == c2.cardVal) { return 0; } // if they are equal we can return 0
        else { return -1; }                 // otherwise, we know c2 is greater so return -1
    }
}

void swapCards(aCard Pack[], int c1, int c2) {
    swapCount++;                            // increase swap count - will be used when counting swaps in sort methods
    aCard temp = Pack[c2];                  // create a temporary variable of c2
    Pack[c2] = Pack[c1];                    // set c2 in the pack to c1
    Pack[c1] = temp;                        // set c1 to be the temporary variable (e.g c2)
}

void bubbleSort(aCard Pack[]) {
    int i, j;                               // create integer variables i and j
    for (i = 0;i < maxCard - 1;i++) {       // loop through the pack of cards
        for (int j = 0; j < maxCard - i - 1;j++) { // loop through the pack of cards again
            if (compareCards(Pack[j], Pack[j + 1]) == 1) { // compare the cards at position j and j+1
                swapCards(Pack, j, j + 1);  // if card at j is greater then swap cards at j and j+1
            }
        }
    }
    std::cout << "Bubble sorted in " << compCount << " comparisons & " << swapCount << " swaps" << endl;; // print out comparisons and swap counts
    printPack("Sorted Pack:", Pack);
    compCount = 0;                          // reset comparisons count
    swapCount = 0;                          // reset swap count
}

void quickSort(aCard Pack[],int lIndex, int hIndex) {
    depth++;                                // increase recursion depth counter
    if (lIndex < hIndex) {                  // check inputted high index is greater than inputted low index
        aCard pivot = Pack[(lIndex + hIndex) / 2]; // create a pivot card in the middle of the pack
        int i = lIndex, j = hIndex;         // create integer variables i and j

        do {
            while (compareCards(pivot, Pack[i]) == 1) { // while the cards left of the pivot are less than than the pivot
                i++;                        // increase i
            }
            while (compareCards(pivot, Pack[j]) == -1) { // otherwise, while the cards right of the pivot are greter than the pivot
                j--; // decrease j
            }
            if (i <= j) { // if i is less than or equal to j
                swapCards(Pack, i, j); // swap cards at positions i and j
                i++; // increase i
                j--; // decrease j
            }
        } while (i <= j); // stop do-while loop when i is greater or equal to j

        quickSort(Pack, lIndex, j); // recursively call quicksort function with low Index the same and high Index as j
        quickSort(Pack, i, hIndex); // recursively call quicksort function with low Index as i and high Index as the same

        if (lIndex == 0 && hIndex == maxCard - 1) { // check whether lIndex is 0 or hIndex is maxCard-1
            std::cout << "Quick sorted in " << compCount << " comparisons & " << swapCount << " swaps & " << depth << " recursion depth" << std::endl;
            // display the amount of comparisons, swaps and recursion depth
            printPack("Sorted pack: ", Pack);
            compCount = 0;          // reset comparisons counter
            swapCount = 0;          // reset swap counter
            depth = 0;              // reset recursion depth counter
        }
    }
}