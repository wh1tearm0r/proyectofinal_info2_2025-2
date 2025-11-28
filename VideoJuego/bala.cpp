#include "Bala.h"
#include <QBrush>
#include <stdlib.h>

Bala::Bala(QGraphicsItem *parent) : Obstaculo(parent) {

    if (juegoPausado) return;

    // Configurar forma de la bala
    setRect(0, 0, 20, 10);

    // Dirección aleatoria (izquierda o derecha)
    direccion = (rand() % 2 == 0) ? 1 : -1;
    int rand_y = rand() % 550;

    // Posición y color según dirección
    if (direccion == 1) {
        setPos(0, rand_y);
        setBrush(QBrush(Qt::red));
    } else {
        setPos(780, rand_y);
        setBrush(QBrush(Qt::darkRed));
    }

    // Configurar temporizador para movimiento
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(mover()));
    timer->start(30);
}

void Bala::mover() {

    if (juegoPausado) return;

    // Verificar colisiones
    manejarColision();

    // Movimiento horizontal
    setPos(x() + direccion * 10, y());

    // Eliminar bala si sale de los límites
    if (x() < -rect().width() || x() > 800) {
        if (scene()) {
            scene()->removeItem(this);
        }
        deleteLater();
    }
}
