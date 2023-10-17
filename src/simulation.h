#ifndef SIMULATION_H
#define SIMULATION_H

#include "network.h"
#include "event.h"
#include "vehicle.h"
#include <unordered_map>
#include <vector>

class Simulation
{
public:
    Simulation(Network network) : network(network) {}

public:
    Network network;
    std::unordered_map<int, Vehicle *> vehicles;

    std::vector<Event *> initializeAtNodeAtCapacity(int nodeID, int numberOfVehicles);
    std::vector<Event *> compileNodeEvents();
};

#endif