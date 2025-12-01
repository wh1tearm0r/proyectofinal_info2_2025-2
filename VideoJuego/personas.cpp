#include "personas.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QPainter>
#include <stdlib.h>


personas::personas(){
    if (juegoPausado) return;

    // Crear un pixmap cuadrado más grande (50x50)
    QPixmap spritePersona(50, 50);
    spritePersona.fill(Qt::transparent);

    // Dibujar un cuadrado azul
    QPainter painter(&spritePersona);
    painter.setBrush(QBrush(QColor(30, 144, 255))); // Azul dodger
    painter.setPen(QPen(Qt::white, 2)); // Borde blanco
    painter.drawRect(2, 2, 46, 46);
    painter.end();

    setPixmap(spritePersona);

    // Posición aleatoria en X (horizontal)
    int rand_x = rand() % 750; // Entre 0 y 750 para que no salga del borde

    // Siempre empieza desde arriba (y = 0)
    setPos(rand_x, 0);

    // Dirección siempre hacia abajo (1 = hacia abajo)
    direccion = 1;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(mover()));
    timer->start(30);
}

void personas::mover(){
    if (juegoPausado) return;

    // Manejar colisiones con el jugador
    manejarColision();

    // Mover hacia abajo (aumentar Y)
    setPos(x(), y() + direccion * 8);

    // Eliminar si sale de la pantalla (parte inferior)
    if (y() > 600) {
        if (scene()) {
            scene()->removeItem(this);
        }
        deleteLater();
    }
}
