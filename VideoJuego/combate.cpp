#include "combate.h"
#include <QTimer>
#include <QMessageBox>
#include <QGraphicsTextItem>
#include <QRandomGenerator>
#include <QFont>

Combate::Combate(QWidget *parent)
    : QWidget(parent)
    , scene(nullptr)
    , view(nullptr)
    , jugador(nullptr)
    , enemigo(nullptr)
    , barraVidaJugador(nullptr)
    , barraEnergiaJugador(nullptr)
    , barraVidaEnemigo(nullptr)
    , labelVidaJugador(nullptr)
    , labelEnergiaJugador(nullptr)
    , labelVidaEnemigo(nullptr)
    , labelMensajes(nullptr)
    , btnAtacar(nullptr)
    , btnDefender(nullptr)
    , panelAcciones(nullptr)
    , turnoDelJugador(true)
{
    setFixedSize(800, 600);
    setWindowTitle("Combate por Turnos");

    configurarUI();
}

void Combate::configurarUI()
{
    // Crear escena y vista
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 400);

    // Establecer fondo de batalla
    QPixmap fondoBatalla(":/imagenes/fondo_combate.png");
    if (!fondoBatalla.isNull()) {
        scene->setBackgroundBrush(fondoBatalla.scaled(800, 400,
                                                      Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        scene->setBackgroundBrush(QBrush(QColor(50, 50, 80)));
    }

    view = new QGraphicsView(scene, this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(800, 400);
    view->setGeometry(0, 0, 800, 400);

    // ========== Panel de información (parte inferior) ==========
    QWidget *panelInfo = new QWidget(this);
    panelInfo->setGeometry(0, 400, 800, 200);
    panelInfo->setStyleSheet("background-color: rgba(30, 30, 40, 220);");

    // Layout principal del panel
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(panelInfo);
    layoutPrincipal->setSpacing(10);
    layoutPrincipal->setContentsMargins(20, 10, 20, 10);

    // ===== Área de estadísticas =====
    QHBoxLayout *layoutEstadisticas = new QHBoxLayout();

    // Estadísticas del jugador (izquierda)
    QVBoxLayout *layoutJugador = new QVBoxLayout();

    QLabel *tituloJugador = new QLabel("JUGADOR");
    tituloJugador->setStyleSheet("color: #4CAF50; font-size: 14px; font-weight: bold;");
    layoutJugador->addWidget(tituloJugador);

    labelVidaJugador = new QLabel("Vida: 100/100");
    labelVidaJugador->setStyleSheet("color: white; font-size: 12px;");
    layoutJugador->addWidget(labelVidaJugador);

    barraVidaJugador = new QProgressBar();
    barraVidaJugador->setMaximum(100);
    barraVidaJugador->setValue(100);
    barraVidaJugador->setTextVisible(false);
    barraVidaJugador->setFixedHeight(20);
    barraVidaJugador->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid #555;"
        "   border-radius: 5px;"
        "   background-color: #2b2b2b;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #4CAF50;"
        "   border-radius: 3px;"
        "}"
        );
    layoutJugador->addWidget(barraVidaJugador);

    labelEnergiaJugador = new QLabel("Energía: 50/50");
    labelEnergiaJugador->setStyleSheet("color: white; font-size: 12px;");
    layoutJugador->addWidget(labelEnergiaJugador);

    barraEnergiaJugador = new QProgressBar();
    barraEnergiaJugador->setMaximum(50);
    barraEnergiaJugador->setValue(50);
    barraEnergiaJugador->setTextVisible(false);
    barraEnergiaJugador->setFixedHeight(20);
    barraEnergiaJugador->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid #555;"
        "   border-radius: 5px;"
        "   background-color: #2b2b2b;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #2196F3;"
        "   border-radius: 3px;"
        "}"
        );
    layoutJugador->addWidget(barraEnergiaJugador);

    // Estadísticas del enemigo (derecha)
    QVBoxLayout *layoutEnemigo = new QVBoxLayout();

    QLabel *tituloEnemigo = new QLabel("JEFE FINAL");
    tituloEnemigo->setStyleSheet("color: #f44336; font-size: 14px; font-weight: bold;");
    tituloEnemigo->setAlignment(Qt::AlignRight);
    layoutEnemigo->addWidget(tituloEnemigo);

    labelVidaEnemigo = new QLabel("Vida: 200/200");
    labelVidaEnemigo->setStyleSheet("color: white; font-size: 12px;");
    labelVidaEnemigo->setAlignment(Qt::AlignRight);
    layoutEnemigo->addWidget(labelVidaEnemigo);

    barraVidaEnemigo = new QProgressBar();
    barraVidaEnemigo->setMaximum(200);
    barraVidaEnemigo->setValue(200);
    barraVidaEnemigo->setTextVisible(false);
    barraVidaEnemigo->setFixedHeight(20);
    barraVidaEnemigo->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid #555;"
        "   border-radius: 5px;"
        "   background-color: #2b2b2b;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #f44336;"
        "   border-radius: 3px;"
        "}"
        );
    layoutEnemigo->addWidget(barraVidaEnemigo);

    layoutEstadisticas->addLayout(layoutJugador);
    layoutEstadisticas->addStretch();
    layoutEstadisticas->addLayout(layoutEnemigo);

    layoutPrincipal->addLayout(layoutEstadisticas);

    // ===== Área de mensajes =====
    labelMensajes = new QLabel("¡Comienza el combate final!");
    labelMensajes->setStyleSheet(
        "color: white; "
        "font-size: 13px; "
        "background-color: rgba(0, 0, 0, 100); "
        "padding: 8px; "
        "border-radius: 5px;"
        );
    labelMensajes->setAlignment(Qt::AlignCenter);
    labelMensajes->setWordWrap(true);
    labelMensajes->setFixedHeight(40);
    layoutPrincipal->addWidget(labelMensajes);

    // ===== Panel de acciones =====
    panelAcciones = new QWidget();
    QHBoxLayout *layoutAcciones = new QHBoxLayout(panelAcciones);
    layoutAcciones->setSpacing(10);

    // Botón Atacar
    btnAtacar = new QPushButton("ATACAR");
    btnAtacar->setFixedHeight(40);
    btnAtacar->setStyleSheet(
        "QPushButton {"
        "   background-color: #f44336;"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #da190b;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #a01207;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #555;"
        "   color: #888;"
        "}"
        );
    connect(btnAtacar, &QPushButton::clicked, this, &Combate::botonAtacarClicked);
    layoutAcciones->addWidget(btnAtacar);

    // Botón Defender
    btnDefender = new QPushButton("️DEFENDER");
    btnDefender->setFixedHeight(40);
    btnDefender->setStyleSheet(
        "QPushButton {"
        "   background-color: #2196F3;"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0b7dda;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #0960a0;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #555;"
        "   color: #888;"
        "}"
        );
    connect(btnDefender, &QPushButton::clicked, this, &Combate::botonDefenderClicked);
    layoutAcciones->addWidget(btnDefender);

    layoutPrincipal->addWidget(panelAcciones);
}

