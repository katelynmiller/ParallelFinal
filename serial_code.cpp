#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include <time.h>

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


    Train * train_track = new Train[track_size];
    Station * station_track = new Station[track_size];


    //{ Track Initializations:

    srand(time(NULL));


    int id_counter = 0;

    for(int i = 0; i < num_trains; ++i)
    {
        int temp_position = rand() % track_size;

        if (train_track[temp_position] != NULL)
        {
            train_track[temp_position] = new Train(id_counter++);
        }
        else
        {
            i--;    //This will make the loop slightly longer, forcing the trains to be placed randomly along the track.
        }
    }


    id_counter = 0;

    for(int i = 0; i < num_stations; ++i)
    {
        int temp_position = rand() % track_size;

        if (station_track[temp_position] != NULL)
        {
            station_track[temp_position] = new Station(id_counter++);
        }
        else
        {
            i--;    //This will make the loop slightly longer, forcing the trains to be placed randomly along the track.
        }
    }

    //}


    for(int i = 0; i < num_updates; ++i)
    {


    }
}
