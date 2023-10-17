#include "simulation.h"
#include "node.h"
#include "event.h"
#include <iostream>
#include <algorithm>

std::vector<Event *> Simulation::initializeAtNodeAtCapacity(int nodeID, int numberOfVehicles)
{
    std::vector<Event *> events;

    Node* nextNode = network.nodes[nodeID];
    Link* nextLink = network.links[nextNode->OutgoingLinksID[0]];
    Link* prevLink = nullptr;
    Lane* prevLane = nullptr;
    float capacity = nextLink->fd.C;
    float currTime = 0;
    int vehCount = 0;

    while (vehCount < numberOfVehicles)
    {
        Vehicle *new_vehicle = new Vehicle(0);
        Lane* nextLane = nextLink->lanes[vehCount % nextLink->numLanes];
        Event *new_event = new Event(nextNode, new_vehicle->GetID(), 0, currTime, prevLink, prevLane, nextLink, nextLane, "vehicle_event");
        vehicles[new_vehicle->GetID()] = new_vehicle;
        events.push_back(new_event);
        currTime += 1 / capacity;
        vehCount += 1;
    }

    return events;
}

std::vector<Event *> Simulation::compileNodeEvents(){
    std::vector<Event *> output;
    for (const auto &pair : network.nodes)
    {
        for (const auto &passedEvents : pair.second->getPassedEvents()){
            output.push_back(passedEvents.second);
        }
    }
    std::sort(output.begin(), output.end(), Event::CompareEventsByTime);
    return output;
}