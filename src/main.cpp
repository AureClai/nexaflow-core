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
#include "turnMoves.h"

int main()
{
    FD fd = FD(25.0f, 0.583334f, 0.14f, 5.0f);
    FD fd2 = FD(25.0f, 0.025f, 0.14f, 5.0f);
    Link link = Link(0, "test", 1., 2, 0, 0.583334f, 25.0f, fd, 1000);
    Link link2 = Link(1, "test2", 1., 2, 0, 0.583334f, 25.0f, fd2, 1000);
    // Link link3 = Link(2, "test3", 1., 1, 0, 0.583334f, 25.0f, fd, 1000);

    std::unordered_map<int, Link *> links;
    links[0] = &link;
    links[1] = &link2;
    // links[2] = &link3;

    Node nodeUp = Node(0, "up_node");
    Node nodeInter = Node(1, "inter_node");
    // Node nodeInter2 = Node(2, "inter_node");
    Node nodeDown = Node(3, "down_node");

    std::unordered_map<int, Node *> nodes;

    nodes[0] = &nodeUp;
    nodes[1] = &nodeInter;
    // nodes[2] = &nodeInter2;
    nodes[3] = &nodeDown;

    Network network = Network(nodes, links);

    std::unordered_map<
    int, std::pair<int, int>> connections = {
        {0, {0, 1}},
        {1, {1, 3}}
        // {2, {2, 3}}
        };
    network.makeConnections(connections);

    // Turn Moves
    for (auto &nodePair : nodes)
    {
        Node *node = nodePair.second;
        if (node->numIncomingLinks != 0 && node->numOutgoingLinks != 0)
        {
            for (auto *inlink : node->incomingLinks)
            {
                for (auto &inLanePair : inlink->lanes)
                {
                    for (auto *outLink : node->outgoingLinks)
                    {
                        for (auto &outLanePair : outLink->lanes)
                        {
                            node->addTurnMove(inLanePair.second, outLanePair.second, true, 1.0f);
                        }
                    }
                }
            }
        }
    }
    // Modif turn move
    // nodes[1]->findTurnMove(links[0]->lanes[0], links[1]->lanes[1])->toggleAuthorization();
    // nodes[1]->findTurnMove(links[0]->lanes[1], links[1]->lanes[1])->toggleAuthorization();

    // std::cout << network.GetInfo() << std::endl;

    //
    Simulation simulation = Simulation(network, 7200);
    std::cout << simulation.getInfos() << std::endl;
    simulation.initializeAtNodeAtCapacity(0, 4000);

    std::cout << simulation.getInfos() << std::endl;

    
    // std::cout << "There are " << std::to_string(simulation.vehicles.size()) << " in the simulation" << std::endl;

    std::cout << "---" << std::endl;

    for (const auto &event : simulation.compileNodeEvents())
    {
        std::cout << "Node : " << std::to_string(event->node->id) << " ID : " << std::to_string(event->vehID) << " Time : " << std::to_string(event->time) << std::endl;
    }

    simulation.run();
    std::cout << simulation.getInfos() << std::endl;

    std::cout << "---" << std::endl;
    std::string info = "Vehicles passed per lane--\n";
    for (auto &pair : network.links)
    {
        info += "\t Link" + std::to_string(pair.first) + ":\n";
        for (auto &lanePair : pair.second->lanes)
        {
            info += "\t\tLane " + std::to_string(lanePair.first) + ": " + std::to_string(lanePair.second->getPassedIn()) + "veh in " + std::to_string(lanePair.second->getPassedOut()) + " veh out\n";
        }
    }
    std::cout << info << std::endl;

    simulation.to_csv("not_tracked/results.csv");

    return 0;
}