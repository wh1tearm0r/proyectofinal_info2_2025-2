#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsRectItem>
#include <QObject>

class Obstaculo:public QObject, public QGraphicsRectItem{
    Q_OBJECT
public:
    Obstaculo();
    static bool juegoPausado;
    static void pausarJuego(bool estado);
public slots:
    void mover();
private:
int direccion;
};

#endif // OBSTACULO_H
