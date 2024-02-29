#ifndef GRAPH_H
#define GRAPH_H
#include<set>
#include "arch.h"
#include "node.h"
#include <vector>
#include <QMouseEvent>
#include <fstream>
#include <stack>

class Graph
{
public:
    Graph();
    Graph(Graph& g);
    ~Graph();
    void addNode(Node* n);
    void addNode(QPoint p);
    void addArch(Arch*);
    void addArch(Node*, Node*);
    void setOrientedOrNot(bool);
    bool verifyArches(Arch* ed);
    

    std::vector<Node*> getNodes();
    std::vector<Arch*> getArches();
    std::vector<Node*> getNeighbors(Node* node);

    Node* GetNod(QMouseEvent* e);
    bool isOrientedOrNot();
    bool verifyNodes(QMouseEvent* e);
    void saveMatrixOriented();


    void resetVisitedNodes();
    bool DFS(Node* source, Node* sink, std::vector<Arch*>& path,std::vector<Arch*>& minCutEdges); 
    Arch* getArch(Node* source, Node* destination);
    bool isNodeReachable(Node* source, Node* node);
    
    std::vector<Arch*> getMins(); 
   
    std::vector<Arch*> minCutEdges;
private:
    std::vector<Node*> m_nodes; 
    std::vector<Arch*> m_arches;
    bool isOriented;

};
#endif // GRAPH_H
