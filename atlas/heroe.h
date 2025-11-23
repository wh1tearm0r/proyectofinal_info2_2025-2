#ifndef HEROE_H
#define HEROE_H

#include "personaje.h"
#include <QKeyEvent>
#include <QGraphicsRectItem>

class heroe : public personaje {
public:
    heroe();
    heroe(bool estaVivo, int cantidadVida);

    // Aquí puedes agregar métodos y atributos específicos de heroe

    void keyPressEvent(QKeyEvent* event) override;
};

#endif // HEROE_H
