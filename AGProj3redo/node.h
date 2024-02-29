#ifndef NODE_H
#define NODE_H

#include <QPainter>

class Node
{
    int value;
    int cost;
    QPoint coordinate;
    bool isSelected;
    bool visited = false;
public:
    Node();
    Node(QPoint, int);
    Node(Node*);
    ~Node();

    void setValue(int value);
    void setCoordinate(QPoint);
    void setNode(Node* val);
    void setIsSelected(bool);
    void setCoordinate(int x, int y);

    QPoint getCoordinate();
    int getValue();
    int getX();
    int getY();

    static const int DimNode = 50;
    void setVisited(bool visited);

    bool getVisited();
    bool getIsSelected();
};

#endif // NODE_H
