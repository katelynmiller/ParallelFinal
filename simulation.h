#ifndef SIMULATION_H
#define SIMULATION_H


struct Train
{
    int ID;

    int num_passengers;

    int done_station;
    int done_move;
};


struct Station
{
    int ID;

    int passengers_waiting;
    int passengers_arriving;
};


#endif  //SIMULATION_H
