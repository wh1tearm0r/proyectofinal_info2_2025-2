#ifndef VENTANACOMBATE_H
#define VENTANACOMBATE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "jugadorcombate.h"
#include "enemigo.h"

class Combate : public QWidget {
    Q_OBJECT
public:
    explicit Combate(QWidget *parent = nullptr);
    void iniciarCombate();

signals:
    void combateTerminado(bool victoria);

private slots:
    void turnoJugador();
    void turnoEnemigo();
    void botonAtacarClicked();
    void botonDefenderClicked();
    void botonHabilidadClicked(int indice);
    void actualizarBarraVidaJugador();
    void actualizarBarraVidaEnemigo();
    void finalizarCombate(bool victoria);

private:
    // Escena y vista
    QGraphicsScene *scene;
    QGraphicsView *view;

    // Personajes
    jugadorCombate *jugador;
    Enemigo *enemigo;

    // UI Elements (sprites, barras, etc)
    QProgressBar *barraVidaJugador;
    QProgressBar *barraEnergiaJugador;
    QProgressBar *barraVidaEnemigo;
    QLabel *labelVidaJugador;
    QLabel *labelEnergiaJugador;
    QLabel *labelVidaEnemigo;
    QLabel *labelMensajes;

    QPushButton *btnAtacar;
    QPushButton *btnDefender;
    QList<QPushButton*> botonesHabilidades;

    QWidget *panelAcciones;

    // Estado del combate
    bool turnoDelJugador;

    void configurarUI();
    void crearEnemigo();
    void mostrarMensaje(const QString &mensaje);
    void deshabilitarBotones();
    void habilitarBotones();
};

#endif // VENTANACOMBATE_H
