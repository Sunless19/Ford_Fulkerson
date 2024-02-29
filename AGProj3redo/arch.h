#ifndef ARCH_H
#define ARCH_H

#include "node.h"

class Arch
{
private:
    Node* firstNode, *secondNode;
    int cost;
public:
    Arch();
    Arch(Node* , Node* );
    ~Arch();
    Node* getFirstNode();
    Node* getSecondNode();
    void setCost(int);
    int getCost();
};

#endif // ARCH_H