void Combate::iniciarCombate()
{
    // Crear jugador
    jugador = new jugadorCombate();
    jugador->setPos(150, 200);
    scene->addItem(jugador);

    // Conectar señales del jugador
    connect(jugador, &jugadorCombate::vidaCambio,
            this, &Combate::actualizarBarraVidaJugador);
    connect(jugador, &jugadorCombate::energiaCambio,
            this, [this](int energiaActual, int energiaMax) {
                barraEnergiaJugador->setMaximum(energiaMax);
                barraEnergiaJugador->setValue(energiaActual);
                labelEnergiaJugador->setText(QString("Energía: %1/%2").arg(energiaActual).arg(energiaMax));
            });
    connect(jugador, &jugadorCombate::murio,
            this, [this]() { finalizarCombate(false); });

    // Crear jefe final (enemigo único y poderoso)
    crearEnemigo();

    // Conectar señales del enemigo
    connect(enemigo, &Enemigo::vidaCambio,
            this, &Combate::actualizarBarraVidaEnemigo);
    connect(enemigo, &Enemigo::murio,
            this, [this]() { finalizarCombate(true); });

    // Crear botones de habilidades
    QHBoxLayout *layoutAcciones = qobject_cast<QHBoxLayout*>(panelAcciones->layout());

    QList<Habilidad> habilidades = jugador->getHabilidades();
    for (int i = 0; i < habilidades.size(); ++i) {
        QPushButton *btnHabilidad = new QPushButton(habilidades[i].nombre);
        btnHabilidad->setFixedHeight(40);
        btnHabilidad->setToolTip(QString("%1\nDaño: %2 | Energía: %3")
                                     .arg(habilidades[i].descripcion)
                                     .arg(habilidades[i].danio)
                                     .arg(habilidades[i].costoEnergia));
        btnHabilidad->setStyleSheet(
            "QPushButton {"
            "   background-color: #FF9800;"
            "   color: white;"
            "   font-size: 12px;"
            "   font-weight: bold;"
            "   border: none;"
            "   border-radius: 5px;"
            "   padding: 10px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #e68900;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #cc7a00;"
            "}"
            "QPushButton:disabled {"
            "   background-color: #555;"
            "   color: #888;"
            "}"
            );

        connect(btnHabilidad, &QPushButton::clicked,
                this, [this, i]() { botonHabilidadClicked(i); });

        botonesHabilidades.append(btnHabilidad);
        layoutAcciones->addWidget(btnHabilidad);
    }

    // Actualizar barras iniciales
    actualizarBarraVidaJugador();
    actualizarBarraVidaEnemigo();

    // Comenzar el combate
    turnoJugador();
}

