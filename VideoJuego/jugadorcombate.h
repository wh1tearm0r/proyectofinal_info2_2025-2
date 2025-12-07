#ifndef JUGADORCOMBATE_H
#define JUGADORCOMBATE_H

#include "personaje.h"
#include <QString>
#include <QList>
#include <QTimer>
#include <QVector>
#include <QPixmap>

struct Habilidad{
    QString nombre;
    int danio;
    int costoEnergia;
    QString descripcion; //Con esto podriamos imprimir en la interfaz la descripción del ataque
};

class jugadorCombate : public Personaje
{
    Q_OBJECT
public:
    explicit jugadorCombate(QGraphicsItem *parent = nullptr);

    void mover(int dx, int dy) override;
    void actualizarEstado() override;

    int ataqueBasico();
    int usarHabilidad(int indiceHabilidad);
    void defender();
    void recibirDanio(int danio);
    void curar(int cantidad);
    bool estaVivo() const;

    int getVidaMaxima() const { return vidaMaxima; }
    int getEnergia() const;
    int getEnergiaMaxima() const { return energiaMaxima; }
    int getAtaque() const { return ataque; }
    int getDefensa() const { return defensa; }
    bool estaDefendiendo() const { return defendiendo; }
    QList<Habilidad> getHabilidades() const { return habilidades; }

signals:
    void vidaCambio(int vidaActual, int vidaMaxima);
    void energiaCambio(int manaActual, int energiaMaxima);
    void murio();

private slots:
    void actualizarAnimacion();

private:

    int vidaMaxima;
    int energia;
    int energiaMaxima;
    int ataque;
    int defensa;
    bool defendiendo;
    QList<Habilidad> habilidades;

    enum EstadoAnimacion {
        QUIETO,
        ATAQUE_BASICO,
        ATAQUE_RAPIDO,
        GOLPE_CRITICO,
        CURACION,
        DEFENSA
    };

    EstadoAnimacion estadoActual;
    QVector<QPixmap> frames;
    QTimer *timerAnimacion;
    int frameActual;

    void inicializarHabilidades();
    void cargarAnimacion(const QString &ruta, int columnas);
    void reproducirAnimacion(EstadoAnimacion nuevoEstado);


};

#endif // JUGADORCOMBATE_H
