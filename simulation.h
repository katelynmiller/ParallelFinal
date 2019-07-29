#ifndef SIMULATION_H
#define SIMULATION_H


struct Train
{
    int num_passengers;
    int ID;
};


struct Station
{
    int passengers_waiting;
    int passengers_arriving;
};


#endif  //SIMULATION_H
