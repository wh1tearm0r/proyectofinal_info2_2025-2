#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include <QString>
#include <QList>
#include <QTimer>
#include <QVector>
#include <QPixmap>

struct HabilidadEnemigo {
    QString nombre;
    int danioBase;
    int probabilidad; // De 0 a 100
    QString descripcion;
};

class Enemigo : public Personaje {
    Q_OBJECT
public:
    explicit Enemigo(QString nombre, int vidaMax, int ataque, int defensa,
                     QString rutaSprite, int nivel = 1, QGraphicsItem *parent = nullptr);

    void mover(int dx, int dy) override;
    void actualizarEstado() override;

    // Métodos de combate
    int atacar();
    int usarHabilidadAleatoria();
    void recibirDanio(int danio);
    void defender();
    bool estaVivo() const;
    int decidirAccion(); // Decide qué hacer en su turno

    // Getters
    QString getNombre() const { return nombre; }
    int getVidaMaxima() const { return vidaMaxima; }
    int getAtaque() const { return ataque; }
    int getDefensa() const { return defensa; }
    bool estaDefendiendo() const { return defendiendo; }
    int getNivel() const { return nivel; }
    QString getUltimaAccion() const { return ultimaAccion; }

signals:
    void murio();
    void vidaCambio(int vidaActual, int vidaMaxima);

private slots:
    void actualizarAnimacion();

private:
    QString nombre;
    int vidaMaxima;
    int ataque;
    int defensa;
    int nivel;
    bool defendiendo;
    QString ultimaAccion;
    QList<HabilidadEnemigo> habilidades;

    enum EstadoAnimacion {
        QUIETO,
        ATAQUE,
        DEFENSA,
        CURACION,
        CRITICO,
        RAPIDO
    };

    EstadoAnimacion estadoActual;
    QVector<QPixmap> frames;
    QTimer *timerAnimacion;
    int frameActual;

    void inicializarHabilidades();
    void cargarAnimacion(const QString &ruta, int columnas);
    void reproducirAnimacion(EstadoAnimacion nuevoEstado);
};

#endif // ENEMIGO_H
