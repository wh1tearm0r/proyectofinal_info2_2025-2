#include "Obstaculo.h"
#include "Jugador.h"
#include <typeinfo>
#include <QMessageBox>
#include <QApplication>
#include <QTimer>
#include <QGraphicsScene>
#include <QDebug>
#include <QBrush>
#include <stdlib.h>

bool Obstaculo::juegoPausado = false;

void Obstaculo::pausarJuego(bool estado) {
    juegoPausado = estado;
}

Obstaculo::Obstaculo(QGraphicsItem *parent) : QGraphicsPixmapItem(parent),
    direccion(0)
{
}

void Obstaculo::manejarColision() {
    QList<QGraphicsItem*> colisiones = collidingItems();

    for (int i = 0; i < colisiones.size(); i++) {
        if (typeid(*(colisiones[i])) == typeid(Jugador)) {

            Obstaculo::pausarJuego(true);

            // Quitamos el obstáculo de la escena (sin borrarlo todavía)
            if (scene()) scene()->removeItem(this);

            // Marcamos este obstáculo para borrarse luego de forma segura
            this->deleteLater();

            // Mostramos el mensaje sin bloquear la interfaz
            QTimer::singleShot(100, [=]() {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Game Over");
                msgBox.setText("Has sido alcanzado por una bala.");
                msgBox.setInformativeText("¿Qué deseas hacer?");
                msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Close);
                msgBox.setDefaultButton(QMessageBox::Retry);

                int ret = msgBox.exec();

                if (ret == QMessageBox::Retry) {
                    QTimer::singleShot(0, [=]() {
                        // Reiniciar escena
                        QGraphicsScene *escenaActual = colisiones[i]->scene();
                        if (!escenaActual) return;



                        QList<QGraphicsItem*> elementos = escenaActual->items();
                        for (auto item : elementos) {
                            escenaActual->removeItem(item);
                            delete item;
                        }

                        Jugador *nuevoJugador = new Jugador();
                        escenaActual->addItem(nuevoJugador);
                        escenaActual->addItem(nuevoJugador->textoTiempo);

                        nuevoJugador->setFlag(QGraphicsItem::ItemIsFocusable);
                        nuevoJugador->setFocus();
                        nuevoJugador->setPos(350, 500);

                        QTimer *timer = new QTimer();
                        QObject::connect(timer, SIGNAL(timeout()), nuevoJugador, SLOT(aparecer()));
                        timer->start(500);

                        Obstaculo::pausarJuego(false);
                    });
                } else {
                    QTimer::singleShot(0, qApp, &QApplication::quit);
                }
            });

            return;
        }
    }
}
