#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>

class personaje:public QGraphicsRectItem{
protected:
    bool estaVivo;
    int cantidadVida;

public:
    personaje();
    personaje(bool estaVivo, int cantidadVida);
};
#endif // PERSONAJE_H