void Combate::crearEnemigo()
{
    // Jefe final único y desafiante
    QString nombre = "Caballero";
    int vida = 85;
    int ataque = 15;
    int defensa = 6;
    QString sprite = ":/imagenes/jefe_final.png";

    enemigo = new Enemigo(nombre, vida, ataque, defensa, sprite);
    enemigo->setPos(550, 200);
    scene->addItem(enemigo);

    // Agregar nombre del enemigo sobre él
    QGraphicsTextItem *nombreEnemigo = new QGraphicsTextItem(nombre);
    nombreEnemigo->setDefaultTextColor(Qt::red);
    nombreEnemigo->setFont(QFont("Arial", 18, QFont::Bold));
    nombreEnemigo->setPos(enemigo->x() - 10, enemigo->y() - 35);
    scene->addItem(nombreEnemigo);
}

void Combate::turnoJugador()
{
    turnoDelJugador = true;
    habilitarBotones();
    mostrarMensaje("Tu turno - Elige una acción");
}

void Combate::turnoEnemigo()
{
    turnoDelJugador = false;
    deshabilitarBotones();

    mostrarMensaje("Turno del enemigo...");

    // Verificar si el enemigo está en modo FURIA (menos del 50% de vida)
    int vidaPorcentaje = (enemigo->getVida() * 100) / enemigo->getVidaMaxima();
    bool modoFuria = vidaPorcentaje < 50;

    if (modoFuria) {
        // Cambiar color de la barra de vida a amarillo/naranja cuando está furioso
        barraVidaEnemigo->setStyleSheet(
            "QProgressBar {"
            "   border: 2px solid #555;"
            "   border-radius: 5px;"
            "   background-color: #2b2b2b;"
            "}"
            "QProgressBar::chunk {"
            "   background-color: #FF6B00;" // Naranja intenso
            "   border-radius: 3px;"
            "}"
            );
    }

    QTimer::singleShot(1000, this, [this, modoFuria]() {
        int danio = 0;

        if (modoFuria) {
            // En modo FURIA el enemigo es MÁS AGRESIVO
            // 70% de probabilidad de usar habilidad especial
            int probabilidad = QRandomGenerator::global()->bounded(0, 100);

            if (probabilidad < 70) {
                // Usar habilidad poderosa
                danio = enemigo->usarHabilidadAleatoria();

                // BONUS: En furia puede atacar 2 veces (30% de probabilidad)
                if (QRandomGenerator::global()->bounded(0, 100) < 30) {
                    int danioExtra = enemigo->atacar();
                    danio += danioExtra;
                    mostrarMensaje(QString("¡%1 está FURIOSO!\n%2\n¡Ataque doble! Daño total: %3")
                                       .arg(enemigo->getNombre())
                                       .arg(enemigo->getUltimaAccion())
                                       .arg(danio));
                } else {
                    mostrarMensaje(QString("¡%1 está FURIOSO!\n%2\nDaño: %3")
                                       .arg(enemigo->getNombre())
                                       .arg(enemigo->getUltimaAccion())
                                       .arg(danio));
                }
            } else {
                danio = enemigo->decidirAccion();
                mostrarMensaje(QString("¡%1 está FURIOSO!\n%2\nDaño: %3")
                                   .arg(enemigo->getNombre())
                                   .arg(enemigo->getUltimaAccion())
                                   .arg(danio));
            }
        } else {
            // Comportamiento normal del enemigo
            danio = enemigo->decidirAccion();

            QString accion = enemigo->getUltimaAccion();
            if (danio > 0) {
                mostrarMensaje(accion + QString("\nDaño: %1").arg(danio));
            } else {
                mostrarMensaje(accion);
            }
        }

        // Aplicar daño al jugador
        if (danio > 0) {
            jugador->recibirDanio(danio);
        }

        // Verificar si el jugador murió
        if (!jugador->estaVivo()) {
            QTimer::singleShot(1500, this, [this]() {
                finalizarCombate(false);
            });
            return;
        }

        // Pasar el turno al jugador después de 2.5 segundos
        QTimer::singleShot(2500, this, &Combate::turnoJugador);
    });
}

