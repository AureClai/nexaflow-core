#include "node.h"

std::string Event::getInfos()
{
    return "Vehicle " + std::to_string(vehID) + " at node " + std::to_string(node->id) + " at time " + std::to_string(time);
}