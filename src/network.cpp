#include "network.h"

std::string Network::GetInfo() const
{
    std::string info = "Network info \n";

    // Print information about links
    for (const auto &pair : links)
    {
        int linkID = pair.first;
        Link *link = pair.second;
        info += "Link ID: " + std::to_string(linkID) + "\n";
        info += link->GetInfo() + "\n";
    }

    for (const auto &pair : nodes)
    {
        int nodeID = pair.first;
        Node *node = pair.second;
        info += "Node ID: " + std::to_string(nodeID) + "\n";
        info += node->GetInfo() + "\n";
    }

    return info;
}

void Network::makeConnections(std::unordered_map<int, std::pair<int, int>> connections)
{
    for (const auto &pair : connections)
    {
        Link *link = links[pair.first];
        Node* nodeUp = nodes[pair.second.first];
        Node* nodeDown = nodes[pair.second.second];

        link->connect(nodeUp, nodeDown);
    }
}