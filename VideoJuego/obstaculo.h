#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsScene>

class Obstaculo : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    explicit Obstaculo(QGraphicsItem *parent = nullptr);
    virtual ~Obstaculo() {}

    static void pausarJuego(bool estado);
    static bool juegoPausado;

public slots:
    virtual void mover() = 0;

protected:
    int direccion;
    void manejarColision();
};

#endif // OBSTACULO_H
