#include "graph.h"
#include <QFile>
Graph::Graph()
	:isOriented(true)
{
}

Graph::Graph(Graph& g)
	:isOriented(true)
{
	for (Node* n : g.m_nodes)
	{
		addNode(n);
	}
	for (Arch* ed : g.m_arches)
	{
		Node* selectFirstNode{ nullptr }, * selectSecondNode{ nullptr }, * firstNode{ ed->getFirstNode() }, * secondNode{ ed->getSecondNode() };
		for (Node* n : g.m_nodes)
		{
			if (n->getValue() == firstNode->getValue())
				selectFirstNode = n;
			else
				if (n->getValue() == secondNode->getValue())
					selectSecondNode = n;
			if (selectFirstNode && selectSecondNode)
			{
				addArch(new Arch{ selectFirstNode ,selectSecondNode });
				break;
			}
		}
	}
}

Graph::~Graph()
{
	for (int i = 0; i < m_arches.size(); i++)
		delete m_arches[i];

	for (int i = 0; i < m_nodes.size(); i++)
		delete m_nodes[i];
}

void Graph::addNode(Node* n1)
{
	Node* n = new Node(n1);
	m_nodes.push_back(n);
}

void Graph::addNode(QPoint p)
{
	Node* n = new Node;
	n->setCoordinate(p);
	n->setValue(m_nodes.size() + 1);
	m_nodes.push_back(n);
}

void Graph::addArch(Arch* arch)
{

	Arch* ed = new Arch(arch->getFirstNode(), arch->getSecondNode());
	ed->setCost(arch->getCost());
	m_arches.push_back(ed);
}

void Graph::addArch(Node* firstNode, Node* secondNode)
{
	Arch* arch = new Arch(firstNode, secondNode);
	m_arches.push_back(arch);
}

void Graph::setOrientedOrNot(bool tOrF)
{
	isOriented = tOrF;
}

bool Graph::verifyArches(Arch* ed)
{
	{
		for (Arch* E : m_arches)
		{
			if (E->getFirstNode()->getCoordinate() == ed->getFirstNode()->getCoordinate() && E->getSecondNode()->getCoordinate() == ed->getSecondNode()->getCoordinate())
				return false;

			if (!isOrientedOrNot())
				if (E->getSecondNode()->getCoordinate() == ed->getFirstNode()->getCoordinate() && E->getFirstNode()->getCoordinate() == ed->getSecondNode()->getCoordinate())
					return false;
		}
		return true;
	}
}

std::vector<Node*> Graph::getNodes()
{
	return m_nodes;
}

std::vector<Arch*> Graph::getArches()
{
	return m_arches;
}

std::vector<Arch*> Graph::getMins()
{
	return minCutEdges;
}

std::vector<Node*> Graph::getNeighbors(Node* node)
{
	std::vector<Node*> neighbors;

	for (Arch* ed : m_arches)
	{
		if (ed->getFirstNode() == node)
		{
			neighbors.push_back(ed->getSecondNode());
		}
		else if (ed->getSecondNode() == node && !isOriented)
		{
			neighbors.push_back(ed->getFirstNode());
		}
	}

	return neighbors;
}

Node* Graph::GetNod(QMouseEvent* e)
{
	for (Node* n : m_nodes)
	{
		if (fabs(n->getX() - e->pos().x()) < Node::DimNode &&
			fabs(n->getY() - e->pos().y()) < Node::DimNode)
			return n;
	}
	return nullptr;
}

bool Graph::isOrientedOrNot()
{
	return isOriented;
}

bool Graph::verifyNodes(QMouseEvent* e)
{
	for (Node* n : m_nodes)
	{
		if (!n->getIsSelected())
			if (fabs(n->getX() - e->pos().x()) < Node::DimNode &&
				fabs(n->getY() - e->pos().y()) < Node::DimNode)
				return false;
	}
	return true;
}

void Graph::saveMatrixOriented()
{
	std::ofstream fout("matrice.txt");

	fout << m_nodes.size() << "\n";
	for (int i = 1; i <= m_nodes.size(); i++)
	{
		fout << i << ":";
		for (Arch* ed : m_arches)
		{
			if (ed->getFirstNode()->getValue() == i)
				fout << ed->getSecondNode()->getValue() << ", ";
		}
		fout << "\n";
	}
	fout.close();
}

bool Graph::DFS(Node* source, Node* sink, std::vector<Arch*>& path, std::vector<Arch*>& minCutEdges)
{
	resetVisitedNodes();
	std::stack<Node*> stack;
	stack.push(source);

	// Se folosește un map pentru a stoca drumul parcurs și ultimul arc utilizat
	std::map<Node*, Arch*> cameFrom;

	while (!stack.empty())
	{
		Node* current = stack.top();
		stack.pop(); 
		current->setVisited(true); 

		std::vector<Node*> neighbors = getNeighbors(current); 
		for (Node* neighbor : neighbors) 
		{
			Arch* ed = getArch(current, neighbor); 
			if (ed && !neighbor->getVisited() && ed->getCost() > 0) 
			{
				stack.push(neighbor); 
				cameFrom[neighbor] = ed; 

				if (neighbor == sink) 
				{
					// Reconstruim drumul de la destinație la sursă folosind informațiile stocate în cameFrom
					Node* node = sink; 
					while (node != source) 
					{
						path.push_back(cameFrom[node]);
						node = cameFrom[node]->getFirstNode();
					}

					// Drumul a fost găsit
					std::reverse(path.begin(), path.end());
					return true;
				}
			}
		}
	}

	return false;
}

void Graph::resetVisitedNodes()
{
	for (Node* n : m_nodes) 
		n->setVisited(false); 
}

Arch* Graph::getArch(Node* source, Node* destination) 
{
	for (Arch* ed : m_arches)
	{ 
		if (ed->getFirstNode() == source && ed->getSecondNode() == destination) 
			return ed;
	}
	return nullptr;
}

bool Graph::isNodeReachable(Node* source, Node* node)
{
	resetVisitedNodes();

	std::stack<Node*> stack;
	stack.push(source);

	while (!stack.empty())
	{
		Node* current = stack.top();
		stack.pop();
		current->setVisited(true);

		std::vector<Node*> neighbors = getNeighbors(current);
		for (Node* neighbor : neighbors)
		{
			Arch* ed = getArch(current, neighbor);
			if (ed && !neighbor->getVisited() && ed->getCost() > 0)
			{
				stack.push(neighbor);

				if (neighbor == node)
					return true;
			}
		}
	}

	return false;
}