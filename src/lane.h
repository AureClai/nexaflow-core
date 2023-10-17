#ifndef LANE_H
#define LANE_H

#include "event.h"
#include <unordered_map>
#include <iostream>

class Link;

class Lane
{
public:
    Lane(Link* link) : link(link), passedIn(0), passedOut(0) {};
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
        outEvents[passedOut] = event;
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

    float getLastTravelTime(){
        return outEvents[passedOut-1]->time-outEvents[passedOut-2]->time;
    }
};

#endif
