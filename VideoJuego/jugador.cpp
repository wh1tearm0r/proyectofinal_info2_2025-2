#include <QMessageBox>
#include "Jugador.h"
#include "Bala.h"
#include "Enemigo.h"
#include <QDebug>
#include <QGraphicsScene>

void Jugador::keyPressEvent(QKeyEvent *event)
{
    int step = 20;

    switch (event->key()) {
    case Qt::Key_A:
        if (pos().x() > 100)
            setPos(x() - step, y());
        break;
    case Qt::Key_D:
        if (pos().x() + rect().width() < 700)
            setPos(x() + step, y());
        break;
    case Qt::Key_W:
        if (pos().y() > 0)
            setPos(x(), y() - step);
        break;
    case Qt::Key_S:
        if (pos().y() + rect().height() < 600)
            setPos(x(), y() + step);
        break;
    default:
        break;
    }
}



void Jugador::aparecer(){
    Enemigo * enemigo =new Enemigo();
    scene()->addItem(enemigo);
}
