#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QGraphicsTextItem>
#include <QPixmap>
#include <QVector>

class Jugador : public Personaje {
    Q_OBJECT
public:
    explicit Jugador(QGraphicsItem *parent = nullptr);

    void keyPressEvent(QKeyEvent *event) override;

    // Implementación de los métodos abstractos
    void mover(int dx, int dy) override;
    void actualizarEstado() override;

    // Control del tiempo del nivel
    QGraphicsTextItem *textoTiempo = nullptr;
    void setNivel(int nivel);
    int getNivel() const { return nivelActual; }

public slots:
    void aparecer();
    void actualizarTiempo();

signals:
    void nivelCompletado();

private:
    // Animaciones
    QVector<QVector<QPixmap>> animaciones;
    int direccionActual;
    QPixmap spriteQuieto;
    int frameActual;
    QTimer *timerAnimacion;

    // Control del tiempo
    QTimer *temporizador;
    QElapsedTimer reloj;
    const int tiempoMaximo = 8000; // 20 segundos
    int nivelActual;

    void actualizarSpriteCorrer();
};

#endif // JUGADOR_H
