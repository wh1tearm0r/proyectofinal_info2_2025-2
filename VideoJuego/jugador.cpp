#include "Jugador.h"
#include "Bala.h"
#include "personas.h"
#include "Obstaculo.h"
#include <QMessageBox>
#include <QGraphicsScene>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

Jugador::Jugador(QGraphicsItem *parent)
    : Personaje(parent),
    direccionActual(0),
    frameActual(0),
    nivelActual(1)
{
    const int anchoEscalado = 100;
    const int altoEscalado = 140;

    spriteQuieto = QPixmap(":/imagenes/Texxturas/SpriteQuieto.png").scaled(anchoEscalado, altoEscalado, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(spriteQuieto);

    QPixmap spriteSheet(":/imagenes/Texxturas/spritescorriendo.png");
    int columnas = 8;
    int filas = 4;
    int anchoFrame = spriteSheet.width() / columnas;
    int altoFrame = spriteSheet.height() / filas;

    for (int fila = 0; fila < filas; fila++) {
        QVector<QPixmap> animFila;
        for (int col = 0; col < columnas; col++) {
            QPixmap frame = spriteSheet.copy(col * anchoFrame, fila * altoFrame, anchoFrame, altoFrame)
            .scaled(anchoEscalado, altoEscalado, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            animFila.append(frame);
        }
        animaciones.append(animFila);
    }

    setPixmap(animaciones[direccionActual][0]);

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Jugador::actualizarSpriteCorrer);
    timerAnimacion->start(100);

    reloj.start();
    temporizador = new QTimer(this);
    connect(temporizador, SIGNAL(timeout()), this, SLOT(actualizarTiempo()));
    temporizador->start(100);

    textoTiempo = new QGraphicsTextItem();
    textoTiempo->setDefaultTextColor(Qt::white);
    textoTiempo->setFont(QFont("Arial", 16, QFont::Bold));
    textoTiempo->setPos(10, 10);
}

void Jugador::setNivel(int nivel) {
    nivelActual = nivel;
}

void Jugador::mover(int dx, int dy) {
    qreal nuevoX = x() + dx;
    qreal nuevoY = y() + dy;
    if (validarMovimiento(nuevoX, nuevoY)) {
        setPos(nuevoX, nuevoY);
    }
}

void Jugador::actualizarEstado() {
    timerAnimacion->stop();
    setPixmap(spriteQuieto);
}

void Jugador::actualizarSpriteCorrer() {
    frameActual = (frameActual + 1) % animaciones[direccionActual].size();
    setPixmap(animaciones[direccionActual][frameActual]);
}

void Jugador::actualizarTiempo(){
    if (Obstaculo::juegoPausado) return;

    int tiempoTranscurrido = reloj.elapsed();
    int tiempoRestante = (tiempoMaximo - tiempoTranscurrido) / 1000;

    textoTiempo->setPlainText(QString("Tiempo restante: %1 s").arg(tiempoRestante));

    if (tiempoTranscurrido >= tiempoMaximo) {

        if (temporizador) {
            temporizador->stop();
            temporizador->disconnect();
        }
        if (timerAnimacion) {
            timerAnimacion->stop();
            timerAnimacion->disconnect();
        }

        setFlag(QGraphicsItem::ItemIsFocusable, false);

        QTimer::singleShot(500, this, [this]() {
            emit nivelCompletado();
            Obstaculo::pausarJuego(true);
        });
    }
}


void Jugador::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_W: // arriba
        mover(0, -velocidad);
        direccionActual = 0;
        break;
    case Qt::Key_A: // izquierda
        mover(-velocidad, 0);
        direccionActual = 1;
        break;
    case Qt::Key_S: // abajo
        mover(0, velocidad);
        direccionActual = 2;
        break;
    case Qt::Key_D: // derecha
        mover(velocidad, 0);
        direccionActual = 3;
        break;
    default:
        break;
    }
}

void Jugador::aparecer() {
    // Crear obstáculos según el nivel actual
    switch(nivelActual) {
    case 1:
        // Nivel 1: Solo balas
        {
            Bala *bala = new Bala();
            scene()->addItem(bala);
        }
        break;

    case 2:
        // Nivel 2: Solo personas
        {
            personas *persona = new personas();
            scene()->addItem(persona);
        }
        break;

    }
}
