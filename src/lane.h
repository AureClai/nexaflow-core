#ifndef LANE_H
#define LANE_H

#include "event.h"
#include <unordered_map>

class Link;

class Lane
{
public:
    Lane(Link* link) : link(link) {};
    ~Lane(){};

private:
    Link *link;
    int passedIn;
    int passedOut;
    std::unordered_map<int, Event *> inEvents;
    std::unordered_map<int, Event *> outEvents;

public:
    int getPassedIn() const
    {
        return passedIn;
    }

    int getPassedOut() const
    {
        return passedOut;
    }

    void storeInEvent(Event *event)
    {
        passedIn+=1;
        inEvents[passedIn] = event;
    }

    void storeOutEvent(Event *event)
    {
        passedOut+=1;
        inEvents[passedOut] = event;
    }

    std::unordered_map<int, Event *> getInEvents() const
    {
        return inEvents;
    }

    std::unordered_map<int, Event *> getOutEvents() const
    {
        return outEvents;
    }

    Link* getLink() const {
        return link;
    }
};

#endif
