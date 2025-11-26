#ifndef JUGADOR_H
#define JUGADOR_H

#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QObject>
#include <Qtimer>
#include <QElapsedTimer>

class Jugador:public QObject,public QGraphicsRectItem{
    Q_OBJECT
public:
    void keyPressEvent(QKeyEvent * event);
    Jugador();
public slots:
    void aparecer();
    void actualizarTiempo();
private:
    QTimer *temporizador = nullptr;
    QElapsedTimer reloj;
    const int tiempoRestante = 20000;
};

#endif // JUGADOR_H
