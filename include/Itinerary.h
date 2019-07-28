#ifndef ITINERARY_H
#define ITINERARY_H


class Itinerary
{
    public:
        Itinerary(int, int);

        int get_passenger(int); //Change the return type to Passenger when it is made.
        int get_station(int);   //Change the return type to Station when it is made.

    private:
        int * stations;
        int * passengers;
};

#endif // ITINERARY_H
