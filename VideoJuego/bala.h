#ifndef BALA_H
#define BALA_H

#include "Obstaculo.h"
#include <QTimer>

class Bala : public Obstaculo {
    Q_OBJECT

public:
    Bala(QGraphicsItem *parent = nullptr);
    ~Bala() {}

public slots:
    void mover() override;

private:
    QTimer *timer;
};

#endif // BALA_H
