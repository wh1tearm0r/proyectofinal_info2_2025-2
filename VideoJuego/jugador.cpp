#include <QMessageBox>
#include "Jugador.h"
#include "Bala.h"
#include "Obstaculo.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QApplication>

Jugador::Jugador() {
    reloj.start();
    temporizador = new QTimer(this);
    connect(temporizador, SIGNAL(timeout()), this, SLOT(actualizarTiempo()));
    temporizador->start(100);
}

void Jugador::actualizarTiempo() {
    if (Obstaculo::juegoPausado) return;

    if (reloj.elapsed() >= Jugador::tiempoRestante) {
        Obstaculo::pausarJuego(true);
        temporizador->stop();

        QMessageBox msgBox;
        msgBox.setWindowTitle("¡Nivel completado!");
        msgBox.setStandardButtons(QMessageBox::Close);
        msgBox.setDefaultButton(QMessageBox::Close);
        qApp->exit(0);
    }
}
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
    Obstaculo * obstaculo =new Obstaculo();
    scene()->addItem(obstaculo);
}

