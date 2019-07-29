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


    //{ Track Initializations:

    srand(time(NULL));


    int id_counter = 1;

    for(int i = 0; i < num_trains; ++i)
    {
        int temp_position = rand() % track_size;

        if (train_track[temp_position].ID != 0)
        {
            train_track[temp_position].ID = id_counter++;
        }
        else
        {
            i--;    //This will make the loop slightly longer, forcing the trains to be placed randomly along the track.
        }
    }


    id_counter = 1;

    for(int i = 0; i < num_stations; ++i)
    {
        int temp_position = rand() % track_size;

        if (station_track[temp_position].ID != 0)
        {
            station_track[temp_position].ID = id_counter++;
        }
        else
        {
            i--;    //This will make the loop slightly longer, forcing the trains to be placed randomly along the track.
        }
    }

    //}


    for(int i = 0; i < num_trains; ++i)
    {
        //
    }


    for(int i = 0; i < num_stations; ++i)
    {
        //
    }


    for(int i = 0; i < num_updates; ++i)
    {
        //
    }
}
