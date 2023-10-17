#ifndef NETWORK_H
#define NETWORK_H

#include <unordered_map>
#include "node.h"
#include "link.h"

class Network
{
public:
    // Constructor
    Network(std::unordered_map<int, Node *> nodes, std::unordered_map<int, Link *> links) : nodes(nodes), links(links) {}

    // Member variables
    std::unordered_map<int, Node *> nodes;
    std::unordered_map<int, Link *> links;

public:
    std::string GetInfo() const;

    void makeConnections(std::unordered_map<int, std::pair<int, int>> connections);
};

#endif // NETWORK_H