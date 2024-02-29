#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QInputDialog>


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, secondNode(nullptr)
	, firstNode(nullptr)
	, MousePressed(false)

{
	ui->setupUi(this);
	connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::on_FordFulkerson);
	/*connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_colorEdges);*/
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::RightButton)
	{
		if (graf.verifyNodes(e))
			graf.addNode(e->pos());
		else
		{
			for (Node* n : graf.getNodes())
			{
				if (fabs(n->getX() - e->pos().x()) < Node::DimNode / 2.0 &&
					fabs(n->getY() - e->pos().y()) < Node::DimNode / 2.0)
					if (firstNode == nullptr)
					{
						firstNode = n;
						firstNode->setIsSelected(true);
					}
					else
					{
						secondNode = n;
						if (firstNode->getValue() != secondNode->getValue())
						{
							Arch* ed = new Arch(firstNode, secondNode);
							if (graf.verifyArches(ed))
							{
								bool ok;
								QString text = QInputDialog::getText(nullptr, "Get Number", "Enter a number:", QLineEdit::Normal, "", &ok);
								int number;
								if (ok && !text.isEmpty()) {
									bool conversionOk = false;
									number = text.toInt(&conversionOk);

									if (conversionOk) {
										qDebug() << "Entered number:" << number;
									}
									else {
										qDebug() << "Invalid number format entered.";
									}
								}
								else {
									qDebug() << "User canceled the input.";
								}
								ed->setCost(number);
								graf.addArch(ed);
							}
							else
								delete ed;
						}
						firstNode->setIsSelected(false);
						firstNode = nullptr;
						secondNode = nullptr;
					}
			}
		}
	}
	else
		if (e->button() == Qt::LeftButton)
		{
			for (Node* n : graf.getNodes())
			{
				if (fabs(n->getX() - e->pos().x()) < Node::DimNode / 2.0 &&
					fabs(n->getY() - e->pos().y()) < Node::DimNode / 2.0)
				{
					n->setIsSelected(true);
					MousePressed = true;
					break;
				}
			}
		}
	update();
}

void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
	if (MousePressed)
		if (Qt::LeftButton)
		{
			for (Node* n : graf.getNodes())
				if (n->getIsSelected())
				{
					if (graf.verifyNodes(e))
						n->setCoordinate(e->pos());
				}
			update();
		}
}

void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		MousePressed = false;
		for (Node* n : graf.getNodes())
		{
			n->setIsSelected(false);
		}
	}
}

void MainWindow::paintNode(QPainter& p, Node* n)
{
	if (n->getIsSelected())
		p.setPen(Qt::red);
	else
		p.setPen(Qt::black);

	QRect r(n->getX() - Node::DimNode / 2, n->getY() - Node::DimNode / 2, Node::DimNode, Node::DimNode);
	p.drawEllipse(r);
	QString s;
	s.setNum(n->getValue());
	p.drawText(r, Qt::AlignCenter, s);
}

void MainWindow::on_FordFulkerson()
{
	Node* source = graf.getNodes()[0];
	Node* sink = graf.getNodes()[graf.getNodes().size()-1];

	// Se inițializează fluxul maxim
	int maxFlow = 0;

	// Se inițializează un vector de arce pentru a stoca drumurile găsite
	std::vector<Arch*> path;
	std::vector<Arch*> mins;

	// Se caută drumuri de creștere a fluxului folosind DFS până când nu mai există astfel de drumuri
	while (graf.DFS(source, sink, path, mins))
	{
		// Se găsește capacitatea minimă pe drumul găsit
		int minCapacity = INT_MAX; 
		for (Arch* ed : path) 
		{
			if (ed->getCost() <= minCapacity) 
				minCapacity = ed->getCost();
		}

		for (Arch* ed : path)
		{
			if (ed->getCost() == minCapacity)
				graf.minCutEdges.push_back(ed);
		}

		// Se actualizează capacitatea arcelor implicate și se actualizează fluxul maxim
		for (Arch* ed : path) 
		{
			ed->setCost(ed->getCost() - minCapacity); 
		}

		// Se resetează drumul și se adaugă la fluxul maxim
		path.clear();
		maxFlow += minCapacity;
	}

	// Se afișează fluxul maxim
	qDebug() << "Fluxul maxim este: " << maxFlow;

}

