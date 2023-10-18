#include "simulation.h"
#include "node.h"
#include "event.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

void Simulation::initializeAtNodeAtCapacity(int nodeID, int numberOfVehicles)
{
    Node *nextNode = network.nodes[nodeID];
    Link *nextLink = nextNode->outgoingLinks[0];
    Link *prevLink = nullptr;
    Lane *prevLane = nullptr;
    float capacity = nextLink->fd.C;
    float currTime = 0;
    int vehCount = 0;

    while (vehCount < numberOfVehicles)
    {
        Vehicle *new_vehicle = new Vehicle(0);
        Lane *nextLane = nextLink->lanes[vehCount % nextLink->numLanes];
        Event *new_event = new Event(nextNode, new_vehicle->getID(), 0, currTime, prevLink, prevLane, nextLink, nextLane, "vehicle_event");
        vehicles[new_vehicle->getID()] = new_vehicle;
        events.push_back(new_event);
        currTime += 1 / capacity;
        vehCount += 1;
    }

    std::sort(events.begin(), events.end(), Event::CompareEventsByTime);

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
    int numberOfEvents = 0;
    while (events.size() != 0)
    {
        numberOfEvents += 1;
        processNextEvent();
    }

    // chrono end
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = endTime - startTime;

    calculationTime = duration;
    numberOfEventsProcessed = numberOfEvents;
    status = SimulationStatus::COMPLETED;
}

void Simulation::processNextEvent()
{
    // Tackle next event
    Event *next_event = events.front();
    next_event->node->storePassedEvent(next_event);
    events.erase(events.begin());
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
        events.push_back(new_event);
    }
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

        info += "\tCalculation pace: " + std::to_string(double(numberOfEventsProcessed)/milliseconds * 1000.0) + " event/sec\n";
        break;

    case SimulationStatus::INITIALIZED:
        info += "\tStatus : INITIALIZED\n";
        info += "\tVehicles generated :" + std::to_string(vehicles.size())+ "\n";
        break;

    case SimulationStatus::CREATED:
        info += "\tStatus : CREATED\n";
        break;

    default:
        break;
    }

    return info;
}