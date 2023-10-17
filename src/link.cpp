#include "link.h"

std::string Link::GetInfo() const
{
    std::string info = "Link Name: " + name + "\n";
    info += "ID: " + std::to_string(id) + "\n";
    info += "Priority: " + std::to_string(priority) + "\n";
    info += "Number of Lanes: " + std::to_string(numLanes) + "\n";
    info += "Node Up ID: " + std::to_string(nodeUpID) + "\n";
    info += "Node Down ID: " + std::to_string(nodeDownID) + "\n";
    info += "Road Type: " + std::to_string(road_type) + "\n";
    info += "Capacity: " + std::to_string(capacity) + "\n";
    info += "Speed: " + std::to_string(speed) + "\n";
    info += "FD Info:\n";
    info += "  Maximum Speed (u): " + std::to_string(fd.u) + "\n";
    info += "  Capacity (C): " + std::to_string(fd.C) + "\n";
    info += "  Maximum Density (kx): " + std::to_string(fd.kx) + "\n";
    info += "  Wave Speed (w): " + std::to_string(fd.w) + "\n";
    info += "Length: " + std::to_string(length) + "\n";
    return info;
}

std::vector<Event *> Link::getOutEvents() const
{
    std::vector<Event *> output;
    for (auto const &lanePair : lanes)
    {
        for (auto const &eventPair : lanePair.second->getOutEvents())
        {
            output.push_back(eventPair.second);
        }
    }

    return output;
}

std::vector<Event *> Link::getInEvents() const
{
    std::vector<Event *> output;
    for (auto const &lanePair : lanes)
    {
        for (auto const &eventPair : lanePair.second->getInEvents())
        {
            output.push_back(eventPair.second);
        }
    }

    return output;
}