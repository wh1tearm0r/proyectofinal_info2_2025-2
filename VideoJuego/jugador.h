#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.h"
#include <QKeyEvent>
#include <QTimer>
#include <QElapsedTimer>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>

class Jugador : public Personaje {
    Q_OBJECT
public:
    Jugador();
    // Implementación de métodos abstractos
    void mover(int dx, int dy) override;
    void actualizarEstado() override;

    // Métodos específicos de Jugador
    void keyPressEvent(QKeyEvent *event) override;
    QGraphicsTextItem *textoTiempo = nullptr;

public slots:
    void aparecer();
    void actualizarTiempo();

private:
    QTimer *temporizador;
    QElapsedTimer reloj;
    const int tiempoMaximo = 20000;
};

#endif // JUGADOR_H
