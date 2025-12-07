#include "Obstaculo.h"
#include "Jugador.h"
#include "mainwindow.h"
#include <typeinfo>
#include <QMessageBox>
#include <QApplication>
#include <QTimer>
#include <QGraphicsScene>
#include <QDebug>

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

    for (auto *item : colisiones) {
        Jugador *jugador = dynamic_cast<Jugador*>(item);
        if (jugador) {

            Obstaculo::pausarJuego(true);
            if (scene()) scene()->removeItem(this);

            QMessageBox msgBox;
            msgBox.setWindowTitle("Game Over");
            msgBox.setText("Has sido alcanzado.");
            msgBox.setInformativeText("¿Qué deseas hacer?");
            msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Close);
            msgBox.setDefaultButton(QMessageBox::Retry);

            int ret = msgBox.exec();

            if (ret == QMessageBox::Retry) {

                QGraphicsScene *escenaActual = jugador->scene();
                if (!escenaActual) return;

                int nivelActual = jugador->getNivel();

                QList<QGraphicsItem*> elementos = escenaActual->items();
                for (auto *e : elementos) {
                    escenaActual->removeItem(e);
                    delete e;
                }

                Obstaculo::pausarJuego(false);

                // Crear nuevo jugador
                Jugador *nuevoJugador = new Jugador();
                nuevoJugador->setNivel(nivelActual);
                nuevoJugador->setFlag(QGraphicsItem::ItemIsFocusable);
                nuevoJugador->setFocus();

                if (nivelActual == 1) {
                    nuevoJugador->setPixmap(QPixmap(":/imagenes/Texxturas/SpriteQuieto.png")
                                                .scaled(100, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    nuevoJugador->setPos(370, 460);
                }
                else if (nivelActual == 2) {
                    nuevoJugador->setPixmap(QPixmap(":/imagenes/Texxturas/SpriteQuieto.png")
                                                .scaled(80, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    nuevoJugador->setPos(350, 470);

                    const int MARGEN_LATERAL = 150;
                    const int ANCHO_JUEGO = 500;

                    QGraphicsRectItem *bordeIzq = new QGraphicsRectItem(0, 0, MARGEN_LATERAL, 600);
                    bordeIzq->setBrush(QBrush(QColor(40, 40, 40, 200)));
                    bordeIzq->setPen(QPen(Qt::black, 2));
                    bordeIzq->setZValue(100);
                    escenaActual->addItem(bordeIzq);

                    QGraphicsRectItem *bordeDer = new QGraphicsRectItem(MARGEN_LATERAL + ANCHO_JUEGO, 0, MARGEN_LATERAL, 600);
                    bordeDer->setBrush(QBrush(QColor(40, 40, 40, 200)));
                    bordeDer->setPen(QPen(Qt::black, 2));
                    bordeDer->setZValue(100);
                    escenaActual->addItem(bordeDer);

                    // Líneas amarillas de límite
                    QGraphicsLineItem *lineaIzq = new QGraphicsLineItem(MARGEN_LATERAL, 0, MARGEN_LATERAL, 600);
                    lineaIzq->setPen(QPen(Qt::yellow, 3));
                    lineaIzq->setZValue(101);
                    escenaActual->addItem(lineaIzq);

                    QGraphicsLineItem *lineaDer = new QGraphicsLineItem(MARGEN_LATERAL + ANCHO_JUEGO, 0,
                                                                        MARGEN_LATERAL + ANCHO_JUEGO, 600);
                    lineaDer->setPen(QPen(Qt::yellow, 3));
                    lineaDer->setZValue(101);
                    escenaActual->addItem(lineaDer);
                }

                escenaActual->addItem(nuevoJugador);
                escenaActual->addItem(nuevoJugador->textoTiempo);

                QTimer *timer = new QTimer();
                QObject::connect(timer, SIGNAL(timeout()), nuevoJugador, SLOT(aparecer()));

                if (nivelActual == 1) timer->start(380);
                else if (nivelActual == 2) timer->start(400);

                Obstaculo::pausarJuego(false);
                qDebug() << "Nivel reiniciado correctamente (nivel" << nivelActual << ")";

            } else {
                qApp->exit(0);
            }

            deleteLater();
            return;
        }
    }
}
