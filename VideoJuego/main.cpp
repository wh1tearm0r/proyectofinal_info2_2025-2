#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsScene>
#include "Jugador.h"
#include <QGraphicsView>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //crear escenario
    QGraphicsScene * scene = new QGraphicsScene();

    //Crear un Item
    Jugador * jugador = new Jugador();
    jugador->setRect(0,0,80,80);
    jugador ->setBrush(Qt::blue);
    scene ->addItem(jugador);

    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    jugador->setFocus();

    QGraphicsView * view = new QGraphicsView(scene);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view -> show();
    view -> setFixedSize(800,600);
    scene ->setSceneRect(0,0,800,600);
    jugador ->setPos(view->width()/2 -jugador->rect().width()/2,view->height() -jugador->rect().height());

    QTimer * timer= new QTimer();
    QObject::connect(timer,SIGNAL(timeout()),jugador,SLOT(aparecer()));
    timer ->start(500);

    return a.exec();
}