void Combate::botonAtacarClicked()
{
    if (!turnoDelJugador) return;

    deshabilitarBotones();

    int danio = jugador->ataqueBasico();
    enemigo->recibirDanio(danio);

    mostrarMensaje(QString("¡Atacaste al enemigo!\n¡Causaste %1 de daño!").arg(danio));

    if (!enemigo->estaVivo()) {
        QTimer::singleShot(1500, this, [this]() {
            finalizarCombate(true);
        });
        return;
    }

    // Turno del enemigo
    QTimer::singleShot(1500, this, &Combate::turnoEnemigo);
}

void Combate::botonDefenderClicked()
{
    if (!turnoDelJugador) return;

    deshabilitarBotones();

    jugador->defender();
    mostrarMensaje("Te preparas para defenderte\n¡Recuperaste 5 de energía!");

    // Turno del enemigo
    QTimer::singleShot(1500, this, &Combate::turnoEnemigo);
}

void Combate::botonHabilidadClicked(int indice)
{
    if (!turnoDelJugador) return;

    deshabilitarBotones();

    int danio = jugador->usarHabilidad(indice);

    if (danio == -1) {
        mostrarMensaje("¡No tienes suficiente energía!");
        QTimer::singleShot(1000, this, &Combate::turnoJugador);
        return;
    }

    Habilidad hab = jugador->getHabilidades()[indice];

    if (danio == 0) {
        // Era una habilidad de curación
        mostrarMensaje(QString("Usaste %1\n¡Te curaste!").arg(hab.nombre));
    } else {
        // Era una habilidad de ataque
        enemigo->recibirDanio(danio);
        mostrarMensaje(QString("Usaste %1\n¡Causaste %2 de daño!").arg(hab.nombre).arg(danio));
    }

    if (!enemigo->estaVivo()) {
        QTimer::singleShot(1500, this, [this]() {
            finalizarCombate(true);
        });
        return;
    }

    // Turno del enemigo
    QTimer::singleShot(1500, this, &Combate::turnoEnemigo);
}

void Combate::actualizarBarraVidaJugador()
{
    int vidaActual = jugador->getVida();
    int vidaMax = jugador->getVidaMaxima();

    barraVidaJugador->setMaximum(vidaMax);
    barraVidaJugador->setValue(vidaActual);
    labelVidaJugador->setText(QString("Vida: %1/%2").arg(vidaActual).arg(vidaMax));
}

void Combate::actualizarBarraVidaEnemigo()
{
    int vidaActual = enemigo->getVida();
    int vidaMax = enemigo->getVidaMaxima();

    barraVidaEnemigo->setMaximum(vidaMax);
    barraVidaEnemigo->setValue(vidaActual);
    labelVidaEnemigo->setText(QString("Vida: %1/%2").arg(vidaActual).arg(vidaMax));
}

void Combate::finalizarCombate(bool victoria)
{
    deshabilitarBotones();

    if (victoria) {
        mostrarMensaje(" ¡VICTORIA! ¡Has derrotado al Enemigo!");
        QTimer::singleShot(2000, this, [this]() {
            emit combateTerminado(true);
            close();
        });
    } else {
        mostrarMensaje(" Has sido derrotado...");
        QTimer::singleShot(2000, this, [this]() {
            emit combateTerminado(false);
            close();
        });
    }
}

void Combate::mostrarMensaje(const QString &mensaje)
{
    labelMensajes->setText(mensaje);
}

void Combate::deshabilitarBotones()
{
    btnAtacar->setEnabled(false);
    btnDefender->setEnabled(false);
    for (QPushButton *btn : botonesHabilidades) {
        btn->setEnabled(false);
    }
}

void Combate::habilitarBotones()
{
    btnAtacar->setEnabled(true);
    btnDefender->setEnabled(true);
    for (QPushButton *btn : botonesHabilidades) {
        btn->setEnabled(true);
    }
}
