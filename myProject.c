#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>
//--------------------------------------------------------------------------------------------------------------
//records
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
//functions :
//queue operations :
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

//Stacks operations :
    void InitStack(Stack *S){
        *S = NULL;
    }

    void PUSH (Stack *S , Player P){
        element *N = malloc(sizeof(element));
        N->info = P;
        N->next = *S;
        *S = N;
    }

    void POP (Stack *S , Player *P){

        *P = (*S)->info;
        element *temp = *S;
        *S = (*S)->next ;
        free(temp); 
    }

    bool EmptyStack (Stack S){
        return( S == NULL );
    }

    Player TOPStack(Stack S){
        return S->info;
    }

//The following functions are for the logic of the game

//______________________________________________________________________________________________________________
//Common Functions 

    //this function generate a random number
    int randomNumber (){
        return rand();
    }

    //this function insert the values of a list in descending order 
    void insertSortedLG(Elist *LG, Player p) {
        element *N = (element*)malloc(sizeof(element));
        if (N == NULL) return; // Error handling
        
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

    //this function fill the queue F with the informations of the players 
    void AutoFill(Queue *F, int n) {
        for (int i = 0; i < n; i++) {
            Player p;
            
            //give the player a name automatically (Player_01, Player_02, etc.)
            sprintf(p.name, "Player_%02d", i+1); 
            
            //an ID for each player start from 1000
            p.playerID = 1000 + i;
            
            //a random number between 18 and 27 will be generated 
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

    //This function will update the players statistics after every round 
    void updatePlayersStatistics (Player *P1 , Player *P2){
        if (P1->currentScore > P2->currentScore) {
            // Player 1 wins
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
            // Player 2 wins
            P2->roundWon++;
            P2->consecutiveWins++;
            P2->consecutiveLosses = 0;
            P2->totalScore += P2->currentScore;

            P1->roundLost++;
            P1->consecutiveLosses++;
            P1->consecutiveWins = 0;
            P1->totalScore += P1->currentScore;
            
            printf("🏆 WINNER: %s (ID: %d)\n", P2->name, P2->playerID);
            printf("Score: %d - %d\n", P1->currentScore, P2->currentScore);
            
        } else {
            // Draw
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

    //This function return the next plyaer 
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
            // if only one player remains in F3, he will be moved to LP
            if (F3->head == F3->tail) {
                Player last;
                Dequeue(F3, &last);
                PUSH(LP, last);
                return false; // No more players available to play
            }
            Dequeue(F3, p);
            return true;
        }
        return false;
    }

    //This fucntion check if the queues are empty or not 
    bool isGameOver(Queue F, Queue F1, Queue F3){
        return EmptyQueue(F) && EmptyQueue(F1) && EmptyQueue(F3);
    }



//________________________________________________________________________________________________________________________
//Part One Functions :

    //this function calculate the sum of the digit of a number
    int sumOfDigits(int value){
        int sum = 0;
        while(value!=0){
            sum += value % 10;
            value /= 10;
        }
        return sum;
    }

    //This function will play a round between Player 1 and Player 2 (strategy of part One )
    void playRoundPartOne (Player *P1,Player *P2){
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

    }

    //This function will classify the player in F , F1 , F3 , LG or LP in the conditions of Part 1
    void classifyPartOne(Player *P, Queue *F, Queue *F1, Queue *F3, Stack *LP, Elist *LG){
        if (P->roundLost >= 5) {
            PUSH(LP, *P);
        }
        else if (P->roundWon >= 5) {
            insertSortedLG(LG, *P);
        }
        else if (P->consecutiveWins >= 3) {
            Enqueue(F1, *P);
        }
        else if (P->roundLost >= 3) {
            Enqueue(F3, *P);
        }
        else {
            Enqueue(F, *P);
        }
    }


//________________________________________________________________________________________________________________________
//Part Two Functions :

    //This function will classify the player in F , F1 , F3 , LG or LP in the conditions of Part 2
    void classifyPartTwo(Player *P, Queue *F3, Stack *LP, Elist *LG){
        if (P->roundLost >= 2) {
            PUSH(LP, *P);
        }
        else if (P->consecutiveWins >= 2) {
            insertSortedLG(LG, *P);
        }else {
            Enqueue(F3,*P);
        }

    }

    //This funciton is to check if the game is over or not and it's used in the Part 2 of the Game 
    bool forcedGameOver(int strategy2Rounds, int n){
        return (strategy2Rounds >= 2 * n);
    }

    //This function calculate the GCD 
    int gcd(int a, int b) {
        while (b != 0) {
            int r = a % b;
            a = b;
            b = r;
        }
        return a;
    }

    //This function check if the GCD appears in a number 
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


    //This function represent the strategy of part 2 
    void playRoundPartTwo (Player *P1, Player *P2) {

        int generatedValue2 = 0;

        printf("\n═══════════════════════════════════════\n");
        printf("       PART II ROUND: %s vs %s         \n", P1->name, P2->name);
        printf("═══════════════════════════════════════\n");

        P1->currentScore = 0;
        P2->currentScore = 0;

        while (abs(P1->currentScore - P2->currentScore) < 3 && generatedValue2 < 16) {

            /* ================= PLAYER 1 TURN ================= */

            printf("\n─── %s's Turn ───\n", P1->name);
            printf("press 'ENTER' to start . . .\n");
            getchar();

            int a1 = randomNumber();
            int b1 = randomNumber();

            printf("The system generated the values: %d and %d\n", a1, b1);
            printf("(press enter to continue)");
            getchar();

            int g1 = gcd(a1, b1);
            printf("GCD(%d , %d) = %d\n", a1, b1, g1);
            printf("(press enter to continue)");
            getchar();

            if (gcdDigitCondition(a1, b1)) {
                printf("%s get a point !(+1)\n", P1->name);
                P1->currentScore++;
            } else {
                printf("%s DID NOT get a point !(+0)\n", P1->name);
            }

            printf("%s's score: %d\n", P1->name, P1->currentScore);
            printf("(press enter to continue)");
            getchar();

            generatedValue2 += 2;

            if (abs(P1->currentScore - P2->currentScore) >= 3)
                break;

            /* ================= PLAYER 2 TURN ================= */

            printf("\n─── %s's Turn ───\n", P2->name);
            printf("press 'ENTER' to start . . .\n");
            getchar();

            int a2 = randomNumber();
            int b2 = randomNumber();

            printf("The system generated the values: %d and %d\n", a2, b2);
            printf("(press enter to continue)");
            getchar();

            int g2 = gcd(a2, b2);
            printf("GCD(%d , %d) = %d\n", a2, b2, g2);
            printf("(press enter to continue)");
            getchar();

            if (gcdDigitCondition(a2, b2)) {
                printf("%s get a point !(+1)\n", P2->name);
                P2->currentScore++;
            } else {
                printf("%s DID NOT get a point !(+0)\n", P2->name);
            }

            printf("%s's score: %d\n", P2->name, P2->currentScore);
            printf("(press enter to continue)");
            getchar();

            generatedValue2 += 2;
        }

        printf("\n═══════════════════════════════════════\n");
        printf("            ROUND FINISHED             \n");
        printf("═══════════════════════════════════════\n");
    }

//________________________________________________________________________________________________________________________



//The main function : 
int main() {
    srand(time(NULL));

    int n;
    int roundsPlayed = 0;
    int strategy2Rounds = 0;
    int strategy = 1;   // 1 = Part I, 2 = Part II

    Queue F, F1, F3;
    Elist LG = NULL;
    Stack LP = NULL;

    Initqueue(&F);
    Initqueue(&F1);
    Initqueue(&F3);
    InitStack(&LP);

    printf("Enter number of players: ");
    scanf("%d", &n);
    getchar(); // consume newline

    AutoFill(&F, n);

    Player currentPlayer, challenger;
    bool hasCurrentPlayer = false;

    /* ===================== MAIN GAME LOOP ===================== */

    while (!isGameOver(F, F1, F3)) {

        /* -------- Strategy switch -------- */
        if (strategy == 1 && roundsPlayed >= 3 * n) {
            strategy = 2;
            printf("\n════════ STRATEGY CHANGED TO PART II ════════\n");
        }

        if (strategy == 2 && forcedGameOver(strategy2Rounds, n)) {
            printf("\n════════ FORCED GAME OVER (PART II) ════════\n");

            /* Move remaining players */
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

        /* -------- Select current player -------- */
        if (!hasCurrentPlayer) {
            if (!GetPlayer(&F1, &F, &F3, &currentPlayer, &LP))
                break;
            hasCurrentPlayer = true;
        }

        /* -------- Select challenger -------- */
        if (!GetPlayer(&F1, &F, &F3, &challenger, &LP))
            break;

        /* -------- Play round -------- */
        if (strategy == 1) {
            playRoundPartOne(&currentPlayer, &challenger);
        } else {
            playRoundPartTwo(&currentPlayer, &challenger);
            strategy2Rounds++;
        }

        updatePlayersStatistics(&currentPlayer, &challenger);
        roundsPlayed++;

        /* -------- Decide outcome -------- */
        if (currentPlayer.currentScore > challenger.currentScore) {
            /* challenger loses */
            if (strategy == 1)
                classifyPartOne(&challenger, &F, &F1, &F3, &LP, &LG);
            else
                classifyPartTwo(&challenger, &F3, &LP, &LG);
        }
        else if (challenger.currentScore > currentPlayer.currentScore) {
            /* current player loses */
            if (strategy == 1)
                classifyPartOne(&currentPlayer, &F, &F1, &F3, &LP, &LG);
            else
                classifyPartTwo(&currentPlayer, &F3, &LP, &LG);

            currentPlayer = challenger;   // winner continues
        }
        else {
            /* draw */
            Enqueue(&F, currentPlayer);
            Enqueue(&F, challenger);
            hasCurrentPlayer = false;
        }
    }

    /* ===================== END OF GAME ===================== */

    printf("\n════════ GAME OVER ════════\n");
    printf("Total rounds played: %d\n", roundsPlayed);

    /* Optional: display LG and LP here */

    return 0;
}
