#ifndef CONNECTOR_H
#define CONNECTOR_H

class Centroid;
class Node;

class Connector
{
public:
    Connector(Centroid *centroid, Node *node) : m_centroid(centroid), m_nodeOut(node){};

private:
    Centroid *m_centroid;
    Node* m_nodeOut;
};

#endif