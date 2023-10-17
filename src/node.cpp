#include "node.h"

std::string Node::GetInfo() const
{
    std::string info = "Node Name: " + name + "\n";
    info += "ID: " + std::to_string(id) + "\n";
    info += "Type: " + std::to_string(type) + "\n";
    info += "Number of Outgoing Links: " + std::to_string(NumOutgoingLinks) + "\n";
    info += "Number of Incoming Links: " + std::to_string(NumIncomingLinks) + "\n";

    info += "Outgoing Link IDs:\n";
    for (const int &outgoingLinkID : OutgoingLinksID)
    {
        info += "  " + std::to_string(outgoingLinkID) + "\n";
    }

    info += "Incoming Link IDs:\n";
    for (const int &incomingLinkID : IncomingLinksID)
    {
        info += "  " + std::to_string(incomingLinkID) + "\n";
    }

    return info;
}

void Node::connect_incoming_link(int link_id)
{
    IncomingLinksID.push_back(link_id);
    NumIncomingLinks += 1;
}

void Node::connect_outgoing_link(int link_id)
{

    OutgoingLinksID.push_back(link_id);
    NumOutgoingLinks += 1;
    std::cout << std::to_string(NumOutgoingLinks) << std::endl;
}

int Node::getPassedCount() const
{
    return passedCount;
}

std::unordered_map<int, Event *> Node::getPassedEvents() const
{
    return passedEvents;
}

void Node::storePassedEvent(Event *event)
{
    passedCount++;
    passedEvents[passedCount] = event;
}

void Node::addTurnMove(Lane *in, Lane *out, bool auth = true, float priority = 0.0f)
{
    TurnMove tm(in, out, auth, priority);
    turnMovesMap[std::make_pair(in, out)] = tm;
}

TurnMove *Node::findTurnMove(Lane *in, Lane *out)
{
    auto it = turnMovesMap.find(std::make_pair(in, out));
    if (it != turnMovesMap.end())
    {
        return &(it->second);
    }
    return nullptr;
}