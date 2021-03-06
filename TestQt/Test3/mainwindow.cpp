#include "mainwindow.h"
#include <Qtgui/QtGui>
#include <QtWidgets/QtWidgets>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
{
        QToolBar *bar = this->addToolBar("Tools");
        QActionGroup *group = new QActionGroup(bar);


        QAction *drawNullAction = new QAction("Null", bar);
        drawNullAction->setIcon(QIcon(":/null.png"));
        drawNullAction->setToolTip(tr("Draw null."));
        drawNullAction->setStatusTip(tr("Draw null."));
        drawNullAction->setCheckable(true);
        drawNullAction->setChecked(true);
        group->addAction(drawNullAction);
        bar->addAction(drawNullAction);

        QAction *drawLineAction = new QAction("Line", bar);
        drawLineAction->setIcon(QIcon(":/line.png"));
        drawLineAction->setToolTip(tr("Draw a line."));
        drawLineAction->setStatusTip(tr("Draw a line."));
        drawLineAction->setCheckable(true);
        drawLineAction->setChecked(true);
        group->addAction(drawLineAction);
        bar->addAction(drawLineAction);

        QAction *drawRectAction = new QAction("Rectangle", bar);
        drawRectAction->setIcon(QIcon(":/rect.png"));
        drawRectAction->setToolTip(tr("Draw a rectangle."));
        drawRectAction->setStatusTip(tr("Draw a rectangle."));
        drawRectAction->setCheckable(true);
        group->addAction(drawRectAction);
        bar->addAction(drawRectAction);

        QLabel *statusMsg = new QLabel;
        statusBar()->addWidget(statusMsg);

        PaintWidget *paintWidget = new PaintWidget(this);
        QGraphicsView *view = new QGraphicsView(paintWidget, this);
        setCentralWidget(view);


        connect(drawNullAction, SIGNAL(triggered()),
           this, SLOT(drawNullActionTriggered()));

        connect(drawLineAction, SIGNAL(triggered()),
                        this, SLOT(drawLineActionTriggered()));
        connect(drawRectAction, SIGNAL(triggered()),
                        this, SLOT(drawRectActionTriggered()));
        connect(this, SIGNAL(changeCurrentShape(Shape::Code)),
                        paintWidget, SLOT(setCurrentShape(Shape::Code)));
}

void MainWindow::drawLineActionTriggered()
{
        emit changeCurrentShape(Shape::Line);
}

void MainWindow::drawNullActionTriggered()
{
        emit changeCurrentShape(Shape::Null);
}

void MainWindow::drawRectActionTriggered()
{
        emit changeCurrentShape(Shape::Rect);
}
