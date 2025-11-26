#include "Personaje.h"

Personaje::Personaje(QGraphicsItem *parent)
    : QGraphicsRectItem(parent), vida(100), velocidad(20),
    limiteSuperior(0), limiteInferior(600),
    limiteIzquierdo(100), limiteDerecho(700) {
}

void Personaje::establecerPosicion(qreal x, qreal y) {
    if (validarMovimiento(x, y)) {
        setPos(x, y);
    }
}

bool Personaje::validarMovimiento(qreal nuevoX, qreal nuevoY) {
    return (nuevoX >= limiteIzquierdo &&
            nuevoX + rect().width() <= limiteDerecho &&
            nuevoY >= limiteSuperior &&
            nuevoY + rect().height() <= limiteInferior);
}
