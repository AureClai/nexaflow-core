#ifndef TURNMOVE_H
#define TURNMOVE_H

#include "lane.h"

class TurnMove {
private:
    Lane* inLane;       // pointer to the incoming lane
    Lane* outLane;      // pointer to the outgoing lane
    bool isAuthorized;  // true if the turn is authorized, false if forbidden
    float priorityShare; // priority share for the turn

public:
    // Constructor
    TurnMove(Lane* in, Lane* out, bool auth = true, float priority = 1.0f)
        : inLane(in), outLane(out), isAuthorized(auth), priorityShare(priority) {}
    TurnMove() : inLane(nullptr), outLane(nullptr), isAuthorized(true), priorityShare(1.0f) {}


    // Toggle the turn authorization status
    void toggleAuthorization() {
        isAuthorized = !isAuthorized;
    }

    // Set priority share
    void setPriorityShare(float priority) {
        priorityShare = priority;
    }

    // Get priority share
    float getPriorityShare() const {
        return priorityShare;
    }

    // Check if the turn is authorized
    bool isTurnAuthorized() const {
        return isAuthorized;
    }

    Lane* getInLane() const {
        return inLane;
    }

    Lane* getOutLane() const {
        return outLane;
    }

    std::string getInfos() const {
        std::string info = std::to_string(isAuthorized);
        return info;
    }
};

#endif
