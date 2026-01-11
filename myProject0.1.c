#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>

//--------------------------------------------------------------------------------------------------------------
// Records
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

typedef struct element{
    struct Player info;
    struct element *next; 
}element;

typedef struct Queue {
    element *head;
    element *tail;
}Queue;

typedef element *Stack; //(lifo)
typedef element *Elist; //(fifo)

//-----------------------------------------------------------------------------------------------------------------------------------
// Functions :
// Queue operations :
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

int QueueSize(Queue F) {
    int count = 0;
    element *temp = F.head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

// Stack operations :
void InitStack(Stack *S){
    *S = NULL;
}

void PUSH(Stack *S, Player P){
    element *N = malloc(sizeof(element));
    N->info = P;
    N->next = *S;
    *S = N;
}

void POP(Stack *S, Player *P){
    *P = (*S)->info;
    element *temp = *S;
    *S = (*S)->next;
    free(temp); 
}

bool EmptyStack(Stack S){
    return(S == NULL);
}

Player TOPStack(Stack S){
    return S->info;
}

// List operations
void InitList(Elist *L) {
    *L = NULL;
}

int ListSize(Elist L) {
    int count = 0;
    element *temp = L;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

// This function generates a random number
int randomNumber(){
    return rand();
}

// This function calculates the sum of the digits of a number
int sumOfDigits(int value){
    int sum = 0;
    while(value != 0){
        sum += value % 10;
        value /= 10;
    }
    return sum;
}

// GCD calculation using Euclidean algorithm
int GCD(int a, int b) {
    a = abs(a);
    b = abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Check if a digit appears in a number
bool digitInNumber(int digit, int number) {
    number = abs(number);
    while (number > 0) {
        if (number % 10 == digit) {
            return true;
        }
        number /= 10;
    }
    return false;
}

// Check if any digit of gcd appears in num1 or num2
bool gcdDigitInNumbers(int gcd, int num1, int num2) {
    while (gcd > 0) {
        int digit = gcd % 10;
        if (digitInNumber(digit, num1) || digitInNumber(digit, num2)) {
            return true;
        }
        gcd /= 10;
    }
    return false;
}

void insertSortedLG(Elist *LG, Player p) {
    element *N = (element*)malloc(sizeof(element));
    if (N == NULL) return;
    
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

        Enqueue(F, p);
    }
}

// Display functions
void displayPlayer(Player p) {
    printf("  ├─ Name: %s (ID: %d, Age: %d)\n", p.name, p.playerID, p.age);
    printf("  ├─ Wins: %d | Losses: %d\n", p.roundWon, p.roundLost);
    printf("  ├─ Total Score: %d | Current Score: %d\n", p.totalScore, p.currentScore);
    printf("  └─ Consecutive W/L: %d/%d\n", p.consecutiveWins, p.consecutiveLosses);
}

void displayQueue(Queue F, const char *queueName) {
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│  %s (Size: %d)                     \n", queueName, QueueSize(F));
    printf("└─────────────────────────────────────┘\n");
    
    if (EmptyQueue(F)) {
        printf("  [Empty Queue]\n");
        return;
    }
    
    element *temp = F.head;
    int count = 1;
    while (temp != NULL) {
        printf("\n[Player %d]\n", count++);
        displayPlayer(temp->info);
        temp = temp->next;
    }
}

void displayList(Elist L, const char *listName) {
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│  %s (Size: %d)                     \n", listName, ListSize(L));
    printf("└─────────────────────────────────────┘\n");
    
    if (L == NULL) {
        printf("  [Empty List]\n");
        return;
    }
    
    element *temp = L;
    int count = 1;
    while (temp != NULL) {
        printf("\n[Player %d]\n", count++);
        displayPlayer(temp->info);
        temp = temp->next;
    }
}

void displayGameState(Queue F, Queue F1, Queue F3, Stack LP, Elist LG) {
    printf("\n╔═════════════════════════════════════════════════════════════╗\n");
    printf("║                    GAME STATE DISPLAY                       ║\n");
    printf("╚═════════════════════════════════════════════════════════════╝\n");
    
    displayQueue(F1, "Queue F1 (Priority 1)");
    displayQueue(F, "Queue F (Normal Priority)");
    displayQueue(F3, "Queue F3 (Priority 3)");
    displayList(LG, "Winners List (LG)");
    displayList(LP, "Losers List (LP)");
    
    printf("\n═══════════════════════════════════════════════════════════════\n\n");
}

// Part I: Play a round with sum of digits strategy
void playRoundPartI(Player *P1, Player *P2, int roundNumber) {
    int generatedValue = 0;
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║              ROUND %d: %s vs %s              \n", roundNumber, P1->name, P2->name);
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    P1->currentScore = 0;
    P2->currentScore = 0;
    
    while (abs(P1->currentScore - P2->currentScore) < 3 && generatedValue < 12) {
        // Player 1 turn
        printf("\n─── %s's Turn ───\n", P1->name);
        printf("Press 'ENTER' to start...\n");
        getchar();

        int random1 = randomNumber();
        printf("Generated value: %d\n", random1);
        
        int sum1 = sumOfDigits(random1);
        printf("Sum of digits: %d", sum1);
        
        if(sum1 % 5 == 0){
            printf(" ✓ Multiple of 5! %s gets +1 point\n", P1->name);
            P1->currentScore++;
        } else {
            printf(" ✗ Not a multiple of 5. No point.\n");
        }
        
        printf("Score: %s %d - %d %s\n", P1->name, P1->currentScore, P2->currentScore, P2->name);
        generatedValue++;
        
        if (abs(P1->currentScore - P2->currentScore) >= 3) {
            break;
        }

        // Player 2 turn
        printf("\n─── %s's Turn ───\n", P2->name);        
        printf("Press 'ENTER' to start...\n");
        getchar();

        int random2 = randomNumber();
        printf("Generated value: %d\n", random2);
        
        int sum2 = sumOfDigits(random2);
        printf("Sum of digits: %d", sum2);
        
        if(sum2 % 5 == 0){
            printf(" ✓ Multiple of 5! %s gets +1 point\n", P2->name);
            P2->currentScore++;
        } else {
            printf(" ✗ Not a multiple of 5. No point.\n");
        }
        
        printf("Score: %s %d - %d %s\n", P1->name, P1->currentScore, P2->currentScore, P2->name);
        generatedValue++;
    }
}

// Part II: Play a round with GCD strategy
void playRoundPartII(Player *P1, Player *P2, int roundNumber) {
    int generatedValue = 0;
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║       ROUND %d (GCD Strategy): %s vs %s       \n", roundNumber, P1->name, P2->name);
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    P1->currentScore = 0;
    P2->currentScore = 0;
    
    while (abs(P1->currentScore - P2->currentScore) < 3 && generatedValue < 16) {
        // Player 1 turn
        printf("\n─── %s's Turn ───\n", P1->name);
        printf("Press 'ENTER' to generate 2 numbers...\n");
        getchar();

        int num1 = randomNumber();
        int num2 = randomNumber();
        printf("Generated: %d and %d\n", num1, num2);
        
        int gcd = GCD(num1, num2);
        printf("GCD(%d, %d) = %d\n", num1, num2, gcd);
        
        if(gcdDigitInNumbers(gcd, num1, num2)){
            printf(" ✓ GCD digit found in numbers! %s gets +1 point\n", P1->name);
            P1->currentScore++;
        } else {
            printf(" ✗ No GCD digit in numbers. No point.\n");
        }
        
        printf("Score: %s %d - %d %s\n", P1->name, P1->currentScore, P2->currentScore, P2->name);
        generatedValue += 2;
        
        if (abs(P1->currentScore - P2->currentScore) >= 3) {
            break;
        }

        // Player 2 turn
        printf("\n─── %s's Turn ───\n", P2->name);        
        printf("Press 'ENTER' to generate 2 numbers...\n");
        getchar();

        int num3 = randomNumber();
        int num4 = randomNumber();
        printf("Generated: %d and %d\n", num3, num4);
        
        int gcd2 = GCD(num3, num4);
        printf("GCD(%d, %d) = %d\n", num3, num4, gcd2);
        
        if(gcdDigitInNumbers(gcd2, num3, num4)){
            printf(" ✓ GCD digit found in numbers! %s gets +1 point\n", P2->name);
            P2->currentScore++;
        } else {
            printf(" ✗ No GCD digit in numbers. No point.\n");
        }
        
        printf("Score: %s %d - %d %s\n", P1->name, P1->currentScore, P2->currentScore, P2->name);
        generatedValue += 2;
    }
}

// Update statistics after Part I round
void updateStatisticsPartI(Player *P1, Player *P2) {
    printf("\n═══════════════════════════════════════\n");
    printf("          ROUND FINISHED             \n");
    printf("═══════════════════════════════════════\n");
    
    if (P1->currentScore > P2->currentScore) {
        P1->roundWon++;
        P1->consecutiveWins++;
        P1->consecutiveLosses = 0;
        P1->totalScore += P1->currentScore;

        P2->roundLost++;
        P2->consecutiveLosses++;
        P2->consecutiveWins = 0;
        P2->totalScore += P2->currentScore;
        
        printf("🏆 WINNER: %s (ID: %d)\n", P1->name, P1->playerID);
        printf("Score: %d - %d\n", P1->currentScore, P2->currentScore);
        
    } else if (P2->currentScore > P1->currentScore) {
        P2->roundWon++;
        P2->consecutiveWins++;
        P2->consecutiveLosses = 0;
        P2->totalScore += P2->currentScore;

        P1->roundLost++;
        P1->consecutiveLosses++;
        P1->consecutiveWins = 0;
        P1->totalScore += P1->currentScore;
        
        printf("🏆 WINNER: %s (ID: %d)\n", P2->name, P2->playerID);
        printf("Score: %d - %d\n", P2->currentScore, P1->currentScore);
        
    } else {
        P1->consecutiveWins = 0;
        P1->consecutiveLosses = 0;
        P2->consecutiveWins = 0;
        P2->consecutiveLosses = 0;

        P1->totalScore += P1->currentScore;
        P2->totalScore += P2->currentScore;
        printf("🤝 DRAW!\n");
        printf("Score: %d - %d\n", P1->currentScore, P2->currentScore);
    }
}

// Update statistics after Part II round
void updateStatisticsPartII(Player *P1, Player *P2) {
    printf("\n═══════════════════════════════════════\n");
    printf("          ROUND FINISHED             \n");
    printf("═══════════════════════════════════════\n");
    
    if (P1->currentScore > P2->currentScore) {
        P1->roundWon++;
        P1->consecutiveWins++;
        P1->consecutiveLosses = 0;
        P1->totalScore += P1->currentScore;

        P2->roundLost++;
        P2->consecutiveLosses++;
        P2->consecutiveWins = 0;
        P2->totalScore += P2->currentScore;
        
        printf("🏆 WINNER: %s (ID: %d)\n", P1->name, P1->playerID);
        printf("Score: %d - %d\n", P1->currentScore, P2->currentScore);
        
    } else if (P2->currentScore > P1->currentScore) {
        P2->roundWon++;
        P2->consecutiveWins++;
        P2->consecutiveLosses = 0;
        P2->totalScore += P2->currentScore;

        P1->roundLost++;
        P1->consecutiveLosses++;
        P1->consecutiveWins = 0;
        P1->totalScore += P1->currentScore;
        
        printf("🏆 WINNER: %s (ID: %d)\n", P2->name, P2->playerID);
        printf("Score: %d - %d\n", P2->currentScore, P1->currentScore);
        
    } else {
        P1->consecutiveWins = 0;
        P1->consecutiveLosses = 0;
        P2->consecutiveWins = 0;
        P2->consecutiveLosses = 0;

        P1->totalScore += P1->currentScore;
        P2->totalScore += P2->currentScore;
        printf("🤝 DRAW!\n");
        printf("Score: %d - %d\n", P1->currentScore, P2->currentScore);
    }
}

// Classify player for Part I
void classifyPlayerPartI(Player *P, Queue *F, Queue *F1, Queue *F3, Stack *LP, Elist *LG) {
    if (P->roundLost >= 5) {
        PUSH(LP, *P);        
        printf("  → %s eliminated (5 losses) → LP\n", P->name);
        return;
    }
    
    if (P->roundWon >= 5) {
        insertSortedLG(LG, *P); 
        printf("  → %s wins the game (5 wins) → LG\n", P->name);
        return;
    }
    
    if (P->consecutiveWins >= 3) {
        Enqueue(F1, *P);
        printf("  → %s (3 consecutive wins) → F1\n", P->name);
        return;
    }
    
    if (P->roundLost >= 3) {
        Enqueue(F3, *P);
        printf("  → %s (3 losses) → F3\n", P->name);
        return;
    }

    Enqueue(F, *P);
    printf("  → %s → F\n", P->name);
}

// Classify player for Part II
void classifyPlayerPartII(Player *P, Queue *F, Queue *F3, Stack *LP, Elist *LG) {
    if (P->roundLost >= 2) {
        PUSH(LP, *P);        
        printf("  → %s eliminated (2 losses) → LP\n", P->name);
        return;
    }
    
    if (P->consecutiveWins >= 2) {
        insertSortedLG(LG, *P); 
        printf("  → %s wins (2 consecutive wins) → LG\n", P->name);
        return;
    }
    
    if (P->currentScore < ((P->currentScore > 0) ? P->currentScore : 1)) {
        Enqueue(F3, *P);
        printf("  → %s → F3\n", P->name);
        return;
    }

    Enqueue(F, *P);
    printf("  → %s → F\n", P->name);
}

// Get next player from queues
bool GetNextPlayer(Queue *F1, Queue *F, Queue *F3, Player *p, Stack *LP) {
    if (!EmptyQueue(*F1)) {
        Dequeue(F1, p);
        return true;
    } else if (!EmptyQueue(*F)) {
        Dequeue(F, p);
        return true;
    } else if (!EmptyQueue(*F3)) {
        if (QueueSize(*F3) == 1) { 
            Player last;
            Dequeue(F3, &last);
            PUSH(LP, last); 
            printf("Last player in F3 eliminated: %s\n", last.name);
            return false;
        } else {
            Dequeue(F3, p);
            return true;
        }
    }
    return false;
}

// Query functions
void showPlayersWithNoWins(Elist LG, Stack LP) {
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│  Players with 0 wins:                \n");
    printf("└─────────────────────────────────────┘\n");
    
    bool found = false;
    element *temp = LP;
    while (temp != NULL) {
        if (temp->info.roundWon == 0) {
            printf("  • %s (ID: %d) - Total Score: %d\n", 
                   temp->info.name, temp->info.playerID, temp->info.totalScore);
            found = true;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("  [No players found]\n");
    }
}

void showPlayersByWins(Elist LG, Stack LP, int wins) {
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│  Players with %d win(s):              \n", wins);
    printf("└─────────────────────────────────────┘\n");
    
    bool found = false;
    
    element *temp = LG;
    while (temp != NULL) {
        if (temp->info.roundWon == wins) {
            printf("  • %s (ID: %d) - Total Score: %d\n", 
                   temp->info.name, temp->info.playerID, temp->info.totalScore);
            found = true;
        }
        temp = temp->next;
    }
    
    temp = LP;
    while (temp != NULL) {
        if (temp->info.roundWon == wins) {
            printf("  • %s (ID: %d) - Total Score: %d\n", 
                   temp->info.name, temp->info.playerID, temp->info.totalScore);
            found = true;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("  [No players found]\n");
    }
}

void showPlayersByLosses(Elist LG, Stack LP, int losses) {
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│  Players with %d loss(es):            \n", losses);
    printf("└─────────────────────────────────────┘\n");
    
    bool found = false;
    
    element *temp = LG;
    while (temp != NULL) {
        if (temp->info.roundLost == losses) {
            printf("  • %s (ID: %d) - Total Score: %d\n", 
                   temp->info.name, temp->info.playerID, temp->info.totalScore);
            found = true;
        }
        temp = temp->next;
    }
    
    temp = LP;
    while (temp != NULL) {
        if (temp->info.roundLost == losses) {
            printf("  • %s (ID: %d) - Total Score: %d\n", 
                   temp->info.name, temp->info.playerID, temp->info.totalScore);
            found = true;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("  [No players found]\n");
    }
}

void showTop3Winners(Elist LG) {
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                    TOP 3 WINNERS                          ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    element *temp = LG;
    int rank = 1;
    
    while (temp != NULL && rank <= 3) {
        printf("\n🏆 Rank %d:\n", rank);
        displayPlayer(temp->info);
        temp = temp->next;
        rank++;
    }
    
    if (rank == 1) {
        printf("  [No winners yet]\n");
    }
}

// Main function
int main() {
    srand(time(NULL));
    
    Queue F, F1, F3;
    Stack LP;
    Elist LG;
    
    Initqueue(&F);
    Initqueue(&F1);
    Initqueue(&F3);
    InitStack(&LP);
    InitList(&LG);
    
    int n;
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║          WELCOME TO THE RANDOM NUMBER GAME!              ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
    
    printf("Enter the number of players: ");
    scanf("%d", &n);
    getchar(); // Clear newline
    
    time_t startTime = time(NULL);
    printf("\nGame started at: %s", ctime(&startTime));
    
    AutoFill(&F, n);
    printf("\n✓ %d players have been registered!\n", n);
    
    int roundNumber = 0;
    int maxRoundsPartI = 3 * n;
    
    // PART I: Sum of Digits Strategy
    printf("\n\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                    PART I: SUM OF DIGITS                  ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    while (roundNumber < maxRoundsPartI) {
        if (EmptyQueue(F) && EmptyQueue(F1) && EmptyQueue(F3)) {
            printf("\nAll players classified. Moving to final checks...\n");
            break;
        }
        
        Player P1, P2;
        
        if (!GetNextPlayer(&F1, &F, &F3, &P1, &LP)) {
            break;
        }
        
        if (!GetNextPlayer(&F1, &F, &F3, &P2, &LP)) {
            classifyPlayerPartI(&P1, &F, &F1, &F3, &LP, &LG);
            break;
        }
        
        roundNumber++;
        playRoundPartI(&P1, &P2, roundNumber);
        updateStatisticsPartI(&P1, &P2);
        
        printf("\nClassifying players:\n");
        
        if (P1.currentScore > P2.currentScore) {
            classifyPlayerPartI(&P1, &F, &F1, &F3, &LP, &LG);
            classifyPlayerPartI(&P2, &F, &F1, &F3, &LP, &LG);
        } else if (P2.currentScore > P1.currentScore) {
            classifyPlayerPartI(&P2, &F, &F1, &F3, &LP, &LG);
            classifyPlayerPartI(&P1, &F, &F1, &F3, &LP, &LG);
        } else {
            Enqueue(&F, P1);
            Enqueue(&F, P2);
            printf("  → Both players (draw) → F\n");
        }
        
        displayGameState(F, F1, F3, LP, LG);
        
        printf("\nPress ENTER to continue to next round...");
        getchar();
    }
    
    // PART II: GCD Strategy (if needed)
    if (!EmptyQueue(F) || !EmptyQueue(F1) || !EmptyQueue(F3)) {
        printf("\n\n╔═══════════════════════════════════════════════════════════╗\n");
        printf("║            STRATEGY CHANGE: GCD STRATEGY                  ║\n");
        printf("╚═══════════════════════════════════════════════════════════╝\n");
        printf("After %d rounds, players remain. Switching to GCD strategy!\n", roundNumber);
        
        int maxRoundsPartII = 2 * n;
        int partIIRounds = 0;
        
        // Reset consecutive wins/losses for Part II
        element *temp = F.head;
        while (temp != NULL) {
            temp->info.consecutiveWins = 0;
            temp->info.consecutiveLosses = 0;
            temp = temp->next;
        }
        
        // Move F1 players to F for Part II
        while (!EmptyQueue(F1)) {
            Player p;
            Dequeue(&F1, &p);
            p.consecutiveWins = 0;
            p.consecutiveLosses = 0;
            Enqueue(&F, p);
        }
        
        while (partIIRounds < maxRoundsPartII) {
            if (EmptyQueue(F) && EmptyQueue(F3)) {
                printf("\nAll players classified in Part II.\n");
                break;
            }
            
            Player P1, P2;
            
            if (!GetNextPlayer(&F1, &F, &F3, &P1, &LP)) {
                break;
            }
            
            if (!GetNextPlayer(&F1, &F, &F3, &P2, &LP)) {
                classifyPlayerPartII(&P1, &F, &F3, &LP, &LG);
                break;
            }
            
            roundNumber++;
            partIIRounds++;
            playRoundPartII(&P1, &P2, roundNumber);
            updateStatisticsPartII(&P1, &P2);
            
            printf("\nClassifying players:\n");
            
            if (P1.currentScore > P2.currentScore) {
                classifyPlayerPartII(&P1, &F, &F3, &LP, &LG);
                classifyPlayerPartII(&P2, &F, &F3, &LP, &LG);
            } else if (P2.currentScore > P1.currentScore) {
                classifyPlayerPartII(&P2, &F, &F3, &LP, &LG);
                classifyPlayerPartII(&P1, &F, &F3, &LP, &LG);
            } else {
                Enqueue(&F, P1);
                Enqueue(&F, P2);
                printf("  → Both players (draw) → F\n");
            }
            
            displayGameState(F, F1, F3, LP, LG);
            
            printf("\nPress ENTER to continue to next round...");
            getchar();
        }
        
        // Final classification if time limit reached
        if (!EmptyQueue(F) || !EmptyQueue(F3)) {
            printf("\n⏰ Time limit reached! Final classification...\n");
            
            while (!EmptyQueue(F)) {
                Player p;
                Dequeue(&F, &p);
                PUSH(&LP, p);
            }
            
            while (!EmptyQueue(F3)) {
                Player p;
                Dequeue(&F3, &p);
                PUSH(&LP, p);
            }
        }
    }
    
    time_t endTime = time(NULL);
    printf("\n\nGame ended at: %s", ctime(&endTime));
    printf("Total duration: %.0f seconds\n", difftime(endTime, startTime));
    
    // Final results
    printf("\n\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                    FINAL RESULTS                          ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    displayGameState(F, F1, F3, LP, LG);
    showTop3Winners(LG);
    
    // Queries
    printf("\n\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                    STATISTICS                             ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    showPlayersWithNoWins(LG, LP);
    
    printf("\n--- Part I Strategy Statistics ---\n");
    showPlayersByWins(LG, LP, 1);
    showPlayersByWins(LG, LP, 2);
    showPlayersByWins(LG, LP, 3);
    
    showPlayersByLosses(LG, LP, 1);
    showPlayersByLosses(LG, LP, 2);
    showPlayersByLosses(LG, LP, 3);
    
    printf("\n\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                 THANK YOU FOR PLAYING!                    ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    return 0;
}