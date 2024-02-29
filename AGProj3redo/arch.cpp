#include "arch.h"

Arch::Arch() 
	:firstNode{nullptr},
	secondNode{nullptr}
	, cost(0)
{
}

Arch::Arch(Node* n1, Node* n2)
{
	firstNode = n1;
	secondNode = n2;
	cost = 0;
}

Arch::~Arch()
{
}

Node* Arch::getFirstNode()
{
	return firstNode;
}

Node* Arch::getSecondNode()
{
	return secondNode;
}

void Arch::setCost(int costul)
{
	this->cost = costul;
}

int Arch::getCost()
{
	return this->cost;
}