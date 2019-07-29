#include <stdio.h>
#include "mpi.h"
#include <omp.h>

struct Train {
    int id;
    int curPassengers;
    int position;
};

struct Station {
    int id;
    int waitingPassengers;
    int arrivingPassengers;
};

void takeTrain(struct Train *train, struct Station *station) {

    #pragma omp parallel
    {

        #pragma omp single
        {

            int allPassengers = train->curPassengers;
            for (int i = 0; i < allPassengers; i++) {
                #pragma omp task
                {

                    train->curPassengers--;
                    station->arrivingPassengers++;
                    printf("Update Train (%d) Station Waiting (%d) Station Arriving (%d)\n",train->curPassengers,station->waitingPassengers,station->arrivingPassengers);

                } //end task
            }

            int waitingPassengers = station->waitingPassengers;
            for (int i = 0; i < waitingPassengers; i++) {
                #pragma omp task
                {

                    train->curPassengers++;
                    station->waitingPassengers--;
                    printf("Update Train (%d) Station Waiting (%d) Station Arriving (%d)\n",train->curPassengers,station->waitingPassengers,station->arrivingPassengers);

                } // end task
            }

            printf("BLEHHHHH\n");
            station->waitingPassengers = station->arrivingPassengers;
            station->arrivingPassengers = 0;

        } //end single

    } //end parallel

} //end func

int main(void) {

    struct Station station = {0, 20, 0};
    struct Train train = {0, 10, 0};

    takeTrain(&train, &station);

    printf("Train Passengers (from 10): %d\n", train.curPassengers);
    printf("Station Waiting (from 20): %d\n", station.waitingPassengers);

}