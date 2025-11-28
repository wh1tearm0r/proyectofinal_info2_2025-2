#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QGraphicsScene>

class Obstaculo : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    Obstaculo(QGraphicsItem *parent = nullptr);
    virtual ~Obstaculo() {}

    static void pausarJuego(bool estado);
    static bool juegoPausado;

public slots:
    virtual void mover() = 0; // Método virtual puro

protected:
    int direccion;
    void manejarColision();
};

#endif // OBSTACULO_H
