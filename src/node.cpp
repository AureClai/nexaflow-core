#include "node.h"
#include <limits>
#include <cmath>

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

void Node::connect_incoming_link(Link *link)
{
    incomingLinks.push_back(link);
    numIncomingLinks++;
}

void Node::connect_outgoing_link(Link *link)
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
    nextArrivals[event->previousLane].erase(nextArrivals[event->previousLane].begin());
    if (nextArrivals.size() == 0)
    {
        nextEventLaneTime.first = nullptr;
        nextEventLaneTime.second = std::numeric_limits<float>().infinity();
    }
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

void Node::initialize()
{
    // Next arrivals
    if (numIncomingLinks == 0)
    {
        nextArrivals[nullptr] = std::vector<Event *>();
    }
    else
    {
        for (const auto *link : incomingLinks)
        {
            for (const auto &pair : link->lanes)
            {
                nextArrivals[pair.second] = std::vector<Event *>();
            }
        }
    }
    // Next supplies
    for (const auto *link : outgoingLinks)
    {
        for (const auto &pair : link->lanes)
        {
            nextCapacityTimes[pair.second] = -std::numeric_limits<float>::infinity();
            nextSupplyTimes[pair.second] = -std::numeric_limits<float>::infinity();
        }
    }
    if (numOutgoingLinks == 0)
    {
        nextCapacityTimes[nullptr] = -std::numeric_limits<float>::infinity();
        nextSupplyTimes[nullptr] = -std::numeric_limits<float>::infinity();
    }

    nextEventLaneTime.first = nullptr;
    nextEventLaneTime.second = std::numeric_limits<float>().infinity();
}

void Node::updateNextCapacityTime(Lane *lane, float time)
{

    if (lane != nullptr)
    {
        nextCapacityTimes[lane] = time + 1 / lane->getLink()->fd.C;
    }
    else
    {
        nextCapacityTimes[lane] = -std::numeric_limits<float>().infinity();
    }
    return;
}

void Node::updateNextSupplyTime(Lane *lane, float time)
{
    float delta_n = std::floor(lane->getLink()->fd.kx * lane->getLink()->length);
    float alpha = lane->getLink()->fd.kx * lane->getLink()->length - delta_n;
    // std::cout << std::to_string(lane->getPassedIn() - lane->getPassedOut()) << std::endl;
    if (lane->getPassedIn() - delta_n - 1 > 0)
    {
        if (lane->getPassedIn() - delta_n - 1 < lane->getPassedOut())
        {
            float nextSupplyTime = (1 - alpha) * lane->getOutEvents()[static_cast<int>(lane->getPassedIn() - (delta_n))]->time + alpha * lane->getOutEvents()[static_cast<int>(lane->getPassedIn() - (delta_n + 1))]->time + lane->getLink()->length / lane->getLink()->fd.w;
            nextSupplyTimes[lane] = nextSupplyTime;
        }
        else
        {
            nextSupplyTimes[lane] = +std::numeric_limits<float>().infinity();
        }
    }
    else
    {
        nextSupplyTimes[lane] = -std::numeric_limits<float>().infinity();
    }
    return;
}

Lane* Node::choseNextLane(Event * nextArrivalFromLane){
    // Chose from the best time and chose the right most
    float travelTime = std::numeric_limits<float>().infinity();
    Lane* lane = nullptr;
    int laneID = -1;
    for (auto const &lanePair: nextArrivalFromLane->nextLink->lanes){
        float lastTT = lanePair.second->getLastTravelTime();
        if (lastTT == -1){
            Link * link = lanePair.second->getLink();
            lastTT = link->length / link->fd.u;
        }
        if (lastTT <= travelTime){
            lane = lanePair.second;
            laneID = lanePair.first;
            // std::cout << std::to_string(laneID) << std::endl;
            travelTime = lastTT;
        }
    }

    return lane;
}

void Node::computeNextEvent()
{
    std::unordered_map<Lane *, float> candidates;
    for (const auto &pair : nextArrivals)
    {
        Lane *inLane = pair.first;
        if (pair.second.size() == 0)
        {
            candidates[inLane] = std::numeric_limits<float>().infinity();
        }
        else
        {
            Event *nextArrivalFromLane = pair.second.front();
            Lane *outLane = nullptr;
            if (nextArrivalFromLane->nextLink!=nullptr){
                outLane = choseNextLane(nextArrivalFromLane);
            }
            // Lane *outLane = pair.second.front()->nextLane;
            float nextSupply = nextSupplyTimes[outLane];
            float nextCapacity = nextCapacityTimes[outLane];
            float nextDown = std::max<float>(nextSupply, nextCapacity);
            candidates[inLane] = std::max<float>(nextArrivalFromLane->time, nextDown);
            if (candidates[inLane]==nextDown){
                nextArrivalFromLane->regime = 1;
            }
        }
    }
    // Compute the real next
    float nextTime = std::numeric_limits<float>().infinity();
    Lane *currLane = nullptr;
    for (const auto &pair : candidates)
    {
        if (pair.second < nextTime)
        {
            nextTime = pair.second;
            currLane = pair.first;
        }
    }
    nextEventLaneTime.first = currLane;
    nextEventLaneTime.second = nextTime;
}