//void MainWindow::on_colorEdges()
//{
//	PopUpWindow* b =new PopUpWindow(graf.getNodes(), graf.getNodes(), graf.getArches(), nullptr);
//	b[0].resize(1100, 1100);
//	b[0].show();
//}

void MainWindow::ClickedButton()
{
	graf.setOrientedOrNot(false);

}

void MainWindow::drawLine(Node* N1, Node* N2, int radius, QPainter& p, int cost)
{
	p.setPen(Qt::black);
	qreal dx = N2->getX() - N1->getX();
	qreal dy = N2->getY() - N1->getY();
	qreal distance = qSqrt(dx * dx + dy * dy);
	qreal x1_intersec = N1->getX() + (dx * radius / distance);
	qreal y1_intersec = N1->getY() + (dy * radius / distance);
	qreal x2_intersec = N2->getX() - (dx * radius / distance);
	qreal y2_intersec = N2->getY() - (dy * radius / distance);

	p.drawLine(x1_intersec, y1_intersec, x2_intersec, y2_intersec);
	drawArrow(QPoint(x1_intersec, y1_intersec), QPoint(x2_intersec, y2_intersec), p); 

	qreal midX = (x1_intersec + x2_intersec) / 2;
	qreal midY = (y1_intersec + y2_intersec) / 2;

	// Desenează un dreptunghi la mijlocul liniei
	qreal rectangleWidth = 35;
	qreal rectangleHeight = 35;
	qreal rectX = midX - rectangleWidth / 2;
	qreal rectY = midY - rectangleHeight / 2;
	p.setBrush(Qt::blue);
	QRect r(rectX, rectY, rectangleWidth, rectangleHeight);
	p.drawRect(r);
	QString s;
	s.setNum(cost);
	p.drawText(r, Qt::AlignCenter, s);

}



void MainWindow::drawLineColor(Node* N1, Node* N2, int radius, QPainter& p, QColor x,int cost)
{
	p.setPen(x);
	qreal dx = N2->getX() - N1->getX();
	qreal dy = N2->getY() - N1->getY();
	qreal distance = qSqrt(dx * dx + dy * dy);
	qreal x1_intersec = N1->getX() + (dx * radius / distance);
	qreal y1_intersec = N1->getY() + (dy * radius / distance);
	qreal x2_intersec = N2->getX() - (dx * radius / distance);
	qreal y2_intersec = N2->getY() - (dy * radius / distance);

	p.drawLine(x1_intersec, y1_intersec, x2_intersec, y2_intersec);
	drawArrow(QPoint(x1_intersec, y1_intersec), QPoint(x2_intersec, y2_intersec), p); 

	qreal midX = (x1_intersec + x2_intersec) / 2;
	qreal midY = (y1_intersec + y2_intersec) / 2;

	// Desenează un dreptunghi la mijlocul liniei
	qreal rectangleWidth = 35;
	qreal rectangleHeight = 35;
	qreal rectX = midX - rectangleWidth / 2;
	qreal rectY = midY - rectangleHeight / 2;
	p.setBrush(Qt::green);
	QRect r(rectX, rectY, rectangleWidth, rectangleHeight);
	p.drawRect(r); 
	QString s; 
	s.setNum(cost); 
	p.drawText(r, Qt::AlignCenter, s);
}

void MainWindow::drawArrow(QPoint Q1, QPoint Q2, QPainter& p)
{
	qreal arrowSize = Node::DimNode / 2.0;
	p.setBrush(Qt::black);
	QLineF line(Q2, Q1);

	double angle = std::atan2(-line.dy(), line.dx());
	QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
	QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);

	QPolygonF arrowHead;
	arrowHead.clear();
	arrowHead << line.p1() << arrowP1 << arrowP2;
	p.drawPolygon(arrowHead);

	p.setBrush(Qt::white);
}

void MainWindow::paintEvent(QPaintEvent* e)
{

	QPainter p(this);
		for (Node* n : graf.getNodes())
			paintNode(p, n);

		for (Arch* ed : graf.getArches())
			drawLine(ed->getFirstNode(), ed->getSecondNode(), Node::DimNode / 2, p, ed->getCost());

		for(Arch* ed : graf.minCutEdges)
			drawLineColor(ed->getFirstNode(), ed->getSecondNode(), Node::DimNode / 2, p, Qt::red,ed->getCost());

		ClickedButton();
		update();
}

