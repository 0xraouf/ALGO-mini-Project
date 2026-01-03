#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

//--------------------------------------------------------------------------------------------------------------
// RECORDS - Following Course Structure (Chained Implementation)

// Define Element structure FIRST
typedef struct Element {
    struct Player info;      // Player information
    struct Element *next;    // Pointer to next element
} Element;

// Player structure
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

// Queue structure with Head and Tail pointers
typedef struct Queue {
    Element *Head;   // Pointer to first element
    Element *Tail;   // Pointer to last element
} Queue;

//-----------------------------------------------------------------------------------------------------------------------------------
// QUEUE PRIMITIVES - Following Course Logic (Pages 17-19)

/**
 * InitQueue: Initialize an empty queue
 * Input/Output: F (Queue)
 * Complexity: O(1)
 */
void InitQueue(Queue *F) {
    F->Head = NULL;
    F->Tail = NULL;
}

/**
 * EmptyQueue: Check if queue is empty
 * Input: F (Queue)
 * Output: boolean (1 = empty, 0 = not empty)
 * Complexity: O(1)
 */
int EmptyQueue(Queue *F) {
    if (F->Head == NULL) {
        return 1;  // true
    } else {
        return 0;  // false
    }
}

/**
 * Enqueue: Add element to the tail of queue
 * Input/Output: F (Queue)
 * Input: x (Player)
 * Complexity: O(1)
 * Logic from course page 17
 */
