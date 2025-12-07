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

Obstaculo::Obstaculo(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
    direccion(0)
{
}

void Obstaculo::manejarColision() {
    if (juegoPausado) return;

    QList<QGraphicsItem*> colisiones = collidingItems();

    for (int i = 0; i < colisiones.size(); i++) {
        if (typeid(*(colisiones[i])) == typeid(Jugador)) {

            Obstaculo::pausarJuego(true);

            if (scene())
                scene()->removeItem(this);

            QMessageBox msgBox;
            msgBox.setWindowTitle("Game Over");
            msgBox.setText("Has sido alcanzado.");
            msgBox.setInformativeText("¿Qué deseas hacer?");
            msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Close);
            msgBox.setDefaultButton(QMessageBox::Retry);

            int ret = msgBox.exec();

            if (ret == QMessageBox::Retry) {
                // Reiniciar escena
                QGraphicsScene *escenaActual = colisiones[i]->scene();

                if (escenaActual) {
                    QList<QGraphicsItem*> elementos = escenaActual->items();
                    for (auto item : elementos) {
                        escenaActual->removeItem(item);
                        delete item;
                    }

                    // Crear nuevo jugador
                    Jugador *nuevoJugador = new Jugador();
                    nuevoJugador->setPixmap(QPixmap(":/imagenes/Texxturas/SpriteQuieto.png")
                                                .scaled(60, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    escenaActual->addItem(nuevoJugador);
                    escenaActual->addItem(nuevoJugador->textoTiempo);

                    nuevoJugador->setFlag(QGraphicsItem::ItemIsFocusable);
                    nuevoJugador->setFocus();
                    nuevoJugador->setPos(350, 500);

                    // Reiniciar obstáculos con un timer nuevo
                    QTimer *timer = new QTimer();
                    QObject::connect(timer, SIGNAL(timeout()), nuevoJugador, SLOT(aparecer()));
                    timer->start(500);
                }

                Obstaculo::pausarJuego(false);
            } else {
                qApp->exit(0);
            }

            deleteLater();
            return;
        }
    }
}
