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

void Network::makeConnections()
{
    for (const auto &pair : links)
    {
        int linkID = pair.first;
        Link *link = pair.second;
        int nodeUpId = link->nodeUpID;
        int nodeDownId = link->nodeDownID;

        nodes[nodeUpId]->connect_outgoing_link(linkID);
        nodes[nodeDownId]->connect_incoming_link(linkID);
    }
}