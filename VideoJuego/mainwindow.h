#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Jugador;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void configurarEscena();
    void configurarMenu();
    void mostrarMenu();
    void ocultarMenu();
    void establecerFondo(const QString &rutaImagen);
    void limpiarNivel();
    void cargarNivel(int nivel);

    // Funciones para bordes laterales (nivel 2)
    void crearBordesLateralesNivel2();
    void eliminarBordesLaterales();
    int getAnchoJuegoNivel2() const { return ANCHO_JUEGO_NIVEL2; }
    int getMargenLateralNivel2() const { return MARGEN_LATERAL_NIVEL2; }

private slots:
    void iniciarNivel1();
    void iniciarNivel2();
    void iniciarNivel3();
    void siguienteNivel();
    void gameOver();
    void volverAlMenu();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    Jugador *jugador;
    QTimer *timer;
    int nivelActual;
    bool enJuego;
    QWidget *menuWidget;

    // 🔹 Atributos para el escenario estrecho (solo nivel 2)
    QGraphicsRectItem* bordeLateral1;
    QGraphicsRectItem* bordeLateral2;
    QGraphicsLineItem* lineaLimiteIzq;
    QGraphicsLineItem* lineaLimiteDer;

    // 🔹 Constantes para el área de juego estrecha (nivel 2)
    const int ANCHO_JUEGO_NIVEL2 = 500;      // Área de juego estrecha
    const int MARGEN_LATERAL_NIVEL2 = 150;   // Margen a cada lado
};

#endif // MAINWINDOW_H
