#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene * scene =  new QGraphicsScene();

    QGraphicsRectItem * rect = new QGraphicsRectItem();
    rect -> setRect(0,0,100,100); //(x, y, width, height)

    scene -> addItem(rect);

    QGraphicsView * view = new QGraphicsView(scene);
    view -> show();

    return a.exec();
}
