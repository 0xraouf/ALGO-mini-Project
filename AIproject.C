#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>

//--------------------------------------------------------------------------------------------------------------
// CONSTANTS
#define SCORE_DIFFERENCE_THRESHOLD 3
#define WIN_THRESHOLD_PART1 5        // [cite: 40]
#define LOSS_THRESHOLD_PART1 5       // [cite: 39]
#define CONSECUTIVE_WIN_THRESHOLD_PART1 3 // [cite: 37]
#define LOSS_F3_THRESHOLD 3          // [cite: 38]
#define MAX_VALUES_PART1 12          // [cite: 33]

#define MAX_VALUES_PART2 16          // 
#define WIN_THRESHOLD_PART2 2        // [cite: 58] (Consecutive)
#define LOSS_THRESHOLD_PART2 2       // [cite: 57] (Cumulative)

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
    
    // Statistics for queries [cite: 83-87]
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

// Stack used for LP (Losers List) - Though PDF calls it a "List", a Stack is valid for storage
typedef element *Stack; 
typedef element *Elist; // For LG (Winners List sorted)

// Round history structure for reporting
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

// Data Structure Operations
void Initqueue(Queue *F);
void Enqueue(Queue *F, Player p);
void Dequeue(Queue *F, Player *p);
bool EmptyQueue(Queue F);
void InitStack(Stack *S);
void PUSH(Stack *S, Player P);
void POP(Stack *S, Player *P);
bool EmptyStack(Stack S);
void insertSortedLG(Elist *LG, Player p); // [cite: 40, 58]

// Game Logic
int randomNumber();
void AutoFill(Queue *F, int n);
void updateStats(Player *Winner, Player *Loser, int strategy, bool isDraw);
bool GetPlayer(Queue *F1, Queue *F, Queue *F3, Player *p, Stack *LP);
bool isGameOver(Queue F, Queue F1, Queue F3, Player *currentHold);

// Part I
int sumOfDigits(int value);
void playRoundPartOne(Player *P1, Player *P2);
// Returns true if player is removed from play (moved to Queue/List), false if they stay "King of the Hill"
bool classifyPartOne(Player *P, Queue *F, Queue *F1, Queue *F3, Stack *LP, Elist *LG, bool isWinner);

// Part II
int gcd(int a, int b);
bool digitAppears(int digit, int number);
bool gcdDigitCondition(int a, int b);
void playRoundPartTwo(Player *P1, Player *P2, bool *reachedThreshold);
void classifyPartTwo(Player *P, Queue *F, Queue *F1, Queue *F3, Stack *LP, Elist *LG, bool isWinner, bool wonByThreshold);
bool forcedGameOver(int strategy2Rounds, int n);

// Display
void displayQueue(Queue F, char* name);
void displayList(Elist LG);
void displayStack(Stack LP);
void displayAllQueuesAndLists(Queue F, Queue F1, Queue F3, Elist LG, Stack LP);
void displayTop3Winners(Elist LG);
void showPlayersWithNoWins(Elist LG, Stack LP);

// Memory
void freeQueue(Queue *F);
void freeList(Elist *LG);
void freeStack(Stack *LP);
void freeRoundHistory(RoundHistory *history);
void addRound(RoundHistory *history, int roundNum, Player p1, Player p2, char* winner, time_t start, time_t end, int strategy);

//-----------------------------------------------------------------------------------------------------------------------------------
// QUEUE OPERATIONS

void Initqueue(Queue *F) {
    F->head = NULL;
    F->tail = NULL;
}

void Enqueue(Queue *F, Player p) {
    element *N = (element*)malloc(sizeof(element));
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
    if (F->head == NULL) return;
    *p = F->head->info;
    element *temp = F->head;
    F->head = F->head->next;
    free(temp);
    if (F->head == NULL) F->tail = NULL;
}

bool EmptyQueue(Queue F) {
    return (F.head == NULL);
}

//-----------------------------------------------------------------------------------------------------------------------------------
// STACK / LIST OPERATIONS

void InitStack(Stack *S) { *S = NULL; }

void PUSH(Stack *S, Player P) {
    element *N = (element*)malloc(sizeof(element));
    N->info = P;
    N->next = *S;
    *S = N;
}

void POP(Stack *S, Player *P) {
    if (*S == NULL) return;
    *P = (*S)->info;
    element *temp = *S;
    *S = (*S)->next;
    free(temp); 
}

