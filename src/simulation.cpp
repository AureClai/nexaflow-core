#include "simulation.h"
#include "node.h"
#include "event.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <fstream>
#include <string>

void Simulation::initializeAtNodeAtCapacity(int nodeID, int numberOfVehicles)
{
    for (auto &pair : network.nodes)
    {
        pair.second->initialize();
    }

    Node *nextNode = network.nodes[nodeID];
    Link *nextLink = nextNode->outgoingLinks[0];
    Link *prevLink = nullptr;
    Lane *prevLane = nullptr;
    float capacity = nextLink->fd.C * nextLink->numLanes;
    float currTime = 0;
    int vehCount = 0;

    while (vehCount < numberOfVehicles)
    {
        Vehicle *new_vehicle = new Vehicle(0);
        Lane *nextLane = nextLink->lanes[vehCount % nextLink->numLanes];
        Event *new_event = new Event(nextNode, new_vehicle->getID(), 0, currTime, prevLink, prevLane, nextLink, nextLane, "vehicle_event");
        vehicles[new_vehicle->getID()] = new_vehicle;
        nextNode->nextArrivals[nullptr].push_back(new_event);
        // events.push_back(new_event);
        currTime += 1 / (capacity * 2);
        vehCount += 1;
    }

    std::sort(events.begin(), events.end(), Event::CompareEventsByTime);

    network.nodes[nodeID]->computeNextEvent();

    status = SimulationStatus::INITIALIZED;
}

std::vector<Event *> Simulation::compileNodeEvents()
{
    std::vector<Event *> output;
    for (const auto &pair : network.nodes)
    {
        for (const auto &passedEvents : pair.second->getPassedEvents())
        {
            output.push_back(passedEvents.second);
        }
    }
    std::sort(output.begin(), output.end(), Event::CompareEventsByTime);
    return output;
}

void Simulation::run()
{
    // chrono start
    auto startTime = std::chrono::high_resolution_clock::now();
    float currTime = 0;
    int numberOfEvents = 0;
    Event *nextEvent = findNextEvent();
    int securityCount = 0;
    while (currTime < simulationEndTime && securityCount < 1000000)
    {

        numberOfEvents += 1;
        processNextEvent(nextEvent);

        // New next event
        nextEvent = findNextEvent();
        if (nextEvent == nullptr)
        {
            std::cerr << "Premature stop of the simulation because of no more event";
            break;
        }
        currTime = nextEvent->time;
        //
    }

    // chrono end
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = endTime - startTime;

    calculationTime = duration;
    numberOfEventsProcessed = numberOfEvents;
    status = SimulationStatus::COMPLETED;
}

Event *Simulation::findNextEvent()
{
    float minTime = std::numeric_limits<float>::infinity();
    Event *next_event = nullptr;
    for (auto &pair : network.nodes)
    {
        auto nodeNextEvent = pair.second->nextEventLaneTime;
        if (nodeNextEvent.second < minTime)
        {
            minTime = nodeNextEvent.second;
            next_event = pair.second->nextArrivals[nodeNextEvent.first].front();
        }
    }
    if (next_event != nullptr)
    {
        next_event->time = minTime;
    }
    return next_event;
}

