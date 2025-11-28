#include "Obstaculo.h"
#include "Jugador.h"
#include <typeinfo>
#include <QMessageBox>
#include <QApplication>
#include <QProcess>
#include <QTimer>
#include <QGraphicsScene>
#include <QDebug>
#include <QBrush>
#include <stdlib.h>

bool Obstaculo::juegoPausado = false;

void Obstaculo::pausarJuego(bool estado) {
    juegoPausado = estado;
}

Obstaculo::Obstaculo(QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    // Constructor base - las subclases definirán su forma y posición
    direccion = 0;
}

void Obstaculo::manejarColision() {
    QList<QGraphicsItem*> colisiones = collidingItems();

    for (int i = 0; i < colisiones.size(); i++) {
        if (typeid(*(colisiones[i])) == typeid(Jugador)) {

            QGraphicsScene *escenaActual = scene();
            escenaActual->removeItem(this);

            Obstaculo::pausarJuego(true);

            QMessageBox msgBox;
            msgBox.setWindowTitle("Game Over");
            msgBox.setText("Prueba a esquivar las balas...");
            msgBox.setInformativeText("¿Qué vas a hacer?");
            msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Close);
            msgBox.setDefaultButton(QMessageBox::Retry);

            int ret = msgBox.exec();

            if (ret == QMessageBox::Retry) {
                // Reiniciar escena
                QList<QGraphicsItem*> elementos = escenaActual->items();

                // Eliminar todos los elementos de la escena
                for (auto item : elementos) {
                    escenaActual->removeItem(item);
                    delete item;
                }

                // Crear un nuevo jugador en la misma escena
                Jugador *nuevoJugador = new Jugador();
                nuevoJugador->setRect(0, 0, 60, 100);
                nuevoJugador->setBrush(QBrush(Qt::blue));
                escenaActual->addItem(nuevoJugador);
                escenaActual->addItem(nuevoJugador->textoTiempo);

                nuevoJugador->setFlag(QGraphicsItem::ItemIsFocusable);
                nuevoJugador->setFocus();
                nuevoJugador->setPos(350, 500);

                QTimer *timer = new QTimer();
                QObject::connect(timer, SIGNAL(timeout()), nuevoJugador, SLOT(aparecer()));
                timer->start(500);

                Obstaculo::pausarJuego(false);

            } else {
                qApp->exit(0);
            }

            delete this;
            return;
        }
    }
}
