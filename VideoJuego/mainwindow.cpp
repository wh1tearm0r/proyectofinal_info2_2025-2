#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Jugador.h"
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(nullptr)
    , view(nullptr)
    , jugador(nullptr)
    , timer(nullptr)
    , nivelActual(0)
    , enJuego(false)
    , menuWidget(nullptr)
{
    ui->setupUi(this);

    // Configurar la escena principal
    configurarEscena();

    // Configurar y mostrar el menú
    configurarMenu();
    mostrarMenu();
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

    QPixmap fondo(":/imagenes/fondo.png");
    scene->setBackgroundBrush(fondo.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

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

void MainWindow::configurarMenu()
{
    // Crear widget del menú
    menuWidget = new QWidget(view);
    menuWidget->setGeometry(250, 250, 300, 300);
    menuWidget->setStyleSheet("background-color: rgba(0, 0, 0, 180); border-radius: 10px;");

    // Layout vertical
    QVBoxLayout *layout = new QVBoxLayout(menuWidget);
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    layout->addSpacing(20);

    // Botón Nivel 1
    QPushButton *btnNivel1 = new QPushButton("Nivel 1");
    btnNivel1->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   font-size: 18px;"
        "   padding: 15px;"
        "   border-radius: 5px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        );
    connect(btnNivel1, &QPushButton::clicked, this, &MainWindow::iniciarNivel1);
    layout->addWidget(btnNivel1);

    // Botón Nivel 2
    QPushButton *btnNivel2 = new QPushButton("Nivel 2");
    btnNivel2->setStyleSheet(
        "QPushButton {"
        "   background-color: #FF9800;"
        "   color: white;"
        "   font-size: 18px;"
        "   padding: 15px;"
        "   border-radius: 5px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e68900;"
        "}"
        );
    connect(btnNivel2, &QPushButton::clicked, this, &MainWindow::iniciarNivel2);
    layout->addWidget(btnNivel2);

    // Botón Nivel 3
    QPushButton *btnNivel3 = new QPushButton("Nivel 3");
    btnNivel3->setStyleSheet(
        "QPushButton {"
        "   background-color: #f44336;"
        "   color: white;"
        "   font-size: 18px;"
        "   padding: 15px;"
        "   border-radius: 5px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #da190b;"
        "}"
        );
    connect(btnNivel3, &QPushButton::clicked, this, &MainWindow::iniciarNivel3);
    layout->addWidget(btnNivel3);

    layout->addStretch();
}

void MainWindow::mostrarMenu()
{
    enJuego = false;
    if(menuWidget) {
        menuWidget->show();
        menuWidget->raise();
    }
}

void MainWindow::ocultarMenu()
{
    enJuego = true;
    if(menuWidget) {
        menuWidget->hide();
    }
}

void MainWindow::establecerFondo(const QString &rutaImagen){
    QPixmap fondo(rutaImagen);
    scene->setBackgroundBrush(fondo.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
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
    ocultarMenu();
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
        volverAlMenu();
        break;
    }
}

void MainWindow::iniciarNivel1()
{
    limpiarNivel();
    ocultarMenu();
    nivelActual = 1;

    establecerFondo(":/imagenes/Texxturas/CampoDeBatalla.png");

    // Crear jugador
    jugador = new Jugador();
    jugador->setPixmap(QPixmap(":/imagenes/Texxturas/SpriteQuieto.png").scaled(60, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    jugador->setFocus();

    // Posicionar jugador
    jugador->setPos(view->width() / 2 - jugador->pixmap().width() / 2,
                    view->height() - jugador->pixmap().height() - 10);

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
    limpiarNivel();
    ocultarMenu();
    nivelActual = 2;

    //Establecer fondo para el nivel

    jugador = new Jugador();
    jugador->setNivel(2);
    jugador->setPixmap(QPixmap(":/imagenes/Texxturas/SpriteQuieto.png").scaled(60, 100));
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    jugador->setFocus();
    jugador->setPos(view->width()/2 - jugador->pixmap().width()/2,
                    view->height() - jugador->pixmap().height());

    scene->addItem(jugador);
    scene->addItem(jugador->textoTiempo);

    // Configurar timer para nivel 2 (puedes ajustar la velocidad)
    if(!timer) {
        timer = new QTimer(this);
    }
    connect(timer, SIGNAL(timeout()), jugador, SLOT(aparecer()));
    timer->start(400);  // Aparecen personas cada 400ms (un poco más rápido)

}

void MainWindow::iniciarNivel3()
{
   // Implementar Tercer Nivel
    limpiarNivel();
    ocultarMenu();
    nivelActual = 3;

    //Establecer fondo para el nivel

    //Combate por turnos, un nivel totalmente distinto a los otros dos
    jugador = new Jugador();
    jugador->setNivel(3);
    jugador->setPixmap(QPixmap(":/imagenes/Texxturas/SpriteQuieto.png").scaled(60, 100));
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    jugador->setFocus();
    jugador->setPos(view->width()/2 - jugador->pixmap().width()/2,
                    view->height() - jugador->pixmap().height());
    scene->addItem(jugador);
}

void MainWindow::siguienteNivel()
{
    nivelActual++;
    if(nivelActual > 3) {
        QMessageBox::information(this, "¡Felicidades!", "¡Has completado todos los niveles!");
        volverAlMenu();
    } else {
        cargarNivel(nivelActual);
    }
}

void MainWindow::gameOver()
{
    if(timer) {
        timer->stop();
    }

    QMessageBox::StandardButton respuesta;
    respuesta = QMessageBox::question(this, "Game Over",
                                      "¡Has perdido!\n\n¿Quieres intentar de nuevo?",
                                      QMessageBox::Yes | QMessageBox::No);

    if(respuesta == QMessageBox::Yes) {
        cargarNivel(nivelActual); // Reiniciar el nivel actual
    } else {
        volverAlMenu();
    }
}

void MainWindow::volverAlMenu()
{
    limpiarNivel();
    mostrarMenu();
}
