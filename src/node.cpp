#include "node.h"
#include <limits>

std::string Node::GetInfo() const
{
    std::string info = "Node Name: " + name + "\n";
    info += "ID: " + std::to_string(id) + "\n";
    info += "Type: " + std::to_string(type) + "\n";
    info += "Number of Outgoing Links: " + std::to_string(NumOutgoingLinks) + "\n";
    info += "Number of Incoming Links: " + std::to_string(NumIncomingLinks) + "\n";

    info += "Outgoing Link IDs:\n";
    for (const Link *outgoingLink : OutgoingLinks)
    {
        info += "  " + std::to_string(outgoingLink->id) + "\n";
    }

    info += "Incoming Link IDs:\n";
    for (const Link *incomingLink : IncomingLinks)
    {
        info += "  " + std::to_string(incomingLink->id) + "\n";
    }

    return info;
}

void Node::connect_incoming_link(Link* link)
{
    IncomingLinks.push_back(link);
    NumIncomingLinks++;
}

void Node::connect_outgoing_link(Link * link)
{

    OutgoingLinks.push_back(link);
    NumOutgoingLinks++;
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

void Node::initialize(){
    // Next arrivals
    for (const auto* link : IncomingLinks){
        for (const auto &pair : link->lanes){
            next_arrivals[pair.second] = nullptr;
        }
    }
    // Next supplies
    for (const auto* link : OutgoingLinks){
        for (const auto &pair : link->lanes){
            next_supplies[pair.second] = -std::numeric_limits<float>::infinity();
        }
    }
}