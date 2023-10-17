#include <iostream>
#include "link.h"
#include "fd.h"
#include "node.h"
#include <unordered_map>
#include <vector>
#include "network.h"
#include "simulation.h"
#include <algorithm>
#include <chrono>

int main()
{
    // chrono start
    auto startTime = std::chrono::high_resolution_clock::now();

    FD fd = FD(25.0f, 0.583334f, 0.14f, 5.0f);
    Link link = Link(0, "test", 1, 2, 0, 1, 0, 0.583334f, 25.0f, fd, 1000);
    Link link2 = Link(1, "test2", 1, 2, 1, 2, 0, 0.583334f, 25.0f, fd, 1000);

    std::unordered_map<int, Link *> links;
    links[0] = &link;
    links[1] = &link2;

    Node nodeUp = Node(0, "up_node", 1);
    Node nodeInter = Node(1, "inter_node", 0);
    Node nodeDown = Node(2, "down_node", 2);
    std::unordered_map<int, Node *> nodes;

    nodes[0] = &nodeUp;
    nodes[1] = &nodeInter;
    nodes[2] = &nodeDown;

    Network network = Network(nodes, links);

    network.makeConnections();

    //
    Simulation simulation = Simulation(network);
    std::vector<Event *> events = simulation.initializeAtNodeAtCapacity(0, 1000);

    std::sort(events.begin(), events.end(), Event::CompareEventsByTime);

    // Tackle next event
    int count = 0;
    while (events.size() != 0)
    {
        count += 1;
        Event *next_event = events.front();
        events.erase(events.begin());
        if (next_event->previousLink != nullptr)
        {
            next_event->previousLane->storeOutEvent(next_event);
        }
        if (next_event->node->type != 2)
        {
            Node* p_nextNode = simulation.network.nodes[next_event->nextLink->nodeDownID];
            Link *p_nextLink = nullptr;
            Lane *p_nextLane = nullptr;
            if (p_nextNode->type != 2)
            {
                p_nextLink = network.links[p_nextNode->OutgoingLinksID[0]];
                // find best lane... TODO
                p_nextLane = p_nextLink->lanes[0];
            }
            Link *p_prevLink = next_event->nextLink;
            Lane *p_prevLane = next_event->nextLane;
            float next_time = next_event->time + p_prevLink->length / p_prevLink->fd.u;

            Event *new_event = new Event(p_nextNode, next_event->vehID, 0, next_time, p_prevLink, p_prevLane, p_nextLink, p_nextLane, "vehicle event");
            events.push_back(new_event);
        }
    }

    // chrono end
    auto endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = endTime - startTime;

    std::cout << network.GetInfo() << std::endl;
    std::cout << "There are " << std::to_string(simulation.vehicles.size()) << " in the simulation" << std::endl;

    std::cout << "---" << std::endl;

    // for (const auto &event : simulation.compileNodeEvents())
    // {
    //     std::cout << "Node : " << std::to_string(event->nodeID) << " ID : " << std::to_string(event->vehID) << " Time : " << std::to_string(event->time) << std::endl;
    // }

    // Print the elapsed time
    std::cout << "Elapsed time: " << duration.count() << " milliseconds" << std::endl;

    std::cout << "---" << std::endl;
    std::string info = "Vehicles passed per lane--\n";
    for (auto &pair : network.links){
        info += "\t Link" + std::to_string(pair.first) + ":\n";
        for (auto &lanePair : pair.second->lanes){
            info += "\t\tLane " + std::to_string(lanePair.first) + ": " + std::to_string(lanePair.second->getPassedOut()) + " veh\n";
        }
    }
    std::cout << info << std::endl;

    return 0;
}