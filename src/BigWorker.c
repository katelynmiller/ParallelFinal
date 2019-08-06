#include <stdio.h>
#include "mpi.h"
#include <omp.h>
#include "simulation.h"

void takeTrain(struct Train *train, struct Station *station) {

    #pragma omp parallel
    {

        #pragma omp single
        {

            int allPassengers = train->num_passengers;
            for (int i = 0; i < allPassengers; i++) {
                #pragma omp task
                {

                    train->num_passengers--;
                    station->passengers_arriving++;
                    //printf("Update Train (%d) Station Waiting (%d) Station Arriving (%d)\n",train->num_passengers,station->passengers_waiting,station->passengers_arriving);

                } //end task
            }

            int passengers_waiting = station->passengers_waiting;
            for (int i = 0; i < passengers_waiting; i++) {
                #pragma omp task
                {

                    train->num_passengers++;
                    station->passengers_waiting--;
                    //printf("Update Train (%d) Station Waiting (%d) Station Arriving (%d)\n",train->num_passengers,station->passengers_waiting,station->passengers_arriving);

                } // end task
            }

            station->passengers_waiting = station->passengers_arriving;
            station->passengers_arriving = 0;

        } //end single

    } //end parallel

    int status = 0;

    MPI_Send(&status,
             1,
             MPI_INT,
             0,
             0,
             MPI_COMM_WORLD);

} //end func

int main(int argc, char** argv) {

    int numRanks;
    int rank;

    //MPI Initialization

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
    MPI_Comm_size(MPI_COMM_WORLD, &numRanks);

    struct Station station = {0, 200, 0};
    struct Train train = {0, 100};

    int response = 0;
    int s = 1;

    if (rank == 0) {

        MPI_Recv(&response,
                 1,
                 MPI_INT,
                 1,
                 0,
                 MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

    } else {

        takeTrain(&train, &station);
        printf("Train Passengers (from 10): %d\n", train.num_passengers);
        printf("Station Waiting (from 20): %d\n", station.passengers_waiting);

    }

    MPI_Finalize();

}