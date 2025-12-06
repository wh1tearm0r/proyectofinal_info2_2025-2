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
    frameActual(0),
    nivelActual(1)  // Nivel por defecto
{
    spriteQuieto = QPixmap(":/imagenes/Texxturas/SpriteQuieto.png").scaled(60, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(spriteQuieto);

    QPixmap spriteSheet(":/imagenes/Texxturas/spritescorriendo.png");
    int anchoFrame = 60;
    int altoFrame = 100;
    for (int i = 0; i < 4; i++) {
        framesCorrer.append(spriteSheet.copy(i * anchoFrame, 0, anchoFrame, altoFrame).scaled(60, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Jugador::actualizarSpriteCorrer);

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
        if (!timerAnimacion->isActive()) timerAnimacion->start(100);
    }
}

void Jugador::actualizarEstado() {
    timerAnimacion->stop();
    setPixmap(spriteQuieto);
}

void Jugador::actualizarSpriteCorrer() {
    frameActual = (frameActual + 1) % framesCorrer.size();
    setPixmap(framesCorrer[frameActual]);
}

void Jugador::actualizarTiempo() {
    if (Obstaculo::juegoPausado) return;

    int tiempoTranscurrido = reloj.elapsed();
    int tiempoRestante = (tiempoMaximo - tiempoTranscurrido) / 1000;

    textoTiempo->setPlainText(QString("Tiempo restante: %1 s").arg(tiempoRestante));

    if (tiempoTranscurrido >= tiempoMaximo) {
        Obstaculo::pausarJuego(true);
        temporizador->stop();

        QMessageBox msgBox;
        msgBox.setWindowTitle("Nivel completado\nSaliendo...");
        emit nivelCompletado();
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

    //case 3:

    default:
        // Nivel por defecto: balas
        {
            Bala *bala = new Bala();
            scene()->addItem(bala);
        }
        break;
    }
}
