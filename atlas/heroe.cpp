#include "heroe.h"

heroe::heroe() : personaje() {}

heroe::heroe(bool estaVivo, int cantidadVida)
    : personaje(estaVivo, cantidadVida) {}

void heroe::keyPressEvent(QKeyEvent* event){
    // Obtener los límites de la escena
    QRectF limites = scene()->sceneRect();

    // Calcular nueva posición
    qreal nuevaX = x();
    qreal nuevaY = y();

    if (event->key() == Qt::Key_Left) {
        nuevaX -= 10;
    } else if (event->key() == Qt::Key_Right) {
        nuevaX += 10;
    } else if (event->key() == Qt::Key_Up) {
        nuevaY -= 10;
    } else if (event->key() == Qt::Key_Down) {
        nuevaY += 10;
    }

    // Verificar que la nueva posición esté dentro de los límites
    // Considerando el tamaño del rectángulo del héroe
    if (nuevaX >= limites.left() &&
        nuevaX + boundingRect().width() <= limites.right() &&
        nuevaY >= limites.top() &&
        nuevaY + boundingRect().height() <= limites.bottom()) {

        setPos(nuevaX, nuevaY);
    }
}
