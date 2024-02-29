#pragma once
#include <qmainwindow.h>
#include "node.h"
#include "graph.h"
#include<vector>
class PopUpWindow : public QMainWindow
{
	Q_OBJECT
public:
	PopUpWindow(std::vector<Node*> nodes, std::vector<Arch*> edges,std::vector<Arch*> colorEdges, QWidget* parent = nullptr);
	~PopUpWindow();
	void paintEvent(QPaintEvent* e);
private:
	std::vector<Node*> m_nodes;
	std::vector<Arch*> m_arches;
};

