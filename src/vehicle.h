#ifndef VEHICLE_H
#define VEHICLE_H

class Vehicle {
public:
    // Constructor to initialize a vehicle with a type
    Vehicle(int type);

    // Getter for the vehicle's ID
    int GetID() const;

    // Getter for the vehicle's type
    int GetType() const;

private:
    static int nextID; // Static variable to generate unique IDs for vehicles
    int id;            // Unique ID for each vehicle
    int type;          // Type of the vehicle
};

#endif // VEHICLE_H
