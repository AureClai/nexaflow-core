#ifndef LINK_H
#define LINK_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "fd.h"
#include "lane.h"
#include <vector>
#include "node.h"
// #include <numpy/ndarrayobject.h>

class Link
{
public:
    // Constructor
    Link(const int id, const std::string &name, float priority, int numLanes, int road_type,
         float capacity, float speed, FD fd,
         float length) : id(id), name(name), priority(priority), numLanes(numLanes), nodeUp(nullptr), nodeDown(nullptr), road_type(road_type), capacity(capacity), speed(speed), fd(fd), length(length)
    {
        for (int i = 0; i < numLanes; i++)
        {
            lanes[i] = new Lane(this);
        }
    }

    ~Link()
    {
        for (auto &pair : lanes)
        {
            delete pair.second;
        }
    }

    // Member variables
    int id;
    std::string name;
    float priority;
    int numLanes;
    Node* nodeUp;
    Node* nodeDown;
    int road_type;
    float capacity;
    float speed;
    FD fd;
    float length;
    std::unordered_map<int,Lane *> lanes;

    // std::vector<float> points;

public:
    std::string GetInfo() const;
    std::vector<Event *> getOutEvents() const;
    std::vector<Event *> getInEvents() const;
    int getPassedOut() const;
    int getPassedIn() const;

    void connect(Node* nodeIn, Node* nodeOut);
};

#endif // LINK_H
