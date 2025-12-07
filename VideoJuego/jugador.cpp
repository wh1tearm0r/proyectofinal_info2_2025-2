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

    // --- Preparar spritesheet ---
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

    //Animación
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Jugador::actualizarSpriteCorrer);
    timerAnimacion->start(100);

    //Tiempo de nivel
    reloj.start();
    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &Jugador::actualizarTiempo);
    temporizador->start(100);

    //Texto del tiempo
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
    setPixmap(animaciones[direccionActual][0]);
}

void Jugador::actualizarSpriteCorrer() {
    frameActual = (frameActual + 1) % animaciones[direccionActual].size();
    setPixmap(animaciones[direccionActual][frameActual]);
}

void Jugador::actualizarTiempo() {
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
        Obstaculo::pausarJuego(true);

        QTimer::singleShot(500, this, [this]() {
            emit nivelCompletado();
        });
    }
}

void Jugador::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_W:
        mover(0, -velocidad);
        direccionActual = 0;
        break;
    case Qt::Key_A:
        mover(-velocidad, 0);
        direccionActual = 1;
        break;
    case Qt::Key_S:
        mover(0, velocidad);
        direccionActual = 2;
        break;
    case Qt::Key_D:
        mover(velocidad, 0);
        direccionActual = 3;
        break;
    default:
        break;
    }
}

void Jugador::aparecer() {
    Obstaculo *nuevo = nullptr;

    switch (nivelActual) {
    case 1:
        nuevo = new Bala();
        break;
    case 2:
        nuevo = new personas();
        break;
    }

    if (nuevo && scene()) {
        scene()->addItem(nuevo);
    }
}