void Enqueue(Queue *F, Player x) {
    Element *nouv;
    
    // Allocate memory for new element
    nouv = (Element*)malloc(sizeof(Element));
    
    if (nouv == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    // Store player info in new element
    nouv->info = x;
    nouv->next = NULL;
    
    if (F->Tail == NULL) {
        // Empty queue case
        F->Head = nouv;
        F->Tail = nouv;
    } else {
        // Add to tail
        F->Tail->next = nouv;
        F->Tail = nouv;
    }
}

/**
 * Dequeue: Remove element from head of queue
 * Input/Output: F (Queue)
 * Output: x (Player)
 * Complexity: O(1)
 * Logic from course page 18
 */
void Dequeue(Queue *F, Player *x) {
    Element *temp;
    
    if (EmptyQueue(F)) {
        printf("Error: Queue is empty! Cannot dequeue.\n");
        return;
    }
    
    // Get the info from head
    *x = F->Head->info;
    
    // Save head pointer
    temp = F->Head;
    
    // Move head to next element
    F->Head = F->Head->next;
    
    // Free memory
    free(temp);
    
    // If queue became empty, update tail
    if (F->Head == NULL) {
        F->Tail = NULL;
    }
}

/**
 * HeadQueue: Return element at head without removing
 * Input: F (Queue)
 * Output: Player
 * Complexity: O(1)
 * Logic from course page 19
 */
Player HeadQueue(Queue *F) {
    if (EmptyQueue(F)) {
        Player dummy;
        dummy.playerID = -1;
        printf("Error: Queue is empty!\n");
        return dummy;
    }
    
    return F->Head->info;
}

/**
 * TailQueue: Return element at tail without removing
 * Input: F (Queue)
 * Output: Player
 * Complexity: O(1)
 * Logic from course page 19
 */
Player TailQueue(Queue *F) {
    if (EmptyQueue(F)) {
        Player dummy;
        dummy.playerID = -1;
        printf("Error: Queue is empty!\n");
        return dummy;
    }
    
    return F->Tail->info;
}

/**
 * QueueSize: Count number of elements in queue
 * Input: F (Queue)
 * Output: integer (size)
 * Complexity: O(n)
 */
int QueueSize(Queue *F) {
    int count = 0;
    Element *current = F->Head;
    
    while (current != NULL) {
        count++;
        current = current->next;
    }
    
    return count;
}

/**
 * DisplayQueue: Display all elements in queue
 * Input: F (Queue), queueName (string)
 * Complexity: O(n)
 */
void DisplayQueue(Queue *F, const char* queueName) {
    if (EmptyQueue(F)) {
        printf("%s: Empty\n", queueName);
        return;
    }
    
    printf("\n%s:\n", queueName);
    printf("┌────────┬──────────────────┬─────┬────────┬────────┬─────────┐\n");
    printf("│ ID     │ Name             │ Age │ Wins   │ Losses │ Total   │\n");
    printf("├────────┼──────────────────┼─────┼────────┼────────┼─────────┤\n");
    
    Element *current = F->Head;
    while (current != NULL) {
        printf("│ %-6d │ %-16s │ %-3d │ %-6d │ %-6d │ %-7d │\n",
               current->info.playerID,
               current->info.name,
               current->info.age,
               current->info.roundWon,
               current->info.roundLost,
               current->info.totalScore);
        current = current->next;
    }
    
    printf("└────────┴──────────────────┴─────┴────────┴────────┴─────────┘\n");
}

/**
 * FreeQueue: Free all memory in queue
 * Input/Output: F (Queue)
 * Complexity: O(n)
 */
void FreeQueue(Queue *F) {
    Player temp;
    
    while (!EmptyQueue(F)) {
        Dequeue(F, &temp);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
// GAME FUNCTIONS

/**
 * Generate a random number
 */
int RandomNumber() {
    return rand() % 1000000;  // Random number between 0 and 999999
}

/**
 * Calculate the sum of digits of a number
 */
int SumOfDigits(int value) {
    int sum = 0;
    value = abs(value);  // Handle negative numbers
    
    while (value != 0) {
        sum += value % 10;
        value /= 10;
    }
    
    return sum;
}

/**
 * Create a new player with initialized values
 */
Player CreatePlayer(int id, const char* name, int age) {
    Player p;
    p.playerID = id;
    strcpy(p.name, name);
    p.age = age;
    p.currentScore = 0;
    p.totalScore = 0;
    p.roundWon = 0;
    p.roundLost = 0;
    p.consecutiveWins = 0;
    p.consecutiveLosses = 0;
    return p;
}

/**
 * Play one round between two players
 * Logic from project description Part I
 */
void GameRound(Player *P1, Player *P2, int roundNumber) {
    int generatedValues = 0;
    
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("           ROUND %d: %s vs %s           \n", roundNumber, P1->name, P2->name);
    printf("═══════════════════════════════════════════════════════════\n");
    
    // Reset current scores for this round
    P1->currentScore = 0;
    P2->currentScore = 0;
    
    // Play until difference reaches 3 or 12 values generated (6 per player)
    while (abs(P1->currentScore - P2->currentScore) < 3 && generatedValues < 12) {
        
        // ──────────────────────────────────────
        // PLAYER 1 TURN
        // ──────────────────────────────────────
        printf("\n─── %s's Turn ───\n", P1->name);
        printf("Press 'ENTER' to generate number...\n");
        getchar();
        
        int random1 = RandomNumber();
        printf("System generated: %d\n", random1);
        
        int sum1 = SumOfDigits(random1);
        printf("Sum of digits: %d\n", sum1);
        
        if (sum1 % 5 == 0) {
            printf("✓ %s gets a point! (+1)\n", P1->name);
            P1->currentScore++;
        } else {
            printf("✗ %s gets no point (0)\n", P1->name);
        }
        
        printf("Current Score: %s=%d, %s=%d\n", 
               P1->name, P1->currentScore, P2->name, P2->currentScore);
        
        generatedValues++;
        
        // Check if difference reached 3
        if (abs(P1->currentScore - P2->currentScore) >= 3) {
            break;
        }
        
        // ──────────────────────────────────────
        // PLAYER 2 TURN
        // ──────────────────────────────────────
        printf("\n─── %s's Turn ───\n", P2->name);
        printf("Press 'ENTER' to generate number...\n");
        getchar();
        
        int random2 = RandomNumber();
        printf("System generated: %d\n", random2);
        
        int sum2 = SumOfDigits(random2);
        printf("Sum of digits: %d\n", sum2);
        
        if (sum2 % 5 == 0) {
            printf("✓ %s gets a point! (+1)\n", P2->name);
            P2->currentScore++;
        } else {
            printf("✗ %s gets no point (0)\n", P2->name);
        }
        
        printf("Current Score: %s=%d, %s=%d\n", 
               P1->name, P1->currentScore, P2->name, P2->currentScore);
        
        generatedValues++;
    }
    
    //______________________________________________________________________________________________________
    // UPDATE PLAYER STATISTICS
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("                   ROUND FINISHED                          \n");
    printf("═══════════════════════════════════════════════════════════\n");
    
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
        printf("Score: %d - %d\n", P2->currentScore, P1->currentScore);
        
    } else {
        // Draw - both get their scores added
        P1->totalScore += P1->currentScore;
        P2->totalScore += P2->currentScore;
        
        printf("🤝 DRAW!\n");
        printf("Score: %d - %d\n", P1->currentScore, P2->currentScore);
    }
    
    printf("\n%s Stats: Wins=%d, Losses=%d, ConsecWins=%d, ConsecLosses=%d\n",
           P1->name, P1->roundWon, P1->roundLost, 
           P1->consecutiveWins, P1->consecutiveLosses);
    printf("%s Stats: Wins=%d, Losses=%d, ConsecWins=%d, ConsecLosses=%d\n",
           P2->name, P2->roundWon, P2->roundLost, 
           P2->consecutiveWins, P2->consecutiveLosses);
}

/**
 * Automatically build initial queue with test players
 * Following course exercise 1 (page 26)
 */
void BuildInitialQueue(Queue *F, int n) {
    const char* names[] = {
        "Alice", "Bob", "Charlie", "Diana", "Eve", 
        "Frank", "Grace", "Henry", "Iris", "Jack",
        "Kate", "Leo", "Mia", "Noah", "Olivia"
    };
    
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("         BUILDING INITIAL QUEUE WITH %d PLAYERS           \n", n);
    printf("═══════════════════════════════════════════════════════════\n");
    
    InitQueue(F);  // Initialize the queue first
    
    for (int i = 0; i < n; i++) {
        int age = 18 + (rand() % 30);  // Random age between 18-47
        Player p = CreatePlayer(i + 1, names[i % 15], age);
        Enqueue(F, p);  // Add element to queue at the Tail
        printf("Added: ID=%d, Name=%s, Age=%d\n", p.playerID, p.name, p.age);
    }
    
    printf("\n✓ Initial queue built successfully!\n");
}

//-----------------------------------------------------------------------------------------------------------------------------------
// MAIN FUNCTION

int main() {
    srand(time(NULL));
    
    // Initialize queues following course structure
    Queue F, F1, F3;
    InitQueue(&F);
    InitQueue(&F1);
    InitQueue(&F3);
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║         WELCOME TO THE RANDOM GAME SYSTEM                ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    // Build initial queue with test players
    int numPlayers = 6;  // You can change this for testing
    BuildInitialQueue(&F, numPlayers);
    
    // Display initial queue
    printf("\n");
    DisplayQueue(&F, "Initial Queue F");
    
    // Test: Play one round
    printf("\n\nPress ENTER to start the first game round...\n");
    getchar();
    
    if (QueueSize(&F) >= 2) {
        Player p1, p2;
        
        // Remove two players from head of queue
        Dequeue(&F, &p1);
        Dequeue(&F, &p2);
        
        // Play the round
        GameRound(&p1, &p2, 1);
        
        // For now, just put them back in the queue
        // (Later you'll implement the logic to move winner/loser)
        Enqueue(&F, p1);
        Enqueue(&F, p2);
        
        printf("\n");
        DisplayQueue(&F, "Queue F After Round 1");
    }
    
    // Clean up memory
    FreeQueue(&F);
    FreeQueue(&F1);
    FreeQueue(&F3);
    
    printf("\n\n═══════════════════════════════════════════════════════════\n");
    printf("              THANK YOU FOR PLAYING!                       \n");
    printf("═══════════════════════════════════════════════════════════\n\n");
    
    return 0;
}