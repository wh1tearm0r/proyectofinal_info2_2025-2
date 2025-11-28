#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Jugador.h"
#include <QGraphicsPixmapItem>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(nullptr)
    , view(nullptr)
    , jugador(nullptr)
    , timer(nullptr)
    , nivelActual(0)
{
    ui->setupUi(this);

    // Configurar la escena principal
    configurarEscena();

    // Iniciar con el nivel 1
    iniciarNivel1();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(timer) delete timer;
    if(scene) delete scene;
}

void MainWindow::configurarEscena()
{
    // Crear escena
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    // Crear vista
    view = new QGraphicsView(scene, this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(800, 600);

    // Establecer la vista como widget central
    setCentralWidget(view);

    // Ajustar tamaño de la ventana
    setFixedSize(800, 600);
}

void MainWindow::limpiarNivel()
{
    // Detener timer si existe
    if(timer) {
        timer->stop();
        disconnect(timer, nullptr, nullptr, nullptr);
    }

    // Limpiar la escena
    scene->clear();
    jugador = nullptr;
}

void MainWindow::cargarNivel(int nivel)
{
    limpiarNivel();
    nivelActual = nivel;

    switch(nivel) {
    case 1:
        iniciarNivel1();
        break;
    case 2:
        iniciarNivel2();
        break;
    case 3:
        iniciarNivel3();
        break;
    default:
        QMessageBox::information(this, "¡Felicidades!", "Has completado todos los niveles");
        close();
        break;
    }
}

void MainWindow::iniciarNivel1()
{
    // Crear jugador
    jugador = new Jugador();
    jugador->setRect(0, 0, 60, 100);
    jugador->setBrush(Qt::blue);
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    jugador->setFocus();

    // Posicionar jugador
    jugador->setPos(view->width()/2 - jugador->rect().width()/2,
                    view->height() - jugador->rect().height());

    // Agregar a la escena
    scene->addItem(jugador);
    scene->addItem(jugador->textoTiempo);

    // Configurar timer (velocidad normal)
    if(!timer) {
        timer = new QTimer(this);
    }
    connect(timer, SIGNAL(timeout()), jugador, SLOT(aparecer()));
    timer->start(450);

    // Conectar señal de nivel completado (deberías implementar esto en Jugador)
    // connect(jugador, &Jugador::nivelCompletado, this, &MainWindow::siguienteNivel);
}

void MainWindow::iniciarNivel2()
{
   // Implementar Segundo Nivel
}

void MainWindow::iniciarNivel3()
{
    // Implementar Tercer Nivel
}

void MainWindow::siguienteNivel()
{
    nivelActual++;
    cargarNivel(nivelActual);
}

void MainWindow::gameOver()
{
    timer->stop();
    QMessageBox::information(this, "Game Over", "¡Has perdido! Intenta de nuevo.");
    cargarNivel(nivelActual); // Reiniciar el nivel actual
}