void Simulation::processNextEvent(Event *next_event)
{
    // std::cout << next_event->getInfos() <<std::endl;
    // Tackle next event
    // Event *next_event = events.front();
    next_event->node->storePassedEvent(next_event);
    // events.erase(events.begin());
    if (next_event->previousLink != nullptr)
    {
        next_event->previousLane->storeOutEvent(next_event);
    }
    if (next_event->node->numOutgoingLinks != 0)
    {
        next_event->nextLane->storeInEvent(next_event);
        Node *p_nextNode = next_event->nextLink->nodeDown;
        Link *p_nextLink = nullptr;
        Lane *p_nextLane = nullptr;
        if (p_nextNode->numOutgoingLinks != 0)
        {
            p_nextLink = p_nextNode->outgoingLinks[0];
            p_nextLane = p_nextLink->lanes[0];
            // find best lane... TODO
            std::vector<TurnMove *> authorizedTurnMoves;
            for (const auto &outLanePair : p_nextLink->lanes)
            {
                Lane *outLane = outLanePair.second;
                TurnMove *foundTurnMove = p_nextNode->findTurnMove(next_event->nextLane, outLane);
                if (foundTurnMove == nullptr)
                {
                    std::cerr << "Error : No turn move found" << std::endl;
                }
                if (foundTurnMove->isTurnAuthorized())
                {
                    authorizedTurnMoves.push_back(foundTurnMove);
                }
            }
            p_nextLane = authorizedTurnMoves.at(next_event->nextLink->getPassedIn() % authorizedTurnMoves.size())->getOutLane();
        }
        Link *p_prevLink = next_event->nextLink;
        Lane *p_prevLane = next_event->nextLane;
        float next_time = next_event->time + p_prevLink->length / p_prevLink->fd.u;

        Event *new_event = new Event(p_nextNode, next_event->vehID, 0, next_time, p_prevLink, p_prevLane, p_nextLink, p_nextLane, "vehicle event");
        p_nextNode->nextArrivals[p_prevLane].push_back(new_event);

        // Update of nodes
        p_nextNode->computeNextEvent();
        // events.push_back(new_event);
    }
    next_event->node->updateNextCapacityTime(next_event->nextLane, next_event->time);
    if (next_event->node->numOutgoingLinks != 0)
    {
        next_event->node->updateNextSupplyTime(next_event->nextLane, next_event->time);
    }

    if (next_event->node->id==1){
        std::cout << next_event->getInfos() << std::endl;
    }

    next_event->node->computeNextEvent();
}

std::string Simulation::getInfos()
{
    std::string info = "Simulation info :\n";

    // Declare the variables outside the switch
    long long milliseconds;
    long long seconds;
    long long minutes;
    long long hours;

    switch (status)
    {
    case SimulationStatus::COMPLETED:
        info += "\tStatus : COMPLETED\n";
        info += "\tVehicles generated :" + std::to_string(vehicles.size()) + "\n";
        info += "\tNumber of events: " + std::to_string(numberOfEventsProcessed) + "\n";

        // Calculate hours, minutes, seconds, and milliseconds
        milliseconds = calculationTime.count();
        seconds = milliseconds / 1000;
        minutes = seconds / 60;
        hours = minutes / 60;
        milliseconds %= 1000;
        seconds %= 60;
        minutes %= 60;

        // Create a formatted string for the time components
        char timeStr[50];
        snprintf(timeStr, sizeof(timeStr), "\tCalculation delay: %02lld:%02lld:%02lld.%03lld\n", hours, minutes, seconds, milliseconds);

        info += timeStr;

        info += "\tCalculation pace: " + std::to_string(double(numberOfEventsProcessed) / milliseconds * 1000.0) + " event/sec\n";
        break;

    case SimulationStatus::INITIALIZED:
        info += "\tStatus : INITIALIZED\n";
        info += "\tVehicles generated :" + std::to_string(vehicles.size()) + "\n";
        break;

    case SimulationStatus::CREATED:
        info += "\tStatus : CREATED\n";
        break;

    default:
        break;
    }

    return info;
}

void Simulation::to_csv(std::string const filename)
{
    // Data compilation
    std::vector<std::vector<std::string>> data;

    data.push_back({"nodeID", "vehID", "time"});

    for (auto const event : compileNodeEvents())
    {
        std::vector<std::string> currLine = {std::to_string(event->node->id), std::to_string(event->vehID), std::to_string(event->time)};
        data.push_back(currLine);
    }

    // Open the file*
    std::ofstream outputFile(filename);

    if (!outputFile.is_open())
    {
        std::cerr << "Failed to open the file." << std::endl;
    }

    for (const std::vector<std::string> &row : data)
    {
        for (size_t i = 0; i < row.size(); i++)
        {
            outputFile << row[i];
            if (i < row.size() - 1)
            {
                outputFile << ",";
            }
        }
        outputFile << "\n";
    }

    outputFile.close();

    std::cout << "Data saved to " << filename << std::endl;
}