#ifndef SIMULATION_H
#define SIMULATION_H


struct Train
{
    int ID = 0;

    int num_passengers = 0;
};


struct Station
{
    int ID = 0;

    int passengers_waiting = 0;
    int passengers_arriving = 0;
};


#endif  //SIMULATION_H
