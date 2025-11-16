#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "heroe.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene * scene =  new QGraphicsScene();
    scene->setSceneRect(0, 0, 800, 600);

    heroe* _heroe = new heroe();
    _heroe -> setRect(0,0,100,100); //(x, y, width, height)

    scene -> addItem(_heroe);

    _heroe -> setFlag(QGraphicsItem::ItemIsFocusable);
    _heroe->setFocus();

    QGraphicsView * view = new QGraphicsView(scene);
    view->setFixedSize(800, 600);
    view -> show();

    return a.exec();
}
