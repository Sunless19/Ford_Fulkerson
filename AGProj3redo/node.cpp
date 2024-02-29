#include "node.h"

Node::Node()
    :value{ -1 }
    , isSelected{ false }
{}

Node::Node(QPoint c, int v)
    :coordinate{ c }
    , value{ v }
    , isSelected{ false }
{}

Node::Node(Node* n)
    :coordinate{ n->getCoordinate() }
    , value{ n->getValue() + 1 }
    , isSelected{ false }
{}

Node::~Node()
{}

void Node::setValue(int value)
{
    this->value = value;
}

void Node::setCoordinate(QPoint p)
{
    coordinate = p;
}

void Node::setNode(Node* val)
{
    setCoordinate(val->getCoordinate());
    setValue(val->getValue());
}

void Node::setIsSelected(bool is)
{
    isSelected = is;
}

void Node::setCoordinate(int x, int y)
{
}

int Node::getValue()
{
    return value;
}

QPoint Node::getCoordinate()
{
    return coordinate;
}

int Node::getX()
{
    return coordinate.x();
}

int Node::getY()
{
    return coordinate.y();
}

bool Node::getIsSelected()
{
    return isSelected;
}

void Node::setVisited(bool visited)
{
    this->visited = visited;
}

bool Node::getVisited()
{
    return this->visited;
}