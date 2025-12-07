#include "personas.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QPainter>
#include <cmath>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

personas::personas()
    : contadorOscilacion(0)
{
    if (juegoPausado) return;

    // Crear un pixmap cuadrado más grande (50x50)
    QPixmap spritePersona(70, 110);
    spritePersona.fill(Qt::transparent);

    // Dibujar un cuadrado azul
    QPainter painter(&spritePersona);
    painter.setBrush(QBrush(QColor(30, 144, 255))); // Azul dodger
    painter.setPen(QPen(Qt::white, 2)); // Borde blanco
    painter.drawRect(2, 2, 46, 46);
    painter.end();

    setPixmap(spritePersona);

    // IMPORTANTE: Posición aleatoria SOLO dentro del área estrecha (150 a 650)
    const int MARGEN_LATERAL = 150;
    const int ANCHO_JUEGO = 500;

    // Generar posición X entre 150 y 600 (dejando 50px de margen por el sprite)
    int rand_x = MARGEN_LATERAL + (rand() % (ANCHO_JUEGO - 70));
    posicionInicialX = rand_x;

    // Siempre empieza desde arriba (y = 0)
    setPos(rand_x, 0);

    // Dirección siempre hacia abajo (1 = hacia abajo)
    direccion = 1;

    // Seleccionar tipo de movimiento aleatorio
    tipoMovimiento = rand() % 3;

    // Configurar parámetros según el tipo de movimiento
    switch(tipoMovimiento) {
    case 0: // Movimiento senoidal suave
        velocidadY = 5;
        amplitud = 60;
        frecuencia = 8;
        break;
    case 1: // Movimiento zigzag pronunciado
        velocidadY = 6;
        amplitud = 100;
        frecuencia = 12;
        break;
    case 2: // Movimiento ondulatorio amplio y lento
        velocidadY = 4;
        amplitud = 80;
        frecuencia = 6;
        break;
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(mover()));
    timer->start(30);
}

void personas::mover(){
    if (juegoPausado) return;

    // Manejar colisiones con el jugador
    manejarColision();

    // Incrementar el contador de oscilación
    contadorOscilacion += 0.1;

    qreal nuevaX = posicionInicialX;
    qreal nuevaY = y() + velocidadY;

    // Calcular el desplazamiento horizontal según el tipo de movimiento
    switch(tipoMovimiento) {
    case 0: // Movimiento senoidal (onda suave)
        nuevaX = posicionInicialX + amplitud * sin(contadorOscilacion * frecuencia * M_PI / 180.0);
        break;

    case 1: // Movimiento zigzag (triangular)
    {
        // Crear una función triangular para zigzag
        qreal periodo = 360.0 / frecuencia;
        qreal fase = fmod(contadorOscilacion * frecuencia, 360.0);
        qreal valorTriangular;

        if (fase < 90.0) {
            valorTriangular = fase / 90.0;
        } else if (fase < 270.0) {
            valorTriangular = 2.0 - (fase / 90.0);
        } else {
            valorTriangular = (fase / 90.0) - 4.0;
        }

        nuevaX = posicionInicialX + amplitud * valorTriangular;
    }
    break;

    case 2: // Movimiento circular/elíptico
    {
        // Combinar seno y coseno para crear un patrón más complejo
        qreal angulo = contadorOscilacion * frecuencia * M_PI / 180.0;
        nuevaX = posicionInicialX + amplitud * sin(angulo);

        // Pequeña variación en Y para efecto más natural
        nuevaY += 2 * cos(angulo * 2);
    }
    break;
    }

    // CRÍTICO: Asegurar que NO salga del área estrecha (150 a 650)
    const int MARGEN_LATERAL = 150;
    const int ANCHO_JUEGO = 500;
    const int LIMITE_IZQUIERDO = MARGEN_LATERAL;
    const int LIMITE_DERECHO = MARGEN_LATERAL + ANCHO_JUEGO - 50; // -50 por el ancho del sprite

    if (nuevaX < LIMITE_IZQUIERDO) nuevaX = LIMITE_IZQUIERDO;
    if (nuevaX > LIMITE_DERECHO) nuevaX = LIMITE_DERECHO;

    // Actualizar posición
    setPos(nuevaX, nuevaY);

    // Eliminar si sale de la pantalla (parte inferior)
    if (y() > 600) {
        if (scene()) {
            scene()->removeItem(this);
        }
        deleteLater();
    }
}
