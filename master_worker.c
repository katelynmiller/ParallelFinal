#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, int **argv){

    int train_amount = 3;

    #pragma omp single for{
        for(int i=0;i<train_amount;i++){
        give_train();
        }
    }

    #pragma omp parallel for {
        if(train_allowed==true){
            drop_off();
            move_train();
        }
        else{
            wait();
        }
    }

}

