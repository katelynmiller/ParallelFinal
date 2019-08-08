#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#include <omp.h>

#include "simulation.h"

int main(int argc, char **argv)
{

    int numRanks;
    int rank;

    //MPI Initialization

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
    MPI_Comm_size(MPI_COMM_WORLD, &numRanks);

    if (rank == 0) {

        //Simulation conditions
        int num_updates = 30000;

        int track_size = 800;

        int num_trains = 100;
        int num_stations = 80;

        //{ Error Checking:
        if (num_trains > track_size || num_stations > track_size)
        {
            return -1;  //This is impossible to simulate.
        }
        //}

        //Track and station location
        struct Train * train_track = (struct Train*)malloc(track_size * sizeof(struct Train));
        struct Station * station_track = (struct Station*)malloc(track_size * sizeof(struct Station));

        //where the train was last
        int* lastStation = malloc(sizeof(int) * num_trains);

        //{ Pre-initializations:
        for (int i = 0; i < track_size; ++i)
        {
            train_track[i].ID = 0;
            train_track[i].num_passengers = 600;
            train_track[i].done_station = 1;
            train_track[i].done_move = 0;

            station_track[i].ID = 0;
            station_track[i].passengers_arriving = 0;
            station_track[i].passengers_waiting = 0;
        }
        //}

        //{ Actual Track Initializations:
        srand(time(NULL));

        int id_counter = 1;

        for (int i = 0; i < num_trains; ++i)
        {
            int temp_position = rand() % track_size;

            if (train_track[temp_position].ID == 0)
            {
                train_track[temp_position].ID = id_counter++;
                train_track[temp_position].num_passengers = 10;
                train_track[temp_position].done_station = 1;
            }
            else
            {
                i--;    //This will make the loop slightly longer, forcing the trains to be placed randomly along the track.
            }
        }

        id_counter = 1;

        for (int i = 0; i < num_stations; ++i)
        {
            int temp_position = rand() % track_size;

            if (station_track[temp_position].ID == 0)
            {
                station_track[temp_position].ID = id_counter++;
                station_track[temp_position].passengers_waiting = 5;
            }
            else
            {
                i--;    //This will make the loop slightly longer, forcing the trains to be placed randomly along the track.
            }
        }
        //}

        //The request relating to a rank's work
        MPI_Request* requests = malloc(sizeof(MPI_Request) * numRanks-1);

        int** recvData = malloc(sizeof(int*) * numRanks-1);
        for (int i = 0; i < numRanks-1; i++) {
            recvData[i] = malloc(sizeof(int) * 2);
        }

        int* rankTrain = malloc(sizeof(int) * numRanks-1);
        for (int i = 0; i < numRanks-1; i++) {
            rankTrain[i] = -1;
        }

        //time update
        for (int i = 0; i < num_updates; ++i)
        {

            printf("Time %d\n-----------------------\n", i);

            //unsigned int retTime = time(0) + 1;   // Get finishing time.
            //while (time(0) < retTime);

            //Get any work results from ranks
            for (int i = 0; i < numRanks-1; i++) {

                if (rankTrain[i] == -1) { continue; }

                int done;

                MPI_Test(&requests[i],
                         &done,
                         MPI_STATUS_IGNORE);

                if (done) {
                    
                    //Update train & station data based on received data
                    train_track[rankTrain[i]].num_passengers = recvData[i][0];
                    train_track[rankTrain[i]].done_station = 1;
                    train_track[rankTrain[i]].done_move = 0;
                    station_track[rankTrain[i]].passengers_waiting = recvData[i][1];

                    printf("Train %d finished unloading & loading at station %d.\n", train_track[rankTrain[i]].ID, station_track[rankTrain[i]].ID);

                    //Reset request to show a rank can take work
                    rankTrain[i] = -1;

                }

            }

            //Check every track position
            for (int pos = 0; pos < track_size; pos++) {

                //Train here that's done working!
                if (train_track[pos].ID != 0 && train_track[pos].done_station) {

                    //Train at station!
                    if (station_track[pos].ID != 0 && lastStation[train_track[pos].ID - 1] != station_track[pos].ID) {

                        lastStation[train_track[pos].ID-1] = station_track[pos].ID;

                        int found = 0;

                        //send to available rank
                        for (int i = 0; i < numRanks-1; i++) {

                            if (rankTrain[i] == -1) {

                                found = 1;
                                
                                printf("Train %d began unloading at station %d.\n", train_track[pos].ID, station_track[pos].ID);
                                train_track[pos].done_station = 0;

                                int* workData = malloc(2 * sizeof(int));

                                workData[0] = train_track[pos].num_passengers;
                                workData[1] = station_track[pos].passengers_waiting;

                                //Send to this rank!
                                MPI_Send(workData,
                                         2,
                                         MPI_INT,
                                         i+1,
                                         0,
                                         MPI_COMM_WORLD);

                                rankTrain[i] = pos;

                                //Start a non-blocking receive!
                                MPI_Irecv(recvData[i],
                                          2,
                                          MPI_INT,
                                          i+1,
                                          0,
                                          MPI_COMM_WORLD,
                                          &requests[i]);

                                break;

                            }

                        } // end for


                        if (found == 0) {
                            
                            printf("Train %d delayed at station %d.\n", train_track[pos].ID, station_track[pos].ID);
                            train_track[pos].done_move = 1;

                        }

                    } else {

                        //No work, no station, must move!
                        if (train_track[pos].done_move == 0) {

                            //Get position to move to
                            int nextPos = pos + 1;

                            if (nextPos == track_size) {

                                nextPos = 0;

                            }

                            //if empty, move!
                            if (train_track[nextPos].ID == 0) {

                                lastStation[train_track[pos].ID-1] = 0;

                                train_track[nextPos].ID = train_track[pos].ID;
                                train_track[nextPos].done_move = 1;
                                train_track[nextPos].num_passengers = train_track[pos].num_passengers;
                                train_track[nextPos].done_station = 1;

                                printf("(Train %d) pos %d -> pos %d.\n", train_track[pos].ID, pos, nextPos);

                                train_track[pos].ID = 0;

                            } else {

                                printf("Train %d could not move to position %d.\n", train_track[pos].ID, nextPos);
                                train_track[pos].done_move = 1;

                            }

                        }

                    }

                } //end valid pos

            } //end pos checking

            //Reset movement so everyone will be moved if possible
            for (int tpos = 0; tpos < track_size; tpos++) {

                if (train_track[tpos].ID != 0) {

                    train_track[tpos].done_move = 0;

                }

            }

            printf("-----------------------\n");

        } //end time update

        //timesteps are done. Tell ranks to quit.
        for (int i = 1; i < numRanks; i++) {

            int* workData = malloc(2 * sizeof(int));

            workData[0] = -1;

            //Send to this rank!
            MPI_Send(workData,
                        2,
                        MPI_INT,
                        i,
                        0,
                        MPI_COMM_WORLD);

        }

    } else { //end rank 0

        while(1) {

            //Recieving data for my train & station
            int* workData = malloc(2 * sizeof(int));

            MPI_Recv(workData,
                    2,
                    MPI_INT,
                    0,
                    0,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);

            if (workData[0] == -1) {
                free(workData);
                break;
            }

            //Only gave the structs what was needed for work.
            struct Train train = {0, workData[0], 0, 0};
            struct Station station = {0, workData[1], 0};

            free(workData);

            //begin handling train

            #pragma omp parallel
            {

                int allPassengers = train.num_passengers;

                #pragma omp for
                for (int i = 0; i < allPassengers; i++) {

                    train.num_passengers--;
                    station.passengers_arriving++;
                    //printf("Update Train (%d) Station Waiting (%d) Station Arriving (%d)\n",train->num_passengers,station->passengers_waiting,station->passengers_arriving);

                } //end for

            } //end parallel

            #pragma omp parallel
            {

                int passengers_waiting = station.passengers_waiting;

                #pragma omp for
                for (int i = 0; i < passengers_waiting; i++) {

                    train.num_passengers++;
                    station.passengers_waiting--;
                    //printf("Update Train (%d) Station Waiting (%d) Station Arriving (%d)\n",train->num_passengers,station->passengers_waiting,station->passengers_arriving);

                } //end for

            } //end parallel

            //turn arriving into waiting
            station.passengers_waiting = station.passengers_arriving;
            station.passengers_arriving = 0;

            //All done

            int* workDone = malloc(2 * sizeof(int));

            workDone[0] = train.num_passengers;
            workDone[1] = station.passengers_waiting;

            MPI_Send(workDone,
                    2,
                    MPI_INT,
                    0,
                    0,
                    MPI_COMM_WORLD);

        } // end while

    }

    MPI_Finalize();

}