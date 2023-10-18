#include "node.h"
#include <limits>

std::string Node::GetInfo() const
{
    std::string info = "Node Name: " + name + "\n";
    info += "ID: " + std::to_string(id) + "\n";
    // info += "Type: " + std::to_string(type) + "\n";
    info += "Number of Outgoing Links: " + std::to_string(numOutgoingLinks) + "\n";
    info += "Number of Incoming Links: " + std::to_string(numIncomingLinks) + "\n";

    info += "Outgoing Link IDs:\n";
    for (const Link *outgoingLink : outgoingLinks)
    {
        info += "  " + std::to_string(outgoingLink->id) + "\n";
    }

    info += "Incoming Link IDs:\n";
    for (const Link *incomingLink : incomingLinks)
    {
        info += "  " + std::to_string(incomingLink->id) + "\n";
    }

    return info;
}

void Node::connect_incoming_link(Link* link)
{
    incomingLinks.push_back(link);
    numIncomingLinks++;
}

void Node::connect_outgoing_link(Link * link)
{

    outgoingLinks.push_back(link);
    numOutgoingLinks++;
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
    for (const auto* link : incomingLinks){
        for (const auto &pair : link->lanes){
            nextArrivals[pair.second] = std::vector<Event *>();
        }
    }
    // Next supplies
    for (const auto* link : outgoingLinks){
        for (const auto &pair : link->lanes){
            nextSupplies[pair.second] = -std::numeric_limits<float>::infinity();
        }
    }
}