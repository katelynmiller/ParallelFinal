#include "Itinerary.h"

Itinerary::Itinerary(int num_stations, int num_passengers)
{
    stations = new int[num_stations];

    passengers = new int[num_passengers];
}


/**
* Gets the passenger at the given index.
*/
int Itinerary::get_passenger(int index)
{
    return passengers[index];
}


/**
* Gets the station at the given index.
*/
int Itinerary::get_station(int index)
{
    return stations[index];
}
