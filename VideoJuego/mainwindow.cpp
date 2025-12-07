#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Jugador.h"
#include "combate.h"
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include "obstaculo.h"

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
    , bordeLateral1(nullptr)
    , bordeLateral2(nullptr)
    , lineaLimiteIzq(nullptr)
    , lineaLimiteDer(nullptr)
{
    ui->setupUi(this);
    configurarEscena();
    configurarMenu();
    mostrarMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(timer) {
        timer->stop();
        timer->disconnect();
        delete timer;
    }
    if(scene) {
        scene->clear();
        delete scene;
    }
}

void MainWindow::configurarEscena()
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    QPixmap fondo(":/imagenes/Texxturas/Cuarto.png");
    scene->setBackgroundBrush(fondo.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    view = new QGraphicsView(scene, this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(800, 600);

    setCentralWidget(view);
    setFixedSize(800, 600);
}

void MainWindow::configurarMenu()
{
    menuWidget = new QWidget(view);
    menuWidget->setGeometry(250, 250, 300, 300);
    menuWidget->setStyleSheet("background-color: rgba(0, 0, 0, 180); border-radius: 10px;");

    QVBoxLayout *layout = new QVBoxLayout(menuWidget);
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->addSpacing(20);

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

void MainWindow::establecerFondo(const QString &rutaImagen)
{
    QPixmap fondo(rutaImagen);
    QBrush brush(fondo.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    scene->setBackgroundBrush(brush);
    scene->update();
}


void MainWindow::crearBordesLateralesNivel2()
{
    bordeLateral1 = new QGraphicsRectItem(0, 0, MARGEN_LATERAL_NIVEL2, 600);
    bordeLateral1->setBrush(QBrush(QColor(40, 40, 40, 200)));
    bordeLateral1->setPen(QPen(Qt::black, 2));
    bordeLateral1->setZValue(100);
    scene->addItem(bordeLateral1);

    bordeLateral2 = new QGraphicsRectItem(MARGEN_LATERAL_NIVEL2 + ANCHO_JUEGO_NIVEL2, 0, MARGEN_LATERAL_NIVEL2, 600);
    bordeLateral2->setBrush(QBrush(QColor(40, 40, 40, 200)));
    bordeLateral2->setPen(QPen(Qt::black, 2));
    bordeLateral2->setZValue(100);
    scene->addItem(bordeLateral2);

    lineaLimiteIzq = new QGraphicsLineItem(MARGEN_LATERAL_NIVEL2, 0, MARGEN_LATERAL_NIVEL2, 600);
    lineaLimiteIzq->setPen(QPen(Qt::yellow, 3));
    lineaLimiteIzq->setZValue(101);
    scene->addItem(lineaLimiteIzq);

    lineaLimiteDer = new QGraphicsLineItem(MARGEN_LATERAL_NIVEL2 + ANCHO_JUEGO_NIVEL2, 0,
                                           MARGEN_LATERAL_NIVEL2 + ANCHO_JUEGO_NIVEL2, 600);
    lineaLimiteDer->setPen(QPen(Qt::yellow, 3));
    lineaLimiteDer->setZValue(101);
    scene->addItem(lineaLimiteDer);
}

void MainWindow::eliminarBordesLaterales()
{
    if(bordeLateral1) {
        scene->removeItem(bordeLateral1);
        delete bordeLateral1;
        bordeLateral1 = nullptr;
    }
    if(bordeLateral2) {
        scene->removeItem(bordeLateral2);
        delete bordeLateral2;
        bordeLateral2 = nullptr;
    }
    if(lineaLimiteIzq) {
        scene->removeItem(lineaLimiteIzq);
        delete lineaLimiteIzq;
        lineaLimiteIzq = nullptr;
    }
    if(lineaLimiteDer) {
        scene->removeItem(lineaLimiteDer);
        delete lineaLimiteDer;
        lineaLimiteDer = nullptr;
    }
}

void MainWindow::limpiarNivel() {
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }

    if (scene) {
        QList<QGraphicsItem*> items = scene->items();
        for (auto item : items) {
            scene->removeItem(item);
            delete item;
        }
        scene->clear();
    }

    jugador = nullptr;
}


void MainWindow::cargarNivel(int nivel) {
    limpiarNivel();
    nivelActual = nivel;

    switch (nivel) {
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
    ocultarMenu();
    Obstaculo::pausarJuego(false);
    nivelActual = 1;

    establecerFondo(":/imagenes/Texxturas/CampoDeBatalla.png");

    // Crear jugador
    jugador = new Jugador();
    jugador->setNivel(1);
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    jugador->setFocus();

    // Posicionar jugador en el centro
    jugador->setPos(view->width() / 2 - jugador->pixmap().width() / 2,
                    view->height() - jugador->pixmap().height() - 10);

    // Agregar a la escena
    scene->addItem(jugador);
    scene->addItem(jugador->textoTiempo);

    connect(jugador, &Jugador::nivelCompletado, this, &MainWindow::siguienteNivel);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, jugador, &Jugador::aparecer);
    timer->start(450);
}

void MainWindow::iniciarNivel2()
{
    ocultarMenu();
    Obstaculo::pausarJuego(false);
    nivelActual = 2;

    // Establecer fondo para nivel 2
    establecerFondo(":/imagenes/Texxturas/fondo_chernobyl.png");

    // Crear bordes laterales SOLO para nivel 2
    crearBordesLateralesNivel2();

    // Crear jugador
    jugador = new Jugador();
    jugador->setNivel(2);
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    jugador->setFocus();

    // Ajustar límites laterales para el área estrecha
    jugador->setLimitesHorizontales(
        MARGEN_LATERAL_NIVEL2,
        MARGEN_LATERAL_NIVEL2 + ANCHO_JUEGO_NIVEL2 - jugador->pixmap().width()
        );


    // Posicionar en el centro del área estrecha
    jugador->setPos(MARGEN_LATERAL_NIVEL2 + (ANCHO_JUEGO_NIVEL2 / 2) - (jugador->pixmap().width() / 2),
                    view->height() - jugador->pixmap().height());

    scene->addItem(jugador);
    scene->addItem(jugador->textoTiempo);
    jugador->textoTiempo->setPlainText("¡¡¡ESCAPA!!!");


    connect(jugador, &Jugador::nivelCompletado, this, &MainWindow::siguienteNivel);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, jugador, &Jugador::aparecer);
    timer->start(400);
}

void MainWindow::iniciarNivel3()
{
    ocultarMenu();
    Obstaculo::pausarJuego(false);
    nivelActual = 3;

    establecerFondo(":/imagenes/Texxturas/fondotroya.png");

    Combate *combate = new Combate(this);
    combate->setGeometry(0, 0, 800, 600);
    combate->show();
    combate->raise();

    connect(combate, &Combate::combateTerminado, this,
            [this, combate](bool victoria) {
                if (victoria) {
                    QMessageBox::information(this, "¡Victoria!",
                                             "¡Has derrotado al Caballero!\n¡Has completado todos los niveles!");
                    volverAlMenu();
                } else {
                    gameOver();
                }
                combate->deleteLater();
            });

    combate->iniciarCombate();
}

void MainWindow::siguienteNivel()
{
    Obstaculo::pausarJuego(true);

    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }

    if (scene) {
        QList<QGraphicsItem*> items = scene->items();
        for (auto item : items) {
            scene->removeItem(item);
            delete item;
        }
        scene->clear();
    }

    nivelActual++;
    if (nivelActual > 3) {
        QMessageBox::information(this, "¡Felicidades!", "¡Has completado todos los niveles!");
        volverAlMenu();
        return;
    }

    QTimer::singleShot(300, [this]() {
        Obstaculo::pausarJuego(false);
        cargarNivel(nivelActual);
    });
}

void MainWindow::gameOver() {
    Obstaculo::pausarJuego(true);

    if (timer) {
        timer->stop();
        timer->disconnect();
    }

    if (jugador) {
        jugador->blockSignals(true);
    }

    QMessageBox::StandardButton respuesta;
    respuesta = QMessageBox::question(this, "PERDISTE",
                                      "¡INTENTA ESQUIVAR!\n\n¿Quieres intentar de nuevo?",
                                      QMessageBox::Yes | QMessageBox::No);

    if (respuesta == QMessageBox::Yes) {
        QTimer::singleShot(200, this, [this]() {
            cargarNivel(nivelActual);
        });
    } else {
        volverAlMenu();
    }
}


void MainWindow::volverAlMenu()
{
    limpiarNivel();
    nivelActual = 0;
    mostrarMenu();
}
