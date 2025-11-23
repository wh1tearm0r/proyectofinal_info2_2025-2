#include "Bala.h"
#include "Enemigo.h"
#include <QTimer>
#include <QObject>
#include <QGraphicsScene>
#include <QDebug>

Bala::Bala() {
    setRect(0,0,10,50);
    QTimer * timer=new QTimer();
    connect(timer, SIGNAL(timeout()),this,SLOT(mover()));
    timer->start(50);
}

void Bala::mover(){
    QList <QGraphicsItem *>colliding_items=collidingItems();
    for (int i=0, n=colliding_items.size(); i<n;i++){
        if (typeid(*(colliding_items[i]))==typeid(Enemigo)){
            //remover ambos
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);
            //Borrar ambos
            delete colliding_items[i];
            delete this;
            return;
        }
    }

    setPos(x(),y()-10);
    if (pos().y()+rect().height()<0){
        scene()->removeItem(this);
        delete this;
        qDebug()<<"Bala eliminada";

    }

}
