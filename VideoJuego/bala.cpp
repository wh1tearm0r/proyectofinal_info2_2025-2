#include "Bala.h"
#include <QTimer>
#include <QGraphicsScene>
#include <stdlib.h>

Bala::Bala(QGraphicsItem *parent) : Obstaculo(parent)
{
    if (juegoPausado) return;

    QPixmap spriteBala(":/imagenes/bala.png");

    setPixmap(spriteBala.scaled(25, 10, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    direccion = (rand() % 2 == 0) ? 1 : -1;
    int rand_y = rand() % 550;

    if (direccion == 1) {
        setPos(0, rand_y);
    } else {
        setPos(780, rand_y);
        setTransform(QTransform().scale(-1, 1));
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(mover()));
    timer->start(30);
}

void Bala::mover() {
    if (juegoPausado) return;

    manejarColision();

    setPos(x() + direccion * 10, y());

    // Eliminar si sale de la pantalla
    if (x() < -pixmap().width() || x() > 800) {
        if (scene()) {
            scene()->removeItem(this);
        }
        deleteLater();
    }
}
