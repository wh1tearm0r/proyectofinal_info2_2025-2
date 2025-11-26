#include "Obstaculo.h"
#include "Jugador.h"
#include <typeinfo>
#include <QMessageBox>
#include <QApplication>
#include <QProcess>
#include <QTimer>
#include <QGraphicsScene>
#include <QDebug>
#include <stdlib.h> // rand()

bool Obstaculo::juegoPausado = false;

void Obstaculo::pausarJuego(bool estado) {
    juegoPausado = estado;
}

Obstaculo::Obstaculo() {

    if (juegoPausado) return;

    setRect(0, 0, 20, 10);
    direccion = (rand() % 2 == 0) ? 1 : -1;
    int rand_y = rand() % 550;

    if (direccion == 1) {
        setPos(0, rand_y);
        setBrush(Qt::red);
    } else {
        setPos(780, rand_y);
        setBrush(Qt::darkRed);
    }

    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(mover()));
    timer->start(30);
}

void Obstaculo::mover() {

    if (juegoPausado) return;

    QList<QGraphicsItem*> colisiones = collidingItems();
    for (int i = 0; i < colisiones.size(); i++) {
        if (typeid(*(colisiones[i])) == typeid(Jugador)) {

            scene()->removeItem(this);
            delete this;

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
                QGraphicsScene *escenaActual = colisiones[i]->scene();
                QList<QGraphicsItem*> elementos = escenaActual->items();

                // Eliminar todos los elementos de la escena
                for (auto item : elementos) {
                    escenaActual->removeItem(item);
                    delete item;
                }

                // Crear un nuevo jugador en la misma escena
                Jugador *nuevoJugador = new Jugador();
                nuevoJugador->setRect(0, 0, 60, 100);
                nuevoJugador->setBrush(Qt::blue);
                escenaActual->addItem(nuevoJugador);

                nuevoJugador->setFlag(QGraphicsItem::ItemIsFocusable);
                nuevoJugador->setFocus();
                nuevoJugador->setPos(350, 500);

                // Reiniciar enemigos
                QTimer *timer = new QTimer();
                QObject::connect(timer, SIGNAL(timeout()), nuevoJugador, SLOT(aparecer()));
                timer->start(500);

                Obstaculo::pausarJuego(false);

            } else {
                qApp->exit(0);
            }

            return;
        }
    }

    // Movimiento horizontal normal
    setPos(x() + direccion * 10, y());

    // Eliminar bala si sale de los límites
    if (x() < -rect().width() || x() > 800) {
        scene()->removeItem(this);
        delete this;
    }
}
