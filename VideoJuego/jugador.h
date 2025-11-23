#ifndef JUGADOR_H
#define JUGADOR_H

#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QObject>

class Jugador:public QObject,public QGraphicsRectItem{
    Q_OBJECT
public:
    void keyPressEvent(QKeyEvent * event);
public slots:
    void aparecer();
};

#endif // JUGADOR_H
