#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>

//--------------------------------------------------------------------------------------------------------------
// CONSTANTS
#define SCORE_DIFFERENCE_THRESHOLD 3
#define WIN_THRESHOLD_PART1 5
#define LOSS_THRESHOLD_PART1 5
#define CONSECUTIVE_WIN_THRESHOLD_PART1 3
#define CONSECUTIVE_LOSS_THRESHOLD_PART1 3
#define MAX_VALUES_PART1 12
#define MAX_VALUES_PART2 16
#define WIN_THRESHOLD_PART2 2
#define LOSS_THRESHOLD_PART2 2
#define CONSECUTIVE_WIN_THRESHOLD_PART2 2

//--------------------------------------------------------------------------------------------------------------
// RECORDS

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
    // Additional tracking for Part I and II
    int winsPartI;
    int lossesPartI;
    int winsPartII;
    int lossesPartII;
} Player;

typedef struct element{
    struct Player info;
    struct element *next; 
} element;

typedef struct Queue {
    element *head;
    element *tail;
} Queue;

typedef element *Stack;
typedef element *Elist;

// Round history structure
typedef struct Round {
    int roundNumber;
    char player1Name[50];
    char player2Name[50];
    int player1ID;
    int player2ID;
    int player1Score;
    int player2Score;
    char winner[50];
    time_t startTime;
    time_t endTime;
    int strategy;
    struct Round *next;
} Round;

typedef struct Round *RoundHistory;

//-----------------------------------------------------------------------------------------------------------------------------------
// FUNCTION PROTOTYPES

// Queue operations
void Initqueue(Queue *F);
void Enqueue(Queue *F, Player p);
void Dequeue(Queue *F, Player *p);
bool EmptyQueue(Queue F);

// Stack operations
void InitStack(Stack *S);
void PUSH(Stack *S, Player P);
void POP(Stack *S, Player *P);
bool EmptyStack(Stack S);
Player TOPStack(Stack S);

// List operations
void insertSortedLG(Elist *LG, Player p);

// Game logic functions
int randomNumber();
void AutoFill(Queue *F, int n);
void updatePlayersStatistics(Player *P1, Player *P2, int strategy);
bool GetPlayer(Queue *F1, Queue *F, Queue *F3, Player *p, Stack *LP);
bool isGameOver(Queue F, Queue F1, Queue F3);

// Part I functions
int sumOfDigits(int value);
void playRoundPartOne(Player *P1, Player *P2);
void classifyPartOne(Player *P, Queue *F, Queue *F1, Queue *F3, Stack *LP, Elist *LG);

// Part II functions
int gcd(int a, int b);
bool digitAppears(int digit, int number);
bool gcdDigitCondition(int a, int b);
void playRoundPartTwo(Player *P1, Player *P2, bool *reachedThreshold);
void classifyPartTwo(Player *P, Queue *F, Queue *F1, Queue *F3, Stack *LP, Elist *LG, bool wonByThreshold);
bool forcedGameOver(int strategy2Rounds, int n);

// Display functions
void displayQueue(Queue F, char* name);
void displayList(Elist LG);
void displayStack(Stack LP);
void displayAllQueuesAndLists(Queue F, Queue F1, Queue F3, Elist LG, Stack LP);
void displayTop3Winners(Elist LG);
void displayRoundHistory(RoundHistory history);

// Query functions
void showPlayersWithNoWins(Elist LG, Stack LP);
void showPlayersByWinsPartI(Elist LG, Stack LP, int wins);
void showPlayersByLossesPartI(Elist LG, Stack LP, int losses);
void showPlayersByWinsPartII(Elist LG, Stack LP, int wins);
void showPlayersByLossesPartII(Elist LG, Stack LP, int losses);

// Round history functions
void addRound(RoundHistory *history, int roundNum, Player p1, Player p2, char* winner, time_t start, time_t end, int strategy);
void freeRoundHistory(RoundHistory *history);

// Memory cleanup functions
void freeQueue(Queue *F);
void freeList(Elist *LG);
void freeStack(Stack *LP);

//-----------------------------------------------------------------------------------------------------------------------------------
// QUEUE OPERATIONS

void Initqueue(Queue *F) {
    F->head = NULL;
    F->tail = NULL;
}

void Enqueue(Queue *F, Player p) {
    element *N = (element*)malloc(sizeof(element));
    if (N == NULL) {
        fprintf(stderr, "Memory allocation error in Enqueue\n");
        return;
    }

    N->info = p;
    N->next = NULL;
    
    if (F->tail == NULL) {
        F->head = N;
        F->tail = N;
    } else {
        F->tail->next = N;
        F->tail = N;
    }
}

void Dequeue(Queue *F, Player *p) {
    if (F->head == NULL) {
        fprintf(stderr, "Error: Attempt to dequeue from empty queue\n");
        return;
    }
    
    *p = F->head->info;
    element *temp = F->head;
    F->head = F->head->next;
    free(temp);
    
    if (F->head == NULL) {
        F->tail = NULL;
    }
}