bool EmptyStack(Stack S) { return (S == NULL); }

// LG is sorted by Total Score [cite: 40]
void insertSortedLG(Elist *LG, Player p) {
    element *N = (element*)malloc(sizeof(element));
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
    return rand() % 100000; 
}

void AutoFill(Queue *F, int n) {
    // [cite: 21] Initial queue F contains n players
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

// Update counters only, logic for movement is in classify functions
void updateStats(Player *Winner, Player *Loser, int strategy, bool isDraw) {
    if (isDraw) {
        Winner->consecutiveWins = 0;
        Winner->consecutiveLosses = 0;
        Loser->consecutiveWins = 0;
        Loser->consecutiveLosses = 0;
        Winner->totalScore += Winner->currentScore;
        Loser->totalScore += Loser->currentScore;
        printf("🤝 DRAW!\n");
        return;
    }

    // Winner Updates
    Winner->roundWon++;
    Winner->consecutiveWins++;
    Winner->consecutiveLosses = 0;
    Winner->totalScore += Winner->currentScore;
    if(strategy == 1) Winner->winsPartI++; else Winner->winsPartII++;

    // Loser Updates
    Loser->roundLost++;
    Loser->consecutiveLosses++;
    Loser->consecutiveWins = 0;
    Loser->totalScore += Loser->currentScore;
    if(strategy == 1) Loser->lossesPartI++; else Loser->lossesPartII++;

    printf("🏆 WINNER: %s (Score: %d)\n", Winner->name, Winner->currentScore);
    printf("💀 LOSER:  %s (Score: %d)\n", Loser->name, Loser->currentScore);
}

// [cite: 41-47] Priority Selection: F1 > F > F3
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
        // [cite: 47] If only one player remains in F3 (and others empty), declared loser -> LP
        if (F3->head == F3->tail) {
            Player last;
            Dequeue(F3, &last);
            PUSH(LP, last);
            printf("ℹ️  Last player in F3 eliminated to LP [cite: 47]\n");
            return false;
        }
        Dequeue(F3, p);
        return true;
    }
    return false;
}

bool isGameOver(Queue F, Queue F1, Queue F3, Player *currentHold) {
    // [cite: 69] Game stops when all queues are empty
    // NOTE: currentHold represents a player currently "on the court" (Winner of previous round)
    // If someone is on the court, the game isn't over yet.
    return EmptyQueue(F) && EmptyQueue(F1) && EmptyQueue(F3) && (currentHold == NULL);
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
    int generatedValues = 0;
    P1->currentScore = 0;
    P2->currentScore = 0;
    
    // [cite: 33] Stop if gap >= 3 OR 12 values generated
    while (abs(P1->currentScore - P2->currentScore) < SCORE_DIFFERENCE_THRESHOLD && 
           generatedValues < MAX_VALUES_PART1) {
        
        // P1
        int r1 = randomNumber();
        if(sumOfDigits(r1) % 5 == 0) P1->currentScore++; // [cite: 25]
        generatedValues++;
        if (abs(P1->currentScore - P2->currentScore) >= SCORE_DIFFERENCE_THRESHOLD) break;

        // P2
        int r2 = randomNumber();
        if(sumOfDigits(r2) % 5 == 0) P2->currentScore++;
        generatedValues++;
    }
}

