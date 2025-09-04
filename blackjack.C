//  CS240 blackjack game,  03/12/2025
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


typedef struct {
    char cards[21];  
    int numAces;
    int value;
    int numCards;
} Hand;


char dealCard(int num);
int getVal(char c);
void printDealerCards(Hand h);
void printCards(Hand h);
void hit(Hand *h);
int getNumAces(Hand *h);
bool checkBlackJack(Hand *h);

int main() {
    srand(time(NULL));

    int money = 1000;
    printf("Welcome to the table, the min bet is $20. You have $1000\n");
    printf("The rules are simple, enter 1 to hit, 2 to stand\n");

    int bet;
    Hand dealerHand;
    Hand playerHand;  

getBet:
    printf("You have $%d\n", money);
    printf("Enter your bet: ");
    if (scanf("%d", &bet) != 1 || bet > money || bet < 20) {
        printf("Error: Invalid input, please enter an integer within your betting range.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ; 
        goto getBet;
    }
    money -= bet;
    {
        int randomNum1 = rand() % 13 + 1;  
        int randomNum2 = rand() % 13 + 1;
        dealerHand.cards[0] = dealCard(randomNum1);
        dealerHand.cards[1] = dealCard(randomNum2);
        dealerHand.numCards = 2;
        dealerHand.value = getVal(dealerHand.cards[0]) + getVal(dealerHand.cards[1]);
        dealerHand.numAces = getNumAces(&dealerHand);

        if (checkBlackJack(&dealerHand)) {
            printCards(dealerHand);
        } else {
            printDealerCards(dealerHand);
        }
    }

    {
        int randomNum3 = rand() % 13 + 1;
        int randomNum4 = rand() % 13 + 1;
        playerHand.cards[0] = dealCard(randomNum3);
        playerHand.cards[1] = dealCard(randomNum4);
        playerHand.numCards = 2;
        playerHand.value = getVal(playerHand.cards[0]) + getVal(playerHand.cards[1]);
        playerHand.numAces = getNumAces(&playerHand);
        printCards(playerHand);
        if (checkBlackJack(&playerHand)) {
            printf("Blackjack!\n");
            money += bet * 2.5;
            printf("You have $%d\n", money);
            goto getPlayAgain;
        }
    }

turn:
    {
        int move;
getTurn:
        printf("Enter your move (1 to hit, 2 to stand): ");
        if (scanf("%d", &move) != 1 || (move != 1 && move != 2)) {
            printf("Error: Invalid input, please enter 1 to hit or 2 to stand.\n");
            int C;
            while ((C = getchar()) != '\n' && C != EOF)
                ; 
            goto getTurn;
        }

        if (move == 2) {
            goto dealerTurn;
        } else {  // move == 1, hit
            hit(&playerHand);
            printCards(playerHand);
            if (playerHand.value == 21) {
                goto dealerTurn;
            } else if (playerHand.value > 21 && playerHand.numAces > 0) {
                playerHand.value -= 10;
                playerHand.numAces--;
                goto getTurn;
            }else if(playerHand.value > 21){
                printf("Bust!\n");
                goto getPlayAgain;
            } else {
                goto getTurn;
            }
        }
    }

dealerTurn:
    {
        printCards(dealerHand);
        if (dealerHand.value > 21 && dealerHand.numAces > 0) {
            dealerHand.value -= 10;
            dealerHand.numAces--;
            goto dealerTurn;
        }else if(dealerHand.value > 21){
            printf("Dealer busts, You win!");
            money += 2 * bet;
            goto getPlayAgain;
        }else if (dealerHand.value > 16) {
            goto determineWinner;
        } else {
            hit(&dealerHand);
            goto dealerTurn;
        }
    }

determineWinner:
    {
        if (dealerHand.value == playerHand.value) {
            printf("You pushed.\n");
            money += bet;
            printf("You have $%d\n", money);
            goto getPlayAgain;
        } else if (dealerHand.value > playerHand.value) {
            printf("You lost.\n");
            printf("You have $%d\n", money);
            goto getPlayAgain;
        } else {
            printf("You win!\n");
            money += bet * 2;
            printf("You have $%d\n", money);
            goto getPlayAgain;
        }
    }

getPlayAgain:
    {
        if(money == 0){
            printf("Game over");
            return 0;
        }
        printf("Enter 1 to play again or 2 to quit: ");
        int pg;
        if (scanf("%d", &pg) != 1 || (pg != 1 && pg != 2)) {
            printf("Error: Invalid input, enter 1 to play again or 2 to quit.\n");
            int D;
            while ((D = getchar()) != '\n' && D != EOF)
                ; // clear input buffer
            goto getPlayAgain;
        }
        if (pg == 1 ) {
            goto getBet;
        } else {
            return 0;  
        }
    }

    return 0;
}

bool checkBlackJack(Hand *h) {
    return (h->value == 21);
}


int getNumAces(Hand *h) {
    int count = 0;
    for (int i = 0; i < h->numCards; i++) {
        if (h->cards[i] == 'A') {
            count++;
        }
    }
    return count;
}


void hit(Hand *h) {
    int numCard = h->numCards;  
    int randomNum = rand() % 13 + 1;
    h->cards[numCard] = dealCard(randomNum);
    h->numCards += 1;
    h->value += getVal(h->cards[numCard]);
}


void printCards(Hand h) {
    for (int i = 0; i < h.numCards; i++) {
        printf("%c  ", h.cards[i]);
    }
    printf("\n");
}

void printDealerCards(Hand h) {
    printf("%c\n", h.cards[0]);
}

char dealCard(int num) {
    if (num == 1) {
        return 'A';
    } else if (num == 11) {
        return 'J';
    } else if (num == 12) {
        return 'Q';
    } else if (num == 13) {
        return 'K';
    } else if (num == 10) {
        return 'T';  // 'T' representing 10
    } else {
       
        return '0' + num;
    }
}


int getVal(char c) {
    if (c == 'A') {
        return 11;
    } else if (c == 'K' || c == 'Q' || c == 'J' || c == 'T') {
        return 10;
    } else {
        return c - '0';
    }
}
