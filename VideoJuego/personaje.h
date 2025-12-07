#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QObject>
#include <QPixmap>

class Personaje : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit Personaje(QGraphicsItem *parent = nullptr);
    virtual ~Personaje() = default;

    // Métodos virtuales puros (abstractos)
    virtual void mover(int dx, int dy) = 0;
    virtual void actualizarEstado() = 0;

    // Métodos virtuales con implementación por defecto
    virtual void establecerPosicion(qreal x, qreal y);
    virtual bool validarMovimiento(qreal nuevoX, qreal nuevoY);

    // Getters y setters
    int getVida() const { return vida; }
    void setVida(int nuevaVida) { vida = nuevaVida; }

    int getVelocidad() const { return velocidad; }
    void setVelocidad(int nuevaVelocidad) { velocidad = nuevaVelocidad; }

protected:
    int vida;
    int velocidad;
    qreal limiteSuperior;
    qreal limiteInferior;
    qreal limiteIzquierdo;
    qreal limiteDerecho;

    QPixmap spriteActual;
};

#endif // PERSONAJE_H
