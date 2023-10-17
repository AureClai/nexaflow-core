#ifndef EVENT_H
#define EVENT_H

#include <string>

class Link;
class Lane;
class Node;

class Event
{
public:
    Event(Node* node, int vehID, int regime, float time, Link* previousLink, Lane* previousLane, Link* nextLink, Lane* nextLane, std::string type) : node(node), vehID(vehID), regime(regime), time(time), previousLink(previousLink), nextLink(nextLink), nextLane(nextLane), previousLane(previousLane) {}

public:
    Node* node;
    int vehID;
    int regime;
    float time;
    Link* previousLink;
    Lane* previousLane;
    Link* nextLink;
    Lane* nextLane;
    std::string type;

    static bool CompareEventsByTime(const Event* event1, const Event* event2) {
        return event1->time < event2->time;
    }
};

#endif