bool EmptyQueue(Queue F) {
    return (F.head == NULL);
}

//-----------------------------------------------------------------------------------------------------------------------------------
// STACK OPERATIONS

void InitStack(Stack *S){
    *S = NULL;
}

void PUSH(Stack *S, Player P){
    element *N = (element*)malloc(sizeof(element));
    if (N == NULL) {
        fprintf(stderr, "Memory allocation error in PUSH\n");
        return;
    }
    
    N->info = P;
    N->next = *S;
    *S = N;
}

void POP(Stack *S, Player *P){
    if (*S == NULL) {
        fprintf(stderr, "Error: Attempt to pop from empty stack\n");
        return;
    }
    
    *P = (*S)->info;
    element *temp = *S;
    *S = (*S)->next;
    free(temp); 
}

bool EmptyStack(Stack S){
    return (S == NULL);
}

Player TOPStack(Stack S){
    return S->info;
}

//-----------------------------------------------------------------------------------------------------------------------------------
// LIST OPERATIONS

void insertSortedLG(Elist *LG, Player p) {
    element *N = (element*)malloc(sizeof(element));
    if (N == NULL) {
        fprintf(stderr, "Memory allocation error in insertSortedLG\n");
        return;
    }
    
    N->info = p;
    N->next = NULL;

    if (*LG == NULL || p.totalScore > (*LG)->info.totalScore) {
        N->next = *LG;
        *LG = N;
    } else {
        element *current = *LG;
        while (current->next != NULL && current->next->info.totalScore >= p.totalScore) {
            current = current->next;
        }
        N->next = current->next;
        current->next = N;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
// COMMON FUNCTIONS

int randomNumber() {
    return rand() % 100000; // Bounded random number
}

void AutoFill(Queue *F, int n) {
    for (int i = 0; i < n; i++) {
        Player p;
        
        sprintf(p.name, "Player_%02d", i+1);
        p.playerID = 1000 + i;
        p.age = 18 + (rand() % 10);
        p.totalScore = 0;
        p.roundWon = 0;
        p.roundLost = 0;
        p.consecutiveWins = 0;
        p.consecutiveLosses = 0;
        p.currentScore = 0;
        p.winsPartI = 0;
        p.lossesPartI = 0;
        p.winsPartII = 0;
        p.lossesPartII = 0;

        Enqueue(F, p);
    }
}

void updatePlayersStatistics(Player *P1, Player *P2, int strategy) {
    if (P1->currentScore > P2->currentScore) {
        // Player 1 wins
        P1->roundWon++;
        P1->consecutiveWins++;
        P1->consecutiveLosses = 0;
        P1->totalScore += P1->currentScore;
        if (strategy == 1) P1->winsPartI++;
        else P1->winsPartII++;

        P2->roundLost++;
        P2->consecutiveLosses++;
        P2->consecutiveWins = 0;
        P2->totalScore += P2->currentScore;
        if (strategy == 1) P2->lossesPartI++;
        else P2->lossesPartII++;
        
        printf("?? WINNER: %s (ID: %d)\n", P1->name, P1->playerID);
        printf("Score: %d - %d\n", P1->currentScore, P2->currentScore);
        
    } else if (P2->currentScore > P1->currentScore) {
        // Player 2 wins
        P2->roundWon++;
        P2->consecutiveWins++;
        P2->consecutiveLosses = 0;
        P2->totalScore += P2->currentScore;
        if (strategy == 1) P2->winsPartI++;
        else P2->winsPartII++;

        P1->roundLost++;
        P1->consecutiveLosses++;
        P1->consecutiveWins = 0;
        P1->totalScore += P1->currentScore;
        if (strategy == 1) P1->lossesPartI++;
        else P1->lossesPartII++;
        
        printf("?? WINNER: %s (ID: %d)\n", P2->name, P2->playerID);
        printf("Score: %d - %d\n", P2->currentScore, P1->currentScore);
        
    } else {
        // Draw
        P1->consecutiveWins = 0;
        P1->consecutiveLosses = 0;
        P2->consecutiveWins = 0;
        P2->consecutiveLosses = 0;

        P1->totalScore += P1->currentScore;
        P2->totalScore += P2->currentScore;
        
        printf("?? DRAW!\n");
        printf("Score: %d - %d\n", P1->currentScore, P2->currentScore);
    }
}

bool GetPlayer(Queue *F1, Queue *F, Queue *F3, Player *p, Stack *LP) {
    if (!EmptyQueue(*F1)) {
        Dequeue(F1, p);
        return true;
    }
    if (!EmptyQueue(*F)) {
        Dequeue(F, p);
        return true;
    }
    if (!EmptyQueue(*F3)) {
        if (F3->head == F3->tail) {
            Player last;
            Dequeue(F3, &last);
            PUSH(LP, last);
            return false;
        }
        Dequeue(F3, p);
        return true;
    }
    return false;
}

bool isGameOver(Queue F, Queue F1, Queue F3){
    return EmptyQueue(F) && EmptyQueue(F1) && EmptyQueue(F3);
}

//-----------------------------------------------------------------------------------------------------------------------------------
// PART I FUNCTIONS


int sumOfDigits(int value){
    int sum = 0;
    while(value != 0){
        sum += value % 10;
        value /= 10;
    }
    return sum;
}

void playRoundPartOne(Player *P1, Player *P2){
    int generatedValue = 0;
    
    printf("\n=======================================\n");
    printf("    PART I ROUND: %s vs %s\n", P1->name, P2->name);
    printf("=======================================\n");
    
    P1->currentScore = 0;
    P2->currentScore = 0;
    
    while (abs(P1->currentScore - P2->currentScore) < SCORE_DIFFERENCE_THRESHOLD && 
           generatedValue < MAX_VALUES_PART1) {
        
        // Player 1 turn
        printf("\n--- %s's Turn ---\n", P1->name);
        printf("Press ENTER to generate value...");
        getchar();

        int random1 = randomNumber();
        printf("Generated: %d\n", random1);

        int sum1 = sumOfDigits(random1);
        printf("Sum of digits: %d\n", sum1);

        if(sum1 % 5 == 0){
            printf("? %s scores! (+1)\n", P1->name);
            P1->currentScore++;
        } else {
            printf("? %s scores nothing\n", P1->name);
        }

        printf("Current: %s=%d, %s=%d\n", P1->name, P1->currentScore, P2->name, P2->currentScore);
        generatedValue++;
        
        if (abs(P1->currentScore - P2->currentScore) >= SCORE_DIFFERENCE_THRESHOLD) {
            break;
        }

        // Player 2 turn
        printf("\n--- %s's Turn ---\n", P2->name);
        printf("Press ENTER to generate value...");
        getchar();

        int random2 = randomNumber();
        printf("Generated: %d\n", random2);

        int sum2 = sumOfDigits(random2);
        printf("Sum of digits: %d\n", sum2);

        if(sum2 % 5 == 0){
            printf("? %s scores! (+1)\n", P2->name);
            P2->currentScore++;
        } else {
            printf("? %s scores nothing\n", P2->name);
        }

        printf("Current: %s=%d, %s=%d\n", P1->name, P1->currentScore, P2->name, P2->currentScore);
        generatedValue++;
    }
    
    printf("\n=======================================\n");
    printf("         ROUND FINISHED\n");
    printf("=======================================\n");
}

void classifyPartOne(Player *P, Queue *F, Queue *F1, Queue *F3, Stack *LP, Elist *LG){
    if (P->roundLost >= LOSS_THRESHOLD_PART1) {
        PUSH(LP, *P);
        printf("? %s ELIMINATED (5 losses)\n", P->name);
    }
    else if (P->roundWon >= WIN_THRESHOLD_PART1) {
        insertSortedLG(LG, *P);
        printf("? %s WINS THE GAME! (5 wins)\n", P->name);
    }
    else if (P->consecutiveWins >= CONSECUTIVE_WIN_THRESHOLD_PART1) {
        Enqueue(F1, *P);
        printf("? %s moved to Priority Queue F1 (3 consecutive wins)\n", P->name);
    }
    else if (P->roundLost >= CONSECUTIVE_LOSS_THRESHOLD_PART1) {  
        Enqueue(F3, *P);
        printf("? %s moved to Queue F3 (3 losses)\n", P->name);  
    }
    else {
        Enqueue(F, *P);
        printf("? %s returns to Queue F\n", P->name);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
// PART II FUNCTIONS

int gcd(int a, int b) {
    if (a == 0) return b;
    if (b == 0) return a;
    
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

bool digitAppears(int digit, int number) {
    if (number == 0 && digit == 0) return true;

    while (number > 0) {
        if (number % 10 == digit)
            return true;
        number /= 10;
    }
    return false;
}

bool gcdDigitCondition(int a, int b) {
    int g = gcd(a, b);

    while (g > 0) {
        int d = g % 10;
        if (digitAppears(d, a) || digitAppears(d, b))
            return true;
        g /= 10;
    }
    return false;
}

void playRoundPartTwo(Player *P1, Player *P2, bool *reachedThreshold) {
    int generatedValue = 0;

    printf("\n================================================\n");
    printf("    PART II ROUND: %s vs %s\n", P1->name, P2->name);
    printf("================================================\n");

    P1->currentScore = 0;
    P2->currentScore = 0;
    *reachedThreshold = false;

    while (abs(P1->currentScore - P2->currentScore) < SCORE_DIFFERENCE_THRESHOLD && 
           generatedValue < MAX_VALUES_PART2) {

        // Player 1 turn
        printf("\n--- %s's Turn ---\n", P1->name);
        printf("Press ENTER to generate values...");
        getchar();

        int a1 = randomNumber();
        int b1 = randomNumber();
        printf("Generated: %d and %d\n", a1, b1);

        int g1 = gcd(a1, b1);
        printf("GCD(%d, %d) = %d\n", a1, b1, g1);

        if (gcdDigitCondition(a1, b1)) {
            printf("? %s scores! (+1)\n", P1->name);
            P1->currentScore++;
        } else {
            printf("? %s scores nothing\n", P1->name);
        }

        printf("Current: %s=%d, %s=%d\n", P1->name, P1->currentScore, P2->name, P2->currentScore);
        generatedValue += 2;

        if (abs(P1->currentScore - P2->currentScore) >= SCORE_DIFFERENCE_THRESHOLD) {
            *reachedThreshold = true;
            break;
        }

        // Player 2 turn
        printf("\n--- %s's Turn ---\n", P2->name);
        printf("Press ENTER to generate values...");
        getchar();

        int a2 = randomNumber();
        int b2 = randomNumber();
        printf("Generated: %d and %d\n", a2, b2);

        int g2 = gcd(a2, b2);
        printf("GCD(%d, %d) = %d\n", a2, b2, g2);

        if (gcdDigitCondition(a2, b2)) {
            printf("? %s scores! (+1)\n", P2->name);
            P2->currentScore++;
        } else {
            printf("? %s scores nothing\n", P2->name);
        }

        printf("Current: %s=%d, %s=%d\n", P1->name, P1->currentScore, P2->name, P2->currentScore);
        generatedValue += 2;
        
        if (abs(P1->currentScore - P2->currentScore) >= SCORE_DIFFERENCE_THRESHOLD) {
            *reachedThreshold = true;
        }
    }

    printf("\n=======================================\n");
    printf("         ROUND FINISHED\n");
    if (*reachedThreshold) {
        printf("   (3-point difference reached)\n");
    } else {
        printf("   (16 values generated)\n");
    }
    printf("=======================================\n");
}

void classifyPartTwo(Player *P, Queue *F, Queue *F1, Queue *F3, Stack *LP, Elist *LG, bool wonByThreshold){
    // Check for elimination first
    if (P->roundLost >= LOSS_THRESHOLD_PART2) {
        PUSH(LP, *P);
        printf("? %s ELIMINATED (2 losses in Part II)\n", P->name);
        return;
    }
    
    // Check for ultimate winner
    if (P->consecutiveWins >= CONSECUTIVE_WIN_THRESHOLD_PART2) {
        insertSortedLG(LG, *P);
        printf("? %s WINS THE GAME! (2 consecutive wins in Part II)\n", P->name);
        return;
    }
    
    // For winners in Part II
    if (P->roundWon > P->roundLost) {
        if (wonByThreshold) {
            // Won by reaching 3-point difference -> goes to F1
            Enqueue(F1, *P);
            printf("? %s moved to Priority Queue F1 (won by 3-point difference)\n", P->name);
        } else {
            // Won after 16 values without 3-point difference -> goes to F
            Enqueue(F, *P);
            printf("? %s moved to Queue F (won after 16 values)\n", P->name);
        }
    }
    // For losers in Part II
    else {
        Enqueue(F3, *P);
        printf("? %s moved to Queue F3 (lost in Part II)\n", P->name);
    }
}

bool forcedGameOver(int strategy2Rounds, int n){
    return (strategy2Rounds >= 2 * n);
}

//-----------------------------------------------------------------------------------------------------------------------------------
// DISPLAY FUNCTIONS

void displayQueue(Queue F, char* name) {
    printf("\n+--- Queue %s ---\n", name);
    if (EmptyQueue(F)) {
        printf("| (empty)\n");
        printf("+----------------\n");
        return;
    }
    
    element *current = F.head;
    int count = 0;
    while (current != NULL) {
        printf("| %d. %s (ID:%d) W:%d L:%d CW:%d Score:%d\n",
               ++count, current->info.name, current->info.playerID,
               current->info.roundWon, current->info.roundLost,
               current->info.consecutiveWins,  // Added consecutive wins display
               current->info.totalScore);
        current = current->next;
    }

    printf("+---------------------------+\n");
}


void displayList(Elist LG) {
    printf("\n+--- Winners List (LG) ---\n");
    if (LG == NULL) {
        printf("|| (empty)\n");
        printf("+-------------------------\n");
        return;
    }
    
    element *current = LG;
    int rank = 1;
    while (current != NULL) {
        printf("|| #%d: %s (ID:%d) - Total Score: %d (W:%d L:%d)\n",
               rank++, current->info.name, current->info.playerID,
               current->info.totalScore, current->info.roundWon, 
               current->info.roundLost);
        current = current->next;
    }

    printf("+-------------------------\n");
}


void displayStack(Stack LP) {
    printf("\n+--- Losers List (LP) ---\n");
    if (EmptyStack(LP)) {
        printf("|| (empty)\n");
        printf("+------------------------\n");
        return;
    }
    
    element *current = LP;
    int count = 1;
    while (current != NULL) {
        printf("|| %d. %s (ID:%d) W:%d L:%d Score:%d\n",
               count++, current->info.name, current->info.playerID,
               current->info.roundWon, current->info.roundLost,
               current->info.totalScore);
        current = current->next;
    }
        printf("+------------------------\n");
}

void displayAllQueuesAndLists(Queue F, Queue F1, Queue F3, Elist LG, Stack LP) {
    printf("\n");
    printf("========================================\n");
    printf("||      CURRENT GAME STATE               ||\n");
    printf("========================================\n");
    
    displayQueue(F1, "F1 (Priority)");
    displayQueue(F, "F (Normal)");
    displayQueue(F3, "F3 (Low Priority)");
    displayList(LG);
    displayStack(LP);
}

void displayTop3Winners(Elist LG) {
    printf("\n");
    printf("========================================\n");
    printf("||          TOP 3 WINNERS                ||\n");
    printf("========================================\n");
    
    if (LG == NULL) {
        printf("No winners yet!\n");
        return;
    }
    
    element *current = LG;
    int rank = 1;
    
    while (current != NULL && rank <= 3) {
        if (rank == 1) printf("?? ");
        else if (rank == 2) printf("?? ");
        else if (rank == 3) printf("?? ");
        
        printf("Rank %d: %s (ID:%d)\n", rank, current->info.name, current->info.playerID);
        printf("   Total Score: %d | Wins: %d | Losses: %d\n", 
               current->info.totalScore, current->info.roundWon, current->info.roundLost);
        printf("   Part I: %dW/%dL | Part II: %dW/%dL\n\n",
               current->info.winsPartI, current->info.lossesPartI,
               current->info.winsPartII, current->info.lossesPartII);
        
        current = current->next;
        rank++;
    }
}

void displayRoundHistory(RoundHistory history) {
    printf("\n");
    printf("========================================\n");
    printf("||        ROUND HISTORY                  ||\n");
    printf("========================================\n");
    
    if (history == NULL) {
        printf("No rounds played yet!\n");
        return;
    }
    
    Round *current = history;
    while (current != NULL) {
        printf("\nRound %d (Strategy: Part %s)\n", 
               current->roundNumber, current->strategy == 1 ? "I" : "II");
        printf("Players: %s (ID:%d) vs %s (ID:%d)\n",
               current->player1Name, current->player1ID,
               current->player2Name, current->player2ID);
        printf("Score: %d - %d\n", current->player1Score, current->player2Score);
        printf("Winner: %s\n", current->winner);
        printf("Duration: %.0f seconds\n", difftime(current->endTime, current->startTime));
        printf("---------+--+--------+--+----------\n");
        
        current = current->next;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
// QUERY FUNCTIONS

void showPlayersWithNoWins(Elist LG, Stack LP) {
    printf("\n");
    printf(" =======================================\n");
    printf("||    PLAYERS WITH NO WINS               ||\n");
    printf(" =======================================\n");
    
    bool found = false;
    
    // Check losers list
    element *current = LP;
    while (current != NULL) {
        if (current->info.roundWon == 0) {
            printf("- %s (ID:%d) | Losses: %d\n", 
                   current->info.name, current->info.playerID, 
                   current->info.roundLost);
            found = true;
        }
        current = current->next;
    }
    
    if (!found) {
        printf("All players have won at least one round!\n");
    }
}

void showPlayersByWinsPartI(Elist LG, Stack LP, int wins) {
    printf("\n");
    printf(" =======================================\n");
    printf("||  PLAYERS WITH %d WIN(S) IN PART I    ||\n", wins);
    printf(" =======================================\n");
    
    bool found = false;
    
    // Check winners list
    element *current = LG;
    while (current != NULL) {
        if (current->info.winsPartI == wins) {
            printf("- %s (ID:%d) | Part I: %dW/%dL | Total Score: %d\n", 
                   current->info.name, current->info.playerID,
                   current->info.winsPartI, current->info.lossesPartI,
                   current->info.totalScore);
            found = true;
        }
        current = current->next;
    }
    
    // Check losers list
    current = LP;
    while (current != NULL) {
        if (current->info.winsPartI == wins) {
            printf("- %s (ID:%d) | Part I: %dW/%dL | Total Score: %d\n", 
                   current->info.name, current->info.playerID,
                   current->info.winsPartI, current->info.lossesPartI,
                   current->info.totalScore);
            found = true;
        }
        current = current->next;
    }
    
    if (!found) {
        printf("No players with exactly %d win(s) in Part I.\n", wins);
    }
}

void showPlayersByLossesPartI(Elist LG, Stack LP, int losses) {
    printf("\n");
    printf(" =======================================\n");
    printf("|| PLAYERS WITH %d LOSS(ES) IN PART I  ||\n", losses);
    printf(" =======================================\n");
    
    bool found = false;
    
    // Check winners list
    element *current = LG;
    while (current != NULL) {
        if (current->info.lossesPartI == losses) {
            printf("- %s (ID:%d) | Part I: %dW/%dL | Total Score: %d\n", 
                   current->info.name, current->info.playerID,
                   current->info.winsPartI, current->info.lossesPartI,
                   current->info.totalScore);
            found = true;
        }
        current = current->next;
    }
    
    // Check losers list
    current = LP;
    while (current != NULL) {
        if (current->info.lossesPartI == losses) {
            printf("- %s (ID:%d) | Part I: %dW/%dL | Total Score: %d\n", 
                   current->info.name, current->info.playerID,
                   current->info.winsPartI, current->info.lossesPartI,
                   current->info.totalScore);
            found = true;
        }
        current = current->next;
    }
    
    if (!found) {
        printf("No players with exactly %d loss(es) in Part I.\n", losses);
    }
}

void showPlayersByWinsPartII(Elist LG, Stack LP, int wins) {
    printf("\n");
    printf(" ======================================\n");
    printf("|| PLAYERS WITH %d WIN(S) IN PART II  ||\n", wins);
    printf(" ======================================\n");
    
    bool found = false;
    
    // Check winners list
    element *current = LG;
    while (current != NULL) {
        if (current->info.winsPartII == wins) {
            printf("- %s (ID:%d) | Part II: %dW/%dL | Total Score: %d\n", 
                   current->info.name, current->info.playerID,
                   current->info.winsPartII, current->info.lossesPartII,
                   current->info.totalScore);
            found = true;
        }
        current = current->next;
    }
    
    // Check losers list
    current = LP;
    while (current != NULL) {
        if (current->info.winsPartII == wins) {
            printf("- %s (ID:%d) | Part II: %dW/%dL | Total Score: %d\n", 
                   current->info.name, current->info.playerID,
                   current->info.winsPartII, current->info.lossesPartII,
                   current->info.totalScore);
            found = true;
        }
        current = current->next;
    }
    
    if (!found) {
        printf("No players with exactly %d win(s) in Part II.\n", wins);
    }
}

void showPlayersByLossesPartII(Elist LG, Stack LP, int losses) {
    printf("\n");
    printf(" =========================================\n");
    printf("|| PLAYERS WITH %d LOSS(ES) IN PART II   ||\n", losses);
    printf(" =========================================\n");
    
    bool found = false;
    
    // Check winners list
    element *current = LG;
    while (current != NULL) {
        if (current->info.lossesPartII == losses) {
            printf("- %s (ID:%d) | Part II: %dW/%dL | Total Score: %d\n", 
                   current->info.name, current->info.playerID,
                   current->info.winsPartII, current->info.lossesPartII,
                   current->info.totalScore);
            found = true;
        }
        current = current->next;
    }
    
    // Check losers list
    current = LP;
    while (current != NULL) {
        if (current->info.lossesPartII == losses) {
            printf("- %s (ID:%d) | Part II: %dW/%dL | Total Score: %d\n", 
                   current->info.name, current->info.playerID,
                   current->info.winsPartII, current->info.lossesPartII,
                   current->info.totalScore);
            found = true;
        }
        current = current->next;
    }
    
    if (!found) {
        printf("No players with exactly %d loss(es) in Part II.\n", losses);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
// ROUND HISTORY FUNCTIONS

void addRound(RoundHistory *history, int roundNum, Player p1, Player p2, 
              char* winner, time_t start, time_t end, int strategy) {
    Round *newRound = (Round*)malloc(sizeof(Round));
    if (newRound == NULL) {
        fprintf(stderr, "Memory allocation error in addRound\n");
        return;
    }
    
    newRound->roundNumber = roundNum;
    strcpy(newRound->player1Name, p1.name);
    strcpy(newRound->player2Name, p2.name);
    newRound->player1ID = p1.playerID;
    newRound->player2ID = p2.playerID;
    newRound->player1Score = p1.currentScore;
    newRound->player2Score = p2.currentScore;
    strcpy(newRound->winner, winner);
    newRound->startTime = start;
    newRound->endTime = end;
    newRound->strategy = strategy;
    newRound->next = NULL;
    
    if (*history == NULL) {
        *history = newRound;
    } else {
        Round *current = *history;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newRound;
    }
}

void freeRoundHistory(RoundHistory *history) {
    Round *current = *history;
    while (current != NULL) {
        Round *temp = current;
        current = current->next;
        free(temp);
    }
    *history = NULL;
}

//-----------------------------------------------------------------------------------------------------------------------------------
// MEMORY CLEANUP FUNCTIONS

void freeQueue(Queue *F) {
    while (!EmptyQueue(*F)) {
        Player temp;
        Dequeue(F, &temp);
    }
}

void freeList(Elist *LG) {
    while (*LG != NULL) {
        element *temp = *LG;
        *LG = (*LG)->next;
        free(temp);
    }
}

void freeStack(Stack *LP) {
    while (!EmptyStack(*LP)) {
        Player temp;
        POP(LP, &temp);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
// MAIN FUNCTION

int main() {
    srand(time(NULL));

    int n;
    int roundsPlayed = 0;
    int strategy2Rounds = 0;
    int strategy = 1;

    Queue F, F1, F3;
    Elist LG = NULL;
    Stack LP = NULL;
    RoundHistory history = NULL;

    Initqueue(&F);
    Initqueue(&F1);
    Initqueue(&F3);
    InitStack(&LP);

    // Game start time
    time_t gameStart = time(NULL);
    printf("\n");
    printf(" =====================================\n");
    printf("||     TOURNAMENT GAME SYSTEM        ||\n");
    printf(" =====================================\n");
    printf("Game started at: %s", ctime(&gameStart));

    printf("\nEnter number of players: ");
    scanf("%d", &n);
    getchar();

    if (n <= 0) {
        printf("Invalid number of players!\n");
        return 1;
    }

    AutoFill(&F, n);
    printf("\n%d players registered successfully!\n", n);
    printf("Press ENTER to start the tournament...");
    getchar();

    Player currentPlayer, challenger;
    bool hasCurrentPlayer = false;

    // MAIN GAME LOOP
    while (!isGameOver(F, F1, F3)) {

        // Strategy switch
        if (strategy == 1 && roundsPlayed >= 3 * n) {
            strategy = 2;
            printf("\n");
            printf(" ======================================\n");
            printf("||   STRATEGY CHANGED TO PART II      ||\n");
            printf("||   (After %d rounds = 3n)           ||\n", roundsPlayed);
            printf(" ======================================\n");
            printf("Press ENTER to continue...");
            getchar();
        }

        // Forced game over check for Part II
        if (strategy == 2 && forcedGameOver(strategy2Rounds, n)) {
            printf("\n");
            printf(" =========================================\n");
            printf("||   FORCED GAME OVER (Part II)          ||\n");
            printf("||   (After %d rounds = 2n in Part II)   ||\n", strategy2Rounds);
            printf(" =========================================\n");

            // Move remaining players
            Player temp;
            while (!EmptyQueue(F1)) {
                Dequeue(&F1, &temp);
                insertSortedLG(&LG, temp);
            }
            while (!EmptyQueue(F)) {
                Dequeue(&F, &temp);
                PUSH(&LP, temp);
            }
            while (!EmptyQueue(F3)) {
                Dequeue(&F3, &temp);
                PUSH(&LP, temp);
            }
            break;
        }

        // Select current player
        if (!hasCurrentPlayer) {
            if (!GetPlayer(&F1, &F, &F3, &currentPlayer, &LP))
                break;
            hasCurrentPlayer = true;
        }

        // Select challenger
        if (!GetPlayer(&F1, &F, &F3, &challenger, &LP))
            break;

        // Record round start time
        time_t roundStart = time(NULL);

        // Play round
        printf("\n");
        printf(" ==================================\n");
        printf("||       ROUND %d                 ||\n", roundsPlayed + 1);
        printf(" ==================================\n");
        
        bool reachedThreshold = false;
        if (strategy == 1) {
            playRoundPartOne(&currentPlayer, &challenger);
        } else {
            playRoundPartTwo(&currentPlayer, &challenger, &reachedThreshold);
            strategy2Rounds++;
        }

        // Record round end time
        time_t roundEnd = time(NULL);

        // Update statistics
        updatePlayersStatistics(&currentPlayer, &challenger, strategy);
        roundsPlayed++;

        // Determine winner for round history
        char winner[50];
        if (currentPlayer.currentScore > challenger.currentScore) {
            strcpy(winner, currentPlayer.name);
        } else if (challenger.currentScore > currentPlayer.currentScore) {
            strcpy(winner, challenger.name);
        } else {
            strcpy(winner, "DRAW");
        }

        // Add to round history
        addRound(&history, roundsPlayed, currentPlayer, challenger, 
                 winner, roundStart, roundEnd, strategy);

        // Decide outcome and classify players
        if (currentPlayer.currentScore > challenger.currentScore) {
            if (strategy == 1) {
                // Part I: Loser is always classified
                classifyPartOne(&challenger, &F, &F1, &F3, &LP, &LG);
                
                // ? FIXED: Check if winner should be classified (3 consecutive wins OR 5 total wins)
                if (currentPlayer.consecutiveWins >= CONSECUTIVE_WIN_THRESHOLD_PART1 || 
                    currentPlayer.roundWon >= WIN_THRESHOLD_PART1) {
                    classifyPartOne(&currentPlayer, &F, &F1, &F3, &LP, &LG);
                    hasCurrentPlayer = false;  // Need new player
                }
                // Otherwise winner continues playing (hasCurrentPlayer stays true)
            } else {
                // Part II: BOTH players are classified, NO ONE continues
                classifyPartTwo(&challenger, &F, &F1, &F3, &LP, &LG, false); // Loser always gets false
                classifyPartTwo(&currentPlayer, &F, &F1, &F3, &LP, &LG, reachedThreshold); // Winner status depends on threshold
                hasCurrentPlayer = false; // Reset for next pair
            }
        }
        else if (challenger.currentScore > currentPlayer.currentScore) {
            if (strategy == 1) {
                // Part I: Loser is always classified
                classifyPartOne(&currentPlayer, &F, &F1, &F3, &LP, &LG);
                
                // ? FIXED: Check if winner should be classified (3 consecutive wins OR 5 total wins)
                if (challenger.consecutiveWins >= CONSECUTIVE_WIN_THRESHOLD_PART1 || 
                    challenger.roundWon >= WIN_THRESHOLD_PART1) {
                    classifyPartOne(&challenger, &F, &F1, &F3, &LP, &LG);
                    hasCurrentPlayer = false;  // Need new player
                } else {
                    // Challenger becomes the new current player
                    currentPlayer = challenger;
                }
            } else {
                // Part II: BOTH players are classified, NO ONE continues
                classifyPartTwo(&currentPlayer, &F, &F1, &F3, &LP, &LG, false); // Loser always gets false
                classifyPartTwo(&challenger, &F, &F1, &F3, &LP, &LG, reachedThreshold); // Winner status depends on threshold
                hasCurrentPlayer = false; // Reset for next pair
            }
        }
        else {
            // Draw - both return to queue F
            Enqueue(&F, currentPlayer);
            Enqueue(&F, challenger);
            hasCurrentPlayer = false;
        }

        // Display current game state after each round
        displayAllQueuesAndLists(F, F1, F3, LG, LP);
        
        printf("\nPress ENTER for next round...");
        getchar();
    }

    // Classify remaining current player if exists
    if (hasCurrentPlayer) {
        if (strategy == 1)
            classifyPartOne(&currentPlayer, &F, &F1, &F3, &LP, &LG);
        else
            classifyPartTwo(&currentPlayer, &F, &F1, &F3, &LP, &LG, false);
    }

    // Game end time
    time_t gameEnd = time(NULL);

    // END OF GAME - DISPLAY RESULTS
    printf("\n");
    printf(" ==================================\n");
    printf("||          GAME OVER             ||\n");
    printf(" ==================================\n");
    printf("Game ended at: %s", ctime(&gameEnd));
    printf("Total duration: %.0f seconds\n", difftime(gameEnd, gameStart));
    printf("Total rounds played: %d\n", roundsPlayed);
    printf("Part I rounds: %d\n", roundsPlayed - strategy2Rounds);
    printf("Part II rounds: %d\n", strategy2Rounds);

    // Display final state
    displayAllQueuesAndLists(F, F1, F3, LG, LP);

    // Display top 3 winners
    displayTop3Winners(LG);

    // QUERIES SECTION
    printf("\n");
    printf(" ==================================\n");
    printf("||     STATISTICS & QUERIES       ||\n");
    printf(" ==================================\n");

    showPlayersWithNoWins(LG, LP);
    
    printf("\n--- PART I STATISTICS ---\n");
    for (int i = 1; i <= 3; i++) {
        showPlayersByWinsPartI(LG, LP, i);
        showPlayersByLossesPartI(LG, LP, i);
    }
    
    printf("\n--- PART II STATISTICS ---\n");
    for (int i = 1; i <= 2; i++) {
        showPlayersByWinsPartII(LG, LP, i);
        showPlayersByLossesPartII(LG, LP, i);
    }

    // Display round history (optional - can be commented out if too long)
    // displayRoundHistory(history);

    // Fairness analysis
    printf("\n");
    printf(" ==================================\n");
    printf("||       FAIRNESS ANALYSIS        ||\n");
    printf(" ==================================\n");
    printf("The game demonstrates fairness through:\n");
    printf("1. Random value generation for all players\n");
    printf("2. Priority queue system rewarding consecutive wins\n");
    printf("3. Clear elimination rules (5 losses in Part I, 2 in Part II)\n");
    printf("4. Immediate winner advancement in Part II\n");
    printf("5. Equal opportunity through queue rotation\n");

    // Memory cleanup
    freeQueue(&F);
    freeQueue(&F1);
    freeQueue(&F3);
    freeList(&LG);
    freeStack(&LP);
    freeRoundHistory(&history);

    printf("\nThank you for playing!\n");

    return 0;
}
