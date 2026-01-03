#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>
//--------------------------------------------------------------------------------------------------------------
//records
typedef struct element{
    struct Player info;
    struct element *next; 
}element;

typedef struct Player {
    char name[50];
    int playerID;
    int age;
    int currentScore;
    int totalScore;
    int roundWon;
    int roundLost;    
    int consecutiveWins;      
    int consecutiveLosses;
} Player;

typedef struct Queue {
    element *head;
    element *tail;
}Queue;



//-----------------------------------------------------------------------------------------------------------------------------------
//functions :
//queue operations :
void Initqueue(Queue *F) {
    F->head = NULL;
    F->tail = NULL;
}

void Enqueue(Queue *F, Player p) {
    element *newElement = (element*)malloc(sizeof(element));

    newElement->info = p;
    newElement->next = NULL;
    
    if (F->tail == NULL) {
        F->head = newElement;
        F->tail = newElement;
    } else {
        F->tail->next = newElement;
        F->tail = newElement;
    }
}

void Dequeue (Queue *F  , Player *p) {
    *p = F->head->info;
    element *temp = F->head ;
    F->head = F->head->next;
    free(temp);
    if (F->head == NULL) {
            F->tail = NULL;
    }

}

bool EmptyQueue(Queue F) {
    return (F.head == NULL);
}
//this function generate a random number

int randomNumber (){
    return rand();
}

//this function calculate the sum of the digit of a number

int sumOfDigits(int value){
    int sum = 0;
    while(value!=0){
        sum += value % 10;
        value /= 10;
    }
    return sum;
}

void gameRound (Player *P1,Player *P2){
    int generatedValue = 0;
    printf("\n═══════════════════════════════════════\n");
    printf("           ROUND: %s vs %s           \n", P1->name, P2->name);
    printf("═══════════════════════════════════════\n");
    P1->currentScore = 0;
    P2->currentScore = 0;
    while (abs(P1->currentScore - P2->currentScore) < 3 && generatedValue < 12 ){
        //player 1 turn : 

        printf("\n─── %s's Turn ───\n", P1->name);

        printf("press 'ENTER' to start . . .\n");
        getchar();

        int random1 = randomNumber();
        printf("The system generated this value : %d\n (press enter to continue)", random1);
        getchar();

        int sum1 = sumOfDigits(random1);
        printf("The sum of the digits of %d is : %d\n(press enter to continue)",random1 , sum1);
        getchar();

        if(sum1 % 5 == 0){
            printf("%s get a point !(+1)\n(press enter to continue)",P1->name);
            getchar();
            P1->currentScore ++;
        }else{
            printf("%s DID NOT get a point !(+0)\n(press enter to continue)", P1->name);
            getchar();
        }

        printf("%s's score: %d\n", P1->name, P1->currentScore);
        if (abs(P1->currentScore - P2->currentScore) >= 3) {
            break;
        }

        //player 2 turn : 
        printf("\n─── %s's Turn ───\n", P2->name);        
        printf("press 'ENTER' to start . . .\n");
        getchar();

        int random2 = randomNumber();
        printf("The system generated this value : %d\n (press enter to continue)", random2);
        getchar();

        int sum2 = sumOfDigits(random2);
        printf("The sum of the digits of %d is : %d\n(press enter to continue)",random2 , sum2);
        getchar();

        if(sum2 % 5 == 0){
            printf("%s get a point !(+1)\n(press enter to continue)",P2->name);
            getchar();
            P2->currentScore ++;
        }else{
            printf("%s DID NOT get a point !(+0)\n(press enter to continue)", P2->name);
            getchar();
        }

        generatedValue +=2;
    }
    //______________________________________________________________________________________________________
    //update players statistics :
    printf("\n═══════════════════════════════════════\n");
    printf("            ROUND FINISHED             \n");
    printf("═══════════════════════════════════════\n");

    if (P1->currentScore > P2->currentScore) {
        // Player 1 wins
        P1->roundWon++;
        P1->totalScore += P1->currentScore;
        P1->consecutiveWins++;
        P1->consecutiveLosses = 0;
        
        P2->roundLost++;
        P2->totalScore += P2->currentScore;
        P2->consecutiveLosses++;
        P2->consecutiveWins = 0;
        
        printf("🏆 WINNER: %s (ID: %d)\n", P1->name, P1->playerID);
        printf("Score: %d - %d\n", P1->currentScore, P2->currentScore);
        
    } else if (P2->currentScore > P1->currentScore) {
        // Player 2 wins
        P2->roundWon++;
        P2->totalScore += P2->currentScore;
        P2->consecutiveWins++;
        P2->consecutiveLosses = 0;
        
        P1->roundLost++;
        P1->totalScore += P1->currentScore;
        P1->consecutiveLosses++;
        P1->consecutiveWins = 0;
        
        printf("🏆 WINNER: %s (ID: %d)\n", P2->name, P2->playerID);
        printf("Score: %d - %d\n", P1->currentScore, P2->currentScore);
        
    } else {
        // Draw
        printf("🤝 DRAW!\n");
        printf("Score: %d - %d\n", P1->currentScore, P2->currentScore);
    }

}



//The main function : 
int main (){
    srand(time(NULL));


    return 0;
}