#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <QGraphicsRectItem>
#include <QObject>

class Enemigo:public QObject, public QGraphicsRectItem{
    Q_OBJECT
public:
    Enemigo();
public slots:
    void mover();
private:
int direccion;
};

#endif // ENEMIGO_H
