#ifndef PERSONAS_H
#define PERSONAS_H

#include "obstaculo.h"
#include <QTimer>

class personas : public Obstaculo
{
    Q_OBJECT

public:
    personas();
    ~personas() {}

public slots:
    void mover() override;

private:
    QTimer* timer;
};

#endif // PERSONAS_H
