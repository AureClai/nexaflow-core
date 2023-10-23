#ifndef SIMULATION_H
#define SIMULATION_H

#include "network.h"
#include "event.h"
#include "vehicle.h"
#include <unordered_map>
#include <vector>
#include <chrono>

// Define an enumeration for the simulation status
enum class SimulationStatus
{
    CREATED,
    INITIALIZED,
    COMPLETED
};

class Simulation
{
public:
    Simulation(Network network, float endTime) : network(network), status(SimulationStatus::CREATED), calculationTime(-1), simulationEndTime(endTime) {}

private:
    // std::vector<Event *> events;
    SimulationStatus status;
    std::chrono::duration<double, std::milli> calculationTime;
    int numberOfEventsProcessed;
    Network network;
    std::unordered_map<int, Vehicle *> vehicles;
    int simulationEndTime;
    
    std::vector<Event*> events;

    void processNextEvent(Event* next_event);
    Event* findNextEvent();

public:
    SimulationStatus getStatus()
    {
        return status;
    }

    std::chrono::duration<double, std::milli> getCalculationTime()
    {
        if (status != SimulationStatus::COMPLETED)
        {
            std::cerr << "Simulation not complete" << std::endl;
        }
        return calculationTime;
    }

    void initializeAtNodeAtCapacity(int nodeID, int numberOfVehicles);
    std::vector<Event *> compileNodeEvents();

    //
    void run();

    std::string getInfos();
    void to_csv(std::string const filename);
};

#endif