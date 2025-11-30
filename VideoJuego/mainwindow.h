#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Jugador;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void iniciarNivel1();
    void iniciarNivel2();
    void iniciarNivel3();
    void siguienteNivel();
    void gameOver();
    void volverAlMenu();

private:
    Ui::MainWindow *ui;

    // Componentes del juego
    QGraphicsScene *scene;
    QGraphicsView *view;
    Jugador *jugador;
    QTimer *timer;

    // Control de niveles
    int nivelActual;
    bool enJuego;

    // Widgets del menú
    QWidget *menuWidget;

    // Métodos auxiliares
    void configurarEscena();
    void configurarMenu();
    void mostrarMenu();
    void ocultarMenu();
    void limpiarNivel();
    void cargarNivel(int nivel);
};

#endif // MAINWINDOW_H
