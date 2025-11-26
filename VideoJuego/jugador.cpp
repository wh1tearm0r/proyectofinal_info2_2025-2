#include <QMessageBox>
#include "Jugador.h"
#include "Bala.h"
#include "Obstaculo.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QApplication>

Jugador::Jugador() : Personaje() {
    velocidad = 20; // Velocidad específica del jugador
    vida = 100;

    reloj.start();
    temporizador = new QTimer(this);
    connect(temporizador, SIGNAL(timeout()), this, SLOT(actualizarTiempo()));
    temporizador->start(100);
}

void Jugador::mover(int dx, int dy) {
    qreal nuevoX = x() + dx;
    qreal nuevoY = y() + dy;

    if (validarMovimiento(nuevoX, nuevoY)) {
        setPos(nuevoX, nuevoY);
    }
}

void Jugador::actualizarEstado() {
    // Aquí puedes agregar lógica para actualizar el estado del jugador
    // Por ejemplo: verificar colisiones, actualizar animaciones, etc.
}

void Jugador::actualizarTiempo() {
    if (Obstaculo::juegoPausado) return;

    if (reloj.elapsed() >= tiempoRestante) {
        Obstaculo::pausarJuego(true);
        temporizador->stop();

        QMessageBox msgBox;
        msgBox.setWindowTitle("¡Nivel completado!");
        msgBox.setStandardButtons(QMessageBox::Close);
        msgBox.setDefaultButton(QMessageBox::Close);
        qApp->exit(0);
    }
}

void Jugador::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_A:
        mover(-velocidad, 0);
        break;
    case Qt::Key_D:
        mover(velocidad, 0);
        break;
    case Qt::Key_W:
        mover(0, -velocidad);
        break;
    case Qt::Key_S:
        mover(0, velocidad);
        break;
    default:
        break;
    }

    actualizarEstado();
}

void Jugador::aparecer() {
    Obstaculo *bala = new Obstaculo();
    scene()->addItem(bala);
}
