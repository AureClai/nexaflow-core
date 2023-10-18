#include "Vehicle.h"

int Vehicle::nextID = 1; // Initialize the nextID counter

Vehicle::Vehicle(int type) : type(type) {
    id = nextID++; // Assign a unique ID to the vehicle and increment nextID
}

int Vehicle::getID() const {
    return id;
}

int Vehicle::getType() const {
    return type;
}
