#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "link.h"
#include "event.h"
#include "turnMoves.h"

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1,T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Combine the hashes
        return h1 ^h2;
    }
};

class Node
{
public:
    // Constructor
    Node(const int id, const std::string &name, int type) : id(id), name(name), type(type)
    {
        NumOutgoingLinks = 0;
        NumIncomingLinks = 0;
    };

    // Member variables
    int id;
    std::string name;
    int type;
    std::vector<int> OutgoingLinksID;
    std::vector<int> IncomingLinksID;
    int NumOutgoingLinks;
    int NumIncomingLinks;
    std::unordered_map<std::pair<Lane *, Lane *>, TurnMove, pair_hash> turnMovesMap;

private:
    std::unordered_map<int, Event *> passedEvents;
    int passedCount;

public:
    void connect_outgoing_link(int link_id);
    void connect_incoming_link(int link_id);
    std::string GetInfo() const;
    int getPassedCount() const;
    std::unordered_map<int, Event *> getPassedEvents() const;
    void storePassedEvent(Event * event);

    void addTurnMove(Lane * in, Lane * out, bool auth, float priority);
    TurnMove* findTurnMove(Lane* in, Lane* out);
};

#endif // NODE_H
