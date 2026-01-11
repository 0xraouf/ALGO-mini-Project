#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>

//==============================================================================
// DATA STRUCTURES
//==============================================================================

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

typedef struct element {
    Player info;
    struct element *next; 
} element;

typedef struct Queue {
    element *head;
    element *tail;
    int size;  // Cache size for O(1) access
} Queue;

typedef element *Stack;
typedef element *Elist;

typedef struct GameStats {
    int totalRounds;
    int partIRounds;
    int partIIRounds;
    time_t startTime;
    time_t endTime;
} GameStats;

//==============================================================================
// QUEUE OPERATIONS - O(1) for all operations
//==============================================================================

void Initqueue(Queue *F) {
    F->head = NULL;
    F->tail = NULL;
    F->size = 0;
}

bool EmptyQueue(Queue F) {
    return (F.head == NULL);
}

int QueueSize(Queue F) {
    return F.size;  // O(1) instead of O(n)
}

void Enqueue(Queue *F, Player p) {
    element *N = (element*)malloc(sizeof(element));
    if (N == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
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
    F->size++;
}

bool Dequeue(Queue *F, Player *p) {
    if (EmptyQueue(*F)) return false;
    
    *p = F->head->info;
    element *temp = F->head;
    F->head = F->head->next;
    free(temp);
    F->size--;
    
    if (F->head == NULL) {
        F->tail = NULL;
    }
    return true;
}

void ClearQueue(Queue *F) {
    Player temp;
    while (!EmptyQueue(*F)) {
        Dequeue(F, &temp);
    }
}

//==============================================================================
// STACK OPERATIONS - O(1) for all operations
//==============================================================================

void InitStack(Stack *S) {
    *S = NULL;
}

bool EmptyStack(Stack S) {
    return (S == NULL);
}

void PUSH(Stack *S, Player P) {
    element *N = (element*)malloc(sizeof(element));
    if (N == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return;
    }
    N->info = P;
    N->next = *S;
    *S = N;
}

bool POP(Stack *S, Player *P) {
    if (EmptyStack(*S)) return false;
    
    *P = (*S)->info;
    element *temp = *S;
    *S = (*S)->next;
    free(temp);
    return true;
}

Player TOPStack(Stack S) {
    return S->info;
}

int StackSize(Stack S) {
    int count = 0;
    element *temp = S;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

void ClearStack(Stack *S) {
    Player temp;
    while (!EmptyStack(*S)) {
        POP(S, &temp);
    }
}

//==============================================================================
// LIST OPERATIONS
//==============================================================================

void InitList(Elist *L) {
    *L = NULL;
}

bool EmptyList(Elist L) {
    return (L == NULL);
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

// Insert in descending order by totalScore - O(n) worst case
void insertSortedLG(Elist *LG, Player p) {
    element *N = (element*)malloc(sizeof(element));
    if (N == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return;
    }
    
    N->info = p;
    N->next = NULL;

    // Insert at head if list is empty or new player has highest score
    if (*LG == NULL || p.totalScore > (*LG)->info.totalScore) {
        N->next = *LG;
        *LG = N;
    } else {
        element *current = *LG;
        // Find insertion point
        while (current->next != NULL && current->next->info.totalScore >= p.totalScore) {
            current = current->next;
        }
        N->next = current->next;
        current->next = N;
    }
}

void ClearList(Elist *L) {
    element *temp;
    while (*L != NULL) {
        temp = *L;
        *L = (*L)->next;
        free(temp);
    }
}

//==============================================================================
// MATHEMATICAL HELPER FUNCTIONS
//==============================================================================

// Generate random number - O(1)
int randomNumber() {
    return rand() % 1000000;  // Limited range for better readability
}

// Calculate sum of digits - O(log n) where n is the number
int sumOfDigits(int value) {
    int sum = 0;
    value = abs(value);
    while (value != 0) {
        sum += value % 10;
        value /= 10;
    }
    return sum;
}

// GCD using Euclidean algorithm - O(log min(a,b))
int GCD(int a, int b) {
    a = abs(a);
    b = abs(b);
    if (b == 0) return a;
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Check if digit appears in number - O(log n)
bool digitInNumber(int digit, int number) {
    if (number == 0 && digit == 0) return true;
    number = abs(number);
    while (number > 0) {
        if (number % 10 == digit) {
            return true;
        }
        number /= 10;
    }
    return false;
}

// Check if any digit of gcd appears in num1 or num2 - O(log gcd)
bool gcdDigitInNumbers(int gcd, int num1, int num2) {
    if (gcd == 0) return digitInNumber(0, num1) || digitInNumber(0, num2);
    
    int original_gcd = gcd;
    while (gcd > 0) {
        int digit = gcd % 10;
        if (digitInNumber(digit, num1) || digitInNumber(digit, num2)) {
            return true;
        }
        gcd /= 10;
    }
    return false;
}

//==============================================================================
// PLAYER INITIALIZATION
//==============================================================================

void AutoFill(Queue *F, int n) {
    printf("\n⏳ Generating %d players...\n", n);
    for (int i = 0; i < n; i++) {
        Player p;
        
        sprintf(p.name, "Player_%02d", i + 1); 
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
    printf("✓ Successfully registered %d players!\n", n);
}

//==============================================================================
// DISPLAY FUNCTIONS
//==============================================================================

void printSeparator(char c, int length) {
    for (int i = 0; i < length; i++) putchar(c);
    putchar('\n');
}

void displayPlayer(Player p, bool detailed) {
    if (detailed) {
        printf("  ┌─ %s (ID: %d) ─┐\n", p.name, p.playerID);
        printf("  │ Age: %d                    \n", p.age);
        printf("  │ Wins: %d | Losses: %d      \n", p.roundWon, p.roundLost);
        printf("  │ Total Score: %d            \n", p.totalScore);
        printf("  │ Consecutive W/L: %d/%d     \n", p.consecutiveWins, p.consecutiveLosses);
        printf("  └────────────────────────────┘\n");
    } else {
        printf("  • %s (ID: %d, Score: %d, W/L: %d/%d)\n", 
               p.name, p.playerID, p.totalScore, p.roundWon, p.roundLost);
    }
}

void displayQueue(Queue F, const char *queueName) {
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║  %-43s Size: %2d  ║\n", queueName, F.size);
    printf("╚══════════════════════════════════════════════════════╝\n");
    
    if (EmptyQueue(F)) {
        printf("  [Empty]\n");
        return;
    }
    
    element *temp = F.head;
    int count = 1;
    while (temp != NULL) {
        printf("[%d] ", count++);
        displayPlayer(temp->info, false);
        temp = temp->next;
    }
}

void displayList(Elist L, const char *listName) {
    int size = ListSize(L);
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║  %-43s Size: %2d  ║\n", listName, size);
    printf("╚══════════════════════════════════════════════════════╝\n");
    
    if (L == NULL) {
        printf("  [Empty]\n");
        return;
    }
    
    element *temp = L;
    int count = 1;
    while (temp != NULL) {
        printf("[%d] ", count++);
        displayPlayer(temp->info, false);
        temp = temp->next;
    }
}

void displayGameState(Queue F, Queue F1, Queue F3, Stack LP, Elist LG) {
    printf("\n");
    printSeparator('═', 70);
    printf("                        CURRENT GAME STATE\n");
    printSeparator('═', 70);
    
    displayQueue(F1, "Queue F1 (Priority 1 - Hot Streak)");
    displayQueue(F, "Queue F (Normal Priority)");
    displayQueue(F3, "Queue F3 (Priority 3 - Struggling)");
    displayList(LG, "Winners List (LG) 🏆");
    displayList(LP, "Losers List (LP) ❌");
    
    int totalActive = F.size + F1.size + F3.size;
    printf("\n📊 Active Players: %d | Winners: %d | Losers: %d\n", 
           totalActive, ListSize(LG), StackSize(LP));
    printSeparator('═', 70);
}

void displayCompactState(Queue F, Queue F1, Queue F3, Stack LP, Elist LG) {
    printf("\n📊 State: F1:%d | F:%d | F3:%d | LG:%d | LP:%d\n", 
           F1.size, F.size, F3.size, ListSize(LG), StackSize(LP));
}

//==============================================================================
// GAMEPLAY - PART I (Sum of Digits Strategy)
//==============================================================================

void playRoundPartI(Player *P1, Player *P2, int roundNumber, bool autoPlay) {
    printf("\n");
    printSeparator('═', 70);
    printf("  ROUND %d - PART I: %s vs %s\n", roundNumber, P1->name, P2->name);
    printSeparator('═', 70);
    
    P1->currentScore = 0;
    P2->currentScore = 0;
    int turn = 0;
    
    while (abs(P1->currentScore - P2->currentScore) < 3 && turn < 12) {
        Player *current = (turn % 2 == 0) ? P1 : P2;
        
        printf("\n─── Turn %d: %s ───\n", turn + 1, current->name);
        
        if (!autoPlay) {
            printf("Press ENTER to generate number...");
            getchar();
        }

        int random = randomNumber();
        int sum = sumOfDigits(random);
        bool scored = (sum % 5 == 0);
        
        printf("  Generated: %d → Sum of digits: %d ", random, sum);
        
        if (scored) {
            printf("✓ (Multiple of 5)\n");
            current->currentScore++;
            printf("  %s scores! (+1 point)\n", current->name);
        } else {
            printf("✗ (Not multiple of 5)\n");
            printf("  No point for %s\n", current->name);
        }
        
        printf("  Score: %s %d - %d %s\n", P1->name, P1->currentScore, P2->currentScore, P2->name);
        
        turn++;
        
        if (abs(P1->currentScore - P2->currentScore) >= 3) {
            break;
        }
    }
}

//==============================================================================
// GAMEPLAY - PART II (GCD Strategy)
//==============================================================================

void playRoundPartII(Player *P1, Player *P2, int roundNumber, bool autoPlay) {
    printf("\n");
    printSeparator('═', 70);
    printf("  ROUND %d - PART II (GCD): %s vs %s\n", roundNumber, P1->name, P2->name);
    printSeparator('═', 70);
    
    P1->currentScore = 0;
    P2->currentScore = 0;
    int turn = 0;
    
    while (abs(P1->currentScore - P2->currentScore) < 3 && turn < 8) { // 8 turns = 16 numbers
        Player *current = (turn % 2 == 0) ? P1 : P2;
        
        printf("\n─── Turn %d: %s ───\n", turn + 1, current->name);
        
        if (!autoPlay) {
            printf("Press ENTER to generate 2 numbers...");
            getchar();
        }

        int num1 = randomNumber();
        int num2 = randomNumber();
        int gcd = GCD(num1, num2);
        bool scored = gcdDigitInNumbers(gcd, num1, num2);
        
        printf("  Generated: %d and %d\n", num1, num2);
        printf("  GCD(%d, %d) = %d ", num1, num2, gcd);
        
        if (scored) {
            printf("✓ (Digit match found)\n");
            current->currentScore++;
            printf("  %s scores! (+1 point)\n", current->name);
        } else {
            printf("✗ (No digit match)\n");
            printf("  No point for %s\n", current->name);
        }
        
        printf("  Score: %s %d - %d %s\n", P1->name, P1->currentScore, P2->currentScore, P2->name);
        
        turn++;
        
        if (abs(P1->currentScore - P2->currentScore) >= 3) {
            break;
        }
    }
}

//==============================================================================
// PLAYER STATISTICS UPDATE
//==============================================================================

void updateStatistics(Player *P1, Player *P2) {
    printf("\n");
    printSeparator('─', 70);
    
    if (P1->currentScore > P2->currentScore) {
        P1->roundWon++;
        P1->consecutiveWins++;
        P1->consecutiveLosses = 0;
        P1->totalScore += P1->currentScore;

        P2->roundLost++;
        P2->consecutiveLosses++;
        P2->consecutiveWins = 0;
        P2->totalScore += P2->currentScore;
        
        printf("  🏆 WINNER: %s\n", P1->name);
        printf("  Final Score: %d - %d\n", P1->currentScore, P2->currentScore);
        
    } else if (P2->currentScore > P1->currentScore) {
        P2->roundWon++;
        P2->consecutiveWins++;
        P2->consecutiveLosses = 0;
        P2->totalScore += P2->currentScore;

        P1->roundLost++;
        P1->consecutiveLosses++;
        P1->consecutiveWins = 0;
        P1->totalScore += P1->currentScore;
        
        printf("  🏆 WINNER: %s\n", P2->name);
        printf("  Final Score: %d - %d\n", P2->currentScore, P1->currentScore);
        
    } else {
        P1->consecutiveWins = 0;
        P1->consecutiveLosses = 0;
        P2->consecutiveWins = 0;
        P2->consecutiveLosses = 0;

        P1->totalScore += P1->currentScore;
        P2->totalScore += P2->currentScore;
        
        printf("  🤝 DRAW!\n");
        printf("  Final Score: %d - %d\n", P1->currentScore, P2->currentScore);
    }
    printSeparator('─', 70);
}

//==============================================================================
// PLAYER CLASSIFICATION
//==============================================================================

void classifyPlayerPartI(Player *P, Queue *F, Queue *F1, Queue *F3, Stack *LP, Elist *LG) {
    // Check elimination (5 losses)
    if (P->roundLost >= 5) {
        PUSH(LP, *P);        
        printf("    ❌ %s → LP (5 losses - ELIMINATED)\n", P->name);
        return;
    }
    
    // Check victory (5 wins)
    if (P->roundWon >= 5) {
        insertSortedLG(LG, *P); 
        printf("    🏆 %s → LG (5 wins - CHAMPION!)\n", P->name);
        return;
    }
    
    // Priority queue F1 (3 consecutive wins)
    if (P->consecutiveWins >= 3) {
        Enqueue(F1, *P);
        printf("    ⚡ %s → F1 (3 consecutive wins - HOT STREAK!)\n", P->name);
        return;
    }
    
    // Low priority queue F3 (3 total losses)
    if (P->roundLost >= 3) {
        Enqueue(F3, *P);
        printf("    ⚠️  %s → F3 (3 losses - STRUGGLING)\n", P->name);
        return;
    }

    // Normal queue F
    Enqueue(F, *P);
    printf("    ➡️  %s → F (Normal priority)\n", P->name);
}

void classifyPlayerPartII(Player *P, Queue *F, Queue *F3, Stack *LP, Elist *LG, bool winner) {
    // Check elimination (2 losses)
    if (P->roundLost >= 2) {
        PUSH(LP, *P);        
        printf("    ❌ %s → LP (2 losses - ELIMINATED)\n", P->name);
        return;
    }
    
    // Check victory (2 consecutive wins)
    if (P->consecutiveWins >= 2) {
        insertSortedLG(LG, *P); 
        printf("    🏆 %s → LG (2 consecutive wins - WINNER!)\n", P->name);
        return;
    }
    
    // Winner goes to F, loser goes to F3
    if (winner) {
        Enqueue(F, *P);
        printf("    ➡️  %s → F (Won this round)\n", P->name);
    } else {
        Enqueue(F3, *P);
        printf("    ⚠️  %s → F3 (Lost this round)\n", P->name);
    }
}

//==============================================================================
// PLAYER SELECTION
//==============================================================================

bool GetNextPlayer(Queue *F1, Queue *F, Queue *F3, Player *p, Stack *LP) {
    // Priority 1: F1
    if (!EmptyQueue(*F1)) {
        Dequeue(F1, p);
        return true;
    }
    
    // Priority 2: F
    if (!EmptyQueue(*F)) {
        Dequeue(F, p);
        return true;
    }
    
    // Priority 3: F3
    if (!EmptyQueue(*F3)) {
        // Special case: last player in F3
        if (QueueSize(*F3) == 1) { 
            Player last;
            Dequeue(F3, &last);
            PUSH(LP, last); 
            printf("\n⚠️  Last player in F3 eliminated: %s\n", last.name);
            return false;
        }
        Dequeue(F3, p);
        return true;
    }
    
    return false;
}

//==============================================================================
// QUERY FUNCTIONS
//==============================================================================

void showTop3Winners(Elist LG) {
    printf("\n");
    printSeparator('═', 70);
    printf("                         🏆 TOP 3 WINNERS 🏆\n");
    printSeparator('═', 70);
    
    element *temp = LG;
    int rank = 1;
    
    const char *medals[] = {"🥇", "🥈", "🥉"};
    
    while (temp != NULL && rank <= 3) {
        printf("\n%s RANK %d:\n", medals[rank - 1], rank);
        displayPlayer(temp->info, true);
        temp = temp->next;
        rank++;
    }
    
    if (rank == 1) {
        printf("\n  No winners yet.\n");
    }
    printSeparator('═', 70);
}

void showPlayersByWinsLosses(Elist LG, Stack LP, int wins, int losses, const char *description) {
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║  %-50s  ║\n", description);
    printf("╚══════════════════════════════════════════════════════╝\n");
    
    bool found = false;
    
    // Check winners list
    element *temp = LG;
    while (temp != NULL) {
        if ((wins == -1 || temp->info.roundWon == wins) && 
            (losses == -1 || temp->info.roundLost == losses)) {
            displayPlayer(temp->info, false);
            found = true;
        }
        temp = temp->next;
    }
    
    // Check losers list
    temp = LP;
    while (temp != NULL) {
        if ((wins == -1 || temp->info.roundWon == wins) && 
            (losses == -1 || temp->info.roundLost == losses)) {
            displayPlayer(temp->info, false);
            found = true;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("  [No players found]\n");
    }
}

void displayStatistics(Elist LG, Stack LP, GameStats stats) {
    printf("\n");
    printSeparator('═', 70);
    printf("                      📊 GAME STATISTICS 📊\n");
    printSeparator('═', 70);
    
    printf("\n⏱️  TIMING:\n");
    printf("  Start: %s", ctime(&stats.startTime));
    printf("  End:   %s", ctime(&stats.endTime));
    printf("  Duration: %.0f seconds (%.1f minutes)\n", 
           difftime(stats.endTime, stats.startTime),
           difftime(stats.endTime, stats.startTime) / 60.0);
    
    printf("\n🎮 ROUNDS:\n");
    printf("  Total Rounds: %d\n", stats.totalRounds);
    printf("  Part I Rounds: %d\n", stats.partIRounds);
    printf("  Part II Rounds: %d\n", stats.partIIRounds);
    
    printf("\n👥 FINAL COUNTS:\n");
    printf("  Winners (LG): %d\n", ListSize(LG));
    printf("  Losers (LP): %d\n", StackSize(LP));
    
    printSeparator('═', 70);
    
    // Detailed queries
    showPlayersByWinsLosses(LG, LP, 0, -1, "Players with 0 wins");
    
    printf("\n");
    printSeparator('─', 70);
    printf("  PART I STRATEGY (Sum of Digits)\n");
    printSeparator('─', 70);
    showPlayersByWinsLosses(LG, LP, 1, -1, "Players with 1 win");
    showPlayersByWinsLosses(LG, LP, 2, -1, "Players with 2 wins");
    showPlayersByWinsLosses(LG, LP, 3, -1, "Players with 3 wins");
    showPlayersByWinsLosses(LG, LP, -1, 1, "Players with 1 loss");
    showPlayersByWinsLosses(LG, LP, -1, 2, "Players with 2 losses");
    showPlayersByWinsLosses(LG, LP, -1, 3, "Players with 3 losses");
}

//==============================================================================
// MAIN GAME LOOP
//==============================================================================

int main() {
    srand(time(NULL));
    
    // Initialize data structures
    Queue F, F1, F3;
    Stack LP;
    Elist LG;
    GameStats stats = {0, 0, 0};
    
    Initqueue(&F);
    Initqueue(&F1);
    Initqueue(&F3);
    InitStack(&LP);
    InitList(&LG);
    
    // Welcome screen
    printf("\n");
    printSeparator('═', 70);
    printf("              🎮 RANDOM NUMBER TOURNAMENT GAME 🎮\n");
    printSeparator('═', 70);
    
    int n;
    printf("\nEnter number of players (recommended 5-20): ");
    scanf("%d", &n);
    getchar();
    
    if (n < 2) {
        printf("❌ Need at least 2 players!\n");
        return 1;
    }
    
    char autoPlayChoice;
    bool autoPlay = false;
    printf("\nAuto-play mode (no pauses)? (y/n): ");
    scanf("%c", &autoPlayChoice);
    getchar();
    autoPlay = (autoPlayChoice == 'y' || autoPlayChoice == 'Y');
    
    stats.startTime = time(NULL);
    printf("\n⏰ Game started at: %s", ctime(&stats.startTime));
    
    AutoFill(&F, n);
    
    int maxRoundsPartI = 3 * n;
    
    //==========================================================================
    // PART I: Sum of Digits Strategy
    //==========================================================================
    
    printf("\n");
    printSeparator('═', 70);
    printf("              🎯 PART I: SUM OF DIGITS STRATEGY 🎯\n");
    printSeparator('═', 70);
    printf("\nRules:\n");
    printf("  • If sum of digits is multiple of 5 → Score +1\n");
    printf("  • First to lead by 3 points wins\n");
    printf("  • Max 12 numbers per round\n");
    printf("  • 5 wins → Victory | 5 losses → Elimination\n");
    printf("  • 3 consecutive wins → Priority Queue F1\n");
    printf("  • 3 losses → Low Priority Queue F3\n");
    printSeparator('═', 70);
    
    if (!autoPlay) {
        printf("\nPress ENTER to begin Part I...");
        getchar();
    }
    
    while (stats.totalRounds < maxRoundsPartI) {
        // Check if game can continue
        if (EmptyQueue(F) && EmptyQueue(F1) && EmptyQueue(F3)) {
            printf("\n✓ All players classified in Part I!\n");
            break;
        }
        
        Player P1, P2;
        
        // Get two players
        if (!GetNextPlayer(&F1, &F, &F3, &P1, &LP)) {
            printf("\n⚠️  No players available for next round.\n");
            break;
        }
        
        if (!GetNextPlayer(&F1, &F, &F3, &P2, &LP)) {
            printf("\n⚠️  Only one player left. Auto-classifying...\n");
            classifyPlayerPartI(&P1, &F, &F1, &F3, &LP, &LG);
            break;
        }
        
        // Play round
        stats.totalRounds++;
        stats.partIRounds++;
        playRoundPartI(&P1, &P2, stats.totalRounds, autoPlay);
        updateStatistics(&P1, &P2);
        
        // Classify players
        printf("\n  Classifying players:\n");
        
        if (P1.currentScore > P2.currentScore) {
            classifyPlayerPartI(&P1, &F, &F1, &F3, &LP, &LG);
            classifyPlayerPartI(&P2, &F, &F1, &F3, &LP, &LG);
        } else if (P2.currentScore > P1.currentScore) {
            classifyPlayerPartI(&P2, &F, &F1, &F3, &LP, &LG);
            classifyPlayerPartI(&P1, &F, &F1, &F3, &LP, &LG);
        } else {
            // Draw: both go back to F
            Enqueue(&F, P1);
            Enqueue(&F, P2);
            printf("    🤝 Both players (draw) → F\n");
        }
        
        // Display state
        if (autoPlay) {
            displayCompactState(F, F1, F3, LP, LG);
        } else {
            displayGameState(F, F1, F3, LP, LG);
            printf("\nPress ENTER for next round...");
            getchar();
        }
    }
    
    //==========================================================================
    // PART II: GCD Strategy (if needed)
    //==========================================================================
    
    if (!EmptyQueue(F) || !EmptyQueue(F1) || !EmptyQueue(F3)) {
        printf("\n");
        printSeparator('═', 70);
        printf("            ⚡ STRATEGY CHANGE: GCD STRATEGY ⚡\n");
        printSeparator('═', 70);
        printf("\nAfter %d rounds, %d players remain. Switching to Part II!\n", 
               stats.partIRounds, F.size + F1.size + F3.size);
        
        printf("\nNew Rules:\n");
        printf("  • Generate 2 numbers, calculate GCD\n");
        printf("  • If GCD digit appears in either number → Score +1\n");
        printf("  • First to lead by 3 points wins\n");
        printf("  • Max 16 numbers (8 turns) per round\n");
        printf("  • 2 consecutive wins → Victory | 2 losses → Elimination\n");
        printSeparator('═', 70);
        
        if (!autoPlay) {
            printf("\nPress ENTER to begin Part II...");
            getchar();
        }
        
        // Merge F1 into F for Part II and reset consecutive stats
        while (!EmptyQueue(F1)) {
            Player p;
            Dequeue(&F1, &p);
            p.consecutiveWins = 0;
            p.consecutiveLosses = 0;
            Enqueue(&F, p);
        }
        
        // Reset consecutive stats for F
        element *temp = F.head;
        while (temp != NULL) {
            temp->info.consecutiveWins = 0;
            temp->info.consecutiveLosses = 0;
            temp = temp->next;
        }
        
        // Reset for F3
        temp = F3.head;
        while (temp != NULL) {
            temp->info.consecutiveWins = 0;
            temp->info.consecutiveLosses = 0;
            temp = temp->next;
        }
        
        int maxRoundsPartII = 2 * n;
        int partIIRoundLimit = 0;
        
        while (partIIRoundLimit < maxRoundsPartII) {
            // Check if game can continue
            if (EmptyQueue(F) && EmptyQueue(F3)) {
                printf("\n✓ All players classified in Part II!\n");
                break;
            }
            
            Player P1, P2;
            
            // Get two players
            if (!GetNextPlayer(&F1, &F, &F3, &P1, &LP)) {
                printf("\n⚠️  No players available for next round.\n");
                break;
            }
            
            if (!GetNextPlayer(&F1, &F, &F3, &P2, &LP)) {
                printf("\n⚠️  Only one player left. Auto-classifying...\n");
                classifyPlayerPartII(&P1, &F, &F3, &LP, &LG, true);
                break;
            }
            
            // Play round
            stats.totalRounds++;
            stats.partIIRounds++;
            partIIRoundLimit++;
            playRoundPartII(&P1, &P2, stats.totalRounds, autoPlay);
            updateStatistics(&P1, &P2);
            
            // Classify players
            printf("\n  Classifying players:\n");
            
            if (P1.currentScore > P2.currentScore) {
                classifyPlayerPartII(&P1, &F, &F3, &LP, &LG, true);
                classifyPlayerPartII(&P2, &F, &F3, &LP, &LG, false);
            } else if (P2.currentScore > P1.currentScore) {
                classifyPlayerPartII(&P2, &F, &F3, &LP, &LG, true);
                classifyPlayerPartII(&P1, &F, &F3, &LP, &LG, false);
            } else {
                // Draw: both go back to F
                Enqueue(&F, P1);
                Enqueue(&F, P2);
                printf("    🤝 Both players (draw) → F\n");
            }
            
            // Display state
            if (autoPlay) {
                displayCompactState(F, F1, F3, LP, LG);
            } else {
                displayGameState(F, F1, F3, LP, LG);
                printf("\nPress ENTER for next round...");
                getchar();
            }
        }
        
        // Final cleanup if time limit reached
        if (!EmptyQueue(F) || !EmptyQueue(F3)) {
            printf("\n⏰ Part II time limit reached! Final classification...\n");
            
            // Remaining F players go to LP
            while (!EmptyQueue(F)) {
                Player p;
                Dequeue(&F, &p);
                PUSH(&LP, p);
                printf("  ❌ %s → LP (timeout)\n", p.name);
            }
            
            // Remaining F3 players go to LP
            while (!EmptyQueue(F3)) {
                Player p;
                Dequeue(&F3, &p);
                PUSH(&LP, p);
                printf("  ❌ %s → LP (timeout)\n", p.name);
            }
        }
    }
    
    //==========================================================================
    // FINAL RESULTS
    //==========================================================================
    
    stats.endTime = time(NULL);
    
    printf("\n");
    printSeparator('═', 70);
    printf("                    🏁 GAME COMPLETE! 🏁\n");
    printSeparator('═', 70);
    
    displayGameState(F, F1, F3, LP, LG);
    showTop3Winners(LG);
    displayStatistics(LG, LP, stats);
    
    // Cleanup memory
    ClearQueue(&F);
    ClearQueue(&F1);
    ClearQueue(&F3);
    ClearList(&LG);
    ClearStack(&LP);
    
    printf("\n");
    printSeparator('═', 70);
    printf("              ✨ THANK YOU FOR PLAYING! ✨\n");
    printSeparator('═', 70);
    printf("\n");
    
    return 0;
}