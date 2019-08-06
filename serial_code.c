#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <omp.h>

#include "simulation.h"

int main(int argc, int **argv)
{
    int num_updates = 100;

    int track_size = 20;

    int num_trains = 5;
    int num_stations = 4;


    //{ Error Checking:

    if (num_trains > track_size || num_stations > track_size)
    {
        return -1;  //This is impossible to simulate.
    }

    //}


    struct Train * train_track = (struct Train*)malloc(track_size * sizeof(struct Train));
    struct Station * station_track = (struct Station*)malloc(track_size * sizeof(struct Station));


    //{ Pre-initializations:

    for (int i = 0; i < track_size; ++i)
    {
        train_track[i].ID = 0;
        train_track[i].num_passengers = 0;

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
        }
        else
        {
            i--;    //This will make the loop slightly longer, forcing the trains to be placed randomly along the track.
        }
    }

    //}


    //{ Debug Info:

    printf("Stations:\n");

    for (int i = 0; i < track_size; ++i)
    {
        printf("station_track[%d].ID = %d\n", i, station_track[i].ID);

        printf("station_track[%d].passengers_arriving = %d\n", i, station_track[i].passengers_arriving);

        printf("station_track[%d].passengers_waiting = %d\n", i, station_track[i].passengers_waiting);

        printf("\n");
    }


    printf("\n\nTrains:\n");

    for (int i = 0; i < track_size; ++i)
    {
        printf("train_track[%d].ID = %d\n", i, train_track[i].ID);

        printf("train_track[%d].passengers_arriving = %d\n", i, train_track[i].num_passengers);

        printf("\n");
    }

    //}


    for (int i = 0; i < num_updates; ++i)
    {
        for (int q = 0; q < track_size; ++q)
        {
            if (train_track[q].ID != 0)
            {
                if (station_track[q].ID != 0)
                {
                    //Unloading process begins.
                }
                else    //Move the train if possible.
                {
                    int looking_position;

                    if (q == track_size - 1)
                    {
                        looking_position = 0;
                    }
                    else
                    {
                        looking_position = q + 1;
                    }

                    if (train_track[looking_position].ID == 0)  //There is a free spot to move.
                    {
                        train_track[looking_position].ID = train_track[q].ID;
                        train_track[looking_position].num_passengers = train_track[q].num_passengers;

                        train_track[q].ID = 0;
                        train_track[q].num_passengers = 0;
                    }
                    else
                    {
                        //Do nothing; it can't move.
                    }
                }

//                while(Train.num_passengers != 0)
//                {
//                    Train.num_passengers--;
//                    printf("People on train %d", Train.num_passengers);
//                    Station.passengers_arriving++;
//                    printf("People on track %d", Station.passengers_arriving);
//                }
            }
        }
    }
}
