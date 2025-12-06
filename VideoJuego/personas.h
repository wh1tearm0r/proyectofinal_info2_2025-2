#ifndef PERSONAS_H
#define PERSONAS_H

#include "obstaculo.h"
#include <QTimer>

class personas : public Obstaculo
{
    Q_OBJECT

public:
    personas();
    ~personas() {}

public slots:
    void mover() override;

private:
    QTimer* timer;

    // Variables para el movimiento oscilatorio
    int velocidadY;           // Velocidad vertical (hacia abajo)
    int amplitud;             // Amplitud del movimiento horizontal
    int frecuencia;           // Controla qué tan rápido oscila
    qreal posicionInicialX;   // Posición X donde comenzó
    qreal contadorOscilacion; // Contador para calcular la oscilación
    int tipoMovimiento;       // Tipo de patrón de movimiento (0: seno, 1: zigzag, 2: circular)
};

#endif // PERSONAS_H
