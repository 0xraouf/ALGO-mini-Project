#include <stdio.h>
#include <time.h>
#include <stdlib.h>
void update (int *x){
    *x = 10;
}
int main(){
    int x = 4;
    int *y = &x;
    update(&x);
    printf("the value of x is %d \n" , x);
    printf("the value of y is %d " , *y);
    return 0;
}