// Returns TRUE if player is moved to a queue/list (removed from court)
// Returns FALSE if player stays to play again (King of the Hill) 
bool classifyPartOne(Player *P, Queue *F, Queue *F1, Queue *F3, Stack *LP, Elist *LG, bool isWinner) {
    
    // [cite: 39] 5 losses total -> LP
    if (P->roundLost >= LOSS_THRESHOLD_PART1) {
        PUSH(LP, *P);
        printf("→ %s ELIMINATED to LP (5 losses)\n", P->name);
        return true;
    }
    
    // [cite: 40] 5 wins total -> LG
    if (P->roundWon >= WIN_THRESHOLD_PART1) {
        insertSortedLG(LG, *P);
        printf("→ %s PROMOTED to LG (5 wins)\n", P->name);
        return true;
    }

    if (isWinner) {
        // [cite: 37] 3 consecutive wins -> F1
        if (P->consecutiveWins >= CONSECUTIVE_WIN_THRESHOLD_PART1) {
            Enqueue(F1, *P);
            printf("→ %s moved to Priority F1 (3 cons. wins)\n", P->name);
            return true; // Moved to queue, stops being "King of the Hill"
        }
        //  Winner plays another game (Stays on court)
        printf("→ %s stays as Current Player (King of the Hill)\n", P->name);
        return false; 
    } else {
        // Loser Logic
        // [cite: 38] 3 losses (consecutive or separated) -> F3
        // Note: We check if losses >= 3. If they have 4 losses, they still go to F3.
        if (P->roundLost >= LOSS_F3_THRESHOLD) {
            Enqueue(F3, *P);
            printf("→ %s moved to F3 (>=3 losses)\n", P->name);
        } else {
            //  Loser placed in tail of F
            Enqueue(F, *P);
            printf("→ %s moved to F (Standard)\n", P->name);
        }
        return true; // Loser always leaves the court
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
// PART II FUNCTIONS

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

bool digitAppears(int digit, int number) {
    char strNum[20];
    sprintf(strNum, "%d", number);
    char d = digit + '0';
    return (strchr(strNum, d) != NULL);
}

bool gcdDigitCondition(int a, int b) {
    int g = gcd(a, b); // [cite: 50]
    int tempG = g;
    // Check if any digit of GCD appears in a OR b
    while (tempG > 0) {
        int d = tempG % 10;
        if (digitAppears(d, a) || digitAppears(d, b)) return true;
        tempG /= 10;
    }
    return false;
}

void playRoundPartTwo(Player *P1, Player *P2, bool *reachedThreshold) {
    int generatedValues = 0;
    P1->currentScore = 0;
    P2->currentScore = 0;
    *reachedThreshold = false;

    // [cite: 52, 54] Stop if gap >= 3 OR 16 values generated
    while (abs(P1->currentScore - P2->currentScore) < SCORE_DIFFERENCE_THRESHOLD && 
           generatedValues < MAX_VALUES_PART2) {
        
        // P1 Turn
        int a1 = randomNumber(), b1 = randomNumber();
        if (gcdDigitCondition(a1, b1)) P1->currentScore++; // [cite: 50]
        generatedValues += 2; // 2 numbers generated
        if (abs(P1->currentScore - P2->currentScore) >= SCORE_DIFFERENCE_THRESHOLD) {
            *reachedThreshold = true;
            break;
        }

        // P2 Turn
        int a2 = randomNumber(), b2 = randomNumber();
        if (gcdDigitCondition(a2, b2)) P2->currentScore++;
        generatedValues += 2;
        if (abs(P1->currentScore - P2->currentScore) >= SCORE_DIFFERENCE_THRESHOLD) {
            *reachedThreshold = true;
            break;
        }
    }
}

void classifyPartTwo(Player *P, Queue *F, Queue *F1, Queue *F3, Stack *LP, Elist *LG, bool isWinner, bool wonByThreshold){
    // [cite: 57] 2 losses total -> LP
    if (P->roundLost >= LOSS_THRESHOLD_PART2) {
        PUSH(LP, *P);
        printf("→ %s ELIMINATED to LP (2 losses Part II)\n", P->name);
        return;
    }

    // [cite: 58] 2 consecutive wins -> LG
    if (P->consecutiveWins >= WIN_THRESHOLD_PART2) { // 2 wins
        insertSortedLG(LG, *P);
        printf("→ %s PROMOTED to LG (2 cons. wins Part II)\n", P->name);
        return;
    }

    if (isWinner) {
        if (wonByThreshold) {
            //  Won by 3-point diff -> F1
            Enqueue(F1, *P);
            printf("→ %s moved to F1 (Won by 3-pt diff)\n", P->name);
        } else {
            //  Won by score after 16 values -> F
            Enqueue(F, *P);
            printf("→ %s moved to F (Won by timeout)\n", P->name);
        }
    } else {
        // [cite: 53, 54] Loser -> F3
        Enqueue(F3, *P);
        printf("→ %s moved to F3 (Lost Part II)\n", P->name);
    }
}

bool forcedGameOver(int strategy2Rounds, int n){
    return (strategy2Rounds >= 2 * n); // 
}

//-----------------------------------------------------------------------------------------------------------------------------------
// DISPLAY FUNCTIONS (Kept mostly same, optimized output)

void displayQueue(Queue F, char* name) {
    printf("   Queue %s: ", name);
    if (EmptyQueue(F)) { printf("(empty)\n"); return; }
    element *curr = F.head;
    while(curr) { printf("[%s Sc:%d] ", curr->info.name, curr->info.totalScore); curr = curr->next; }
    printf("\n");
}

void displayAllQueuesAndLists(Queue F, Queue F1, Queue F3, Elist LG, Stack LP) {
    printf("   ----------------STATE----------------\n");
    displayQueue(F1, "F1");
    displayQueue(F,  "F ");
    displayQueue(F3, "F3");
    printf("   -------------------------------------\n");
}

void displayTop3Winners(Elist LG) {
    printf("\n🏆 TOP 3 WINNERS [cite: 81]\n");
    element *curr = LG;
    int rank = 1;
    while (curr != NULL && rank <= 3) {
        printf("%d. %s (Total Score: %d)\n", rank++, curr->info.name, curr->info.totalScore);
        curr = curr->next;
    }
}

void showPlayersWithNoWins(Elist LG, Stack LP) {
    printf("\n📊 Players with NO wins[cite: 83]:\n");
    bool found = false;
    element *curr = LP;
    while(curr) {
        if(curr->info.roundWon == 0) { printf(" - %s\n", curr->info.name); found=true;}
        curr = curr->next;
    }
    // Also check LG (unlikely but possible if moved by complex rules? No, LG requires wins)
    if(!found) printf(" None.\n");
}

// History Helper
void addRound(RoundHistory *history, int roundNum, Player p1, Player p2, char* winner, time_t start, time_t end, int strategy) {
    Round *newRound = (Round*)malloc(sizeof(Round));
    newRound->roundNumber = roundNum;
    strcpy(newRound->player1Name, p1.name);
    strcpy(newRound->player2Name, p2.name);
    newRound->player1Score = p1.currentScore;
    newRound->player2Score = p2.currentScore;
    strcpy(newRound->winner, winner);
    newRound->startTime = start;
    newRound->endTime = end;
    newRound->strategy = strategy;
    newRound->next = *history; // Insert at head for simpler history or traverse for tail
    *history = newRound;
}
void freeRoundHistory(RoundHistory *history) {
    while(*history) { Round *t = *history; *history = (*history)->next; free(t); }
}

//-----------------------------------------------------------------------------------------------------------------------------------
// MEMORY CLEANUP
void freeQueue(Queue *F) { while(!EmptyQueue(*F)) { Player p; Dequeue(F, &p); } }
void freeList(Elist *LG) { while(*LG) { element *t = *LG; *LG = (*LG)->next; free(t); } }
void freeStack(Stack *LP) { while(!EmptyStack(*LP)) { Player p; POP(LP, &p); } }

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

    printf("Enter number of players (n): ");
    scanf("%d", &n);
    AutoFill(&F, n);

    Player *currentPlayer = NULL; // Pointer to handle King of the Hill logic
    Player tempPlayer;            // Storage for actual player data
    Player challenger;
    
    // Initial fetch for first player
    Player p1_storage;
    if(GetPlayer(&F1, &F, &F3, &p1_storage, &LP)) {
        currentPlayer = &p1_storage;
    }

    // MAIN LOOP
    while (!isGameOver(F, F1, F3, currentPlayer)) {

        // Check Strategy Change [cite: 49]
        if (strategy == 1 && roundsPlayed >= 3 * n) {
            strategy = 2;
            printf("\n⚠️  STRATEGY CHANGE: Entering Part II [cite: 49]\n");
            // In Part II, no King of the Hill. Current player must be queued properly before starting logic?
            // Actually, Part II rules imply queuing after every round.
            // If we hold a player, put them back in F1 or F to follow standard priority selection for Part II.
            if(currentPlayer != NULL) {
                Enqueue(&F, *currentPlayer);
                currentPlayer = NULL; 
            }
        }

        // Check Forced Game Over Part II 
        if (strategy == 2 && forcedGameOver(strategy2Rounds, n)) {
            printf("\n🛑 FORCED GAME OVER (2n rounds in Part II)\n");
            // F1 -> LG, F/F3 -> LP
            Player p;
            while(!EmptyQueue(F1)) { Dequeue(&F1, &p); insertSortedLG(&LG, p); }
            while(!EmptyQueue(F))  { Dequeue(&F, &p); PUSH(&LP, p); }
            while(!EmptyQueue(F3)) { Dequeue(&F3, &p); PUSH(&LP, p); }
            break;
        }

        // Ensure we have a Player 1
        if (currentPlayer == NULL) {
             if (!GetPlayer(&F1, &F, &F3, &p1_storage, &LP)) break;
             currentPlayer = &p1_storage;
        }

        // Get Challenger
        if (!GetPlayer(&F1, &F, &F3, &challenger, &LP)) {
            // If no challenger, game might be over, or P1 declared loser if from F3 logic?
            // If P1 exists but no challenger, P1 technically wins by default or waits. 
            // Given "isGameOver" check, this likely triggers end.
            break; 
        }

        roundsPlayed++;
        if(strategy == 2) strategy2Rounds++;

        time_t start = time(NULL);
        printf("\n--- Round %d (%s) : %s vs %s ---\n", roundsPlayed, strategy==1?"Part I":"Part II", currentPlayer->name, challenger.name);

        bool reachedThreshold = false;
        if (strategy == 1) {
            playRoundPartOne(currentPlayer, &challenger);
        } else {
            playRoundPartTwo(currentPlayer, &challenger, &reachedThreshold);
        }
        time_t end = time(NULL);

        // Determine result
        bool p1Wins = (currentPlayer->currentScore > challenger.currentScore);
        bool p2Wins = (challenger.currentScore > currentPlayer->currentScore);
        bool draw   = (!p1Wins && !p2Wins);

        char winnerName[50] = "Draw";
        if(p1Wins) strcpy(winnerName, currentPlayer->name);
        if(p2Wins) strcpy(winnerName, challenger.name);

        updateStats(p1Wins ? currentPlayer : (p2Wins ? &challenger : currentPlayer), 
                    p1Wins ? &challenger : (p2Wins ? currentPlayer : &challenger), 
                    strategy, draw);
        
        addRound(&history, roundsPlayed, *currentPlayer, challenger, winnerName, start, end, strategy);

        // CLASSIFICATION & MOVEMENT
        
        if (strategy == 1) {
            if (draw) {
                // [cite: 34] Draw -> Both to tail of F
                Enqueue(&F, *currentPlayer);
                Enqueue(&F, challenger);
                currentPlayer = NULL; // New players needed
            } else if (p1Wins) {
                // Classify Loser first (P2)
                classifyPartOne(&challenger, &F, &F1, &F3, &LP, &LG, false);
                
                // Classify Winner (P1)
                // Returns TRUE if moved to queue (removed from court), FALSE if stays
                if (classifyPartOne(currentPlayer, &F, &F1, &F3, &LP, &LG, true)) {
                    currentPlayer = NULL; // Moved to F1/LG, need new player
                } 
                // Else: currentPlayer stays (King of the Hill)
            } else { // P2 Wins
                // Classify Loser (P1)
                classifyPartOne(currentPlayer, &F, &F1, &F3, &LP, &LG, false);
                
                // Classify Winner (P2)
                // P2 becomes the new "Current Player" if they don't move queue
                if (classifyPartOne(&challenger, &F, &F1, &F3, &LP, &LG, true)) {
                    currentPlayer = NULL; 
                } else {
                    // Challenger takes the hill
                    p1_storage = challenger; // Copy data to storage
                    currentPlayer = &p1_storage;
                }
            }
        } 
        else { // STRATEGY 2
            // [cite: 53-55] Part II implies immediate re-queuing, no King of the Hill mention like Part I
            if (draw) {
                // [cite: 55] Draw -> Tail of F
                Enqueue(&F, *currentPlayer);
                Enqueue(&F, challenger);
            } else if (p1Wins) {
                classifyPartTwo(&challenger, &F, &F1, &F3, &LP, &LG, false, false);
                classifyPartTwo(currentPlayer, &F, &F1, &F3, &LP, &LG, true, reachedThreshold);
            } else { // P2 Wins
                classifyPartTwo(currentPlayer, &F, &F1, &F3, &LP, &LG, false, false);
                classifyPartTwo(&challenger, &F, &F1, &F3, &LP, &LG, true, reachedThreshold);
            }
            currentPlayer = NULL; // In Part II, everyone goes back to queues
        }

        displayAllQueuesAndLists(F, F1, F3, LG, LP);
    }

    // FINAL OUTPUTS
    printf("\n================ GAME OVER ================\n");
    displayTop3Winners(LG);
    showPlayersWithNoWins(LG, LP);

    // [cite: 85] Example specific queries can be added here iterating LG/LP like showPlayersWithNoWins

    // Cleanup
    freeQueue(&F); freeQueue(&F1); freeQueue(&F3);
    freeList(&LG); freeStack(&LP); freeRoundHistory(&history);

    return 0;
}