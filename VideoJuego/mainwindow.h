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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void iniciarNivel1();
    void iniciarNivel2();
    void iniciarNivel3();
    void siguienteNivel();
    void gameOver();

private:
    Ui::MainWindow *ui;

    // Componentes del juego
    QGraphicsScene *scene;
    QGraphicsView *view;
    Jugador *jugador;
    QTimer *timer;

    // Control de niveles
    int nivelActual;

    // Métodos auxiliares
    void configurarEscena();
    void limpiarNivel();
    void cargarNivel(int nivel);
};

#endif // MAINWINDOW_H
