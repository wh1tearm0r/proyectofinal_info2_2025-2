#include "jugadorcombate.h"
#include <QRandomGenerator>
#include <QDebug>

jugadorCombate::jugadorCombate(QGraphicsItem *parent) : Personaje(parent){
    // Estadísticas iniciales
    vidaMaxima = 100;
    vida = vidaMaxima;
    energiaMaxima = 50;
    energia = energiaMaxima;
    ataque = 15;
    defensa = 5;
    velocidad = 10;
    defendiendo = false;

    // Cargar sprite del jugador
    cargarAnimacion(":/imagenes/Texxturas/spritespeleaquieto.png", 3);
    setPixmap(frames[0]);

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &jugadorCombate::actualizarAnimacion);
    timerAnimacion->start(120);

    // Inicializar habilidades
    inicializarHabilidades();
}

void jugadorCombate::cargarAnimacion(const QString &ruta, int columnas)
{
    frames.clear();
    QPixmap spriteSheet(ruta);
    if (spriteSheet.isNull()) {
        qWarning() << "No se pudo cargar:" << ruta;
        return;
    }

    int anchoFrame = spriteSheet.width() / columnas;
    int altoFrame = spriteSheet.height();

    for (int i = 0; i < columnas; ++i) {
        QPixmap frame = spriteSheet.copy(i * anchoFrame, 0, anchoFrame, altoFrame)
        .scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        frames.append(frame);
    }
}

void jugadorCombate::reproducirAnimacion(EstadoAnimacion nuevoEstado)
{
    estadoActual = nuevoEstado;
    frameActual = 0;

    switch (estadoActual) {
    case QUIETO:
        cargarAnimacion(":/imagenes/Texxturas/spritespeleaquieto.png", 3);
        break;
    case ATAQUE_BASICO:
        cargarAnimacion(":/imagenes/Texxturas/spritespeleaataque.png", 5);
        break;
    case ATAQUE_RAPIDO:
        cargarAnimacion(":/imagenes/Texxturas/spritespeleaataquerapido.png", 6);
        break;
    case GOLPE_CRITICO:
        cargarAnimacion(":/imagenes/Texxturas/spritespeleacritico.png", 6);
        break;
    case CURACION:
        cargarAnimacion(":/imagenes/Texxturas/spritespeleacuracion.png", 8);
        break;
    case DEFENSA:
        cargarAnimacion(":/imagenes/Texxturas/spritespeleadefensa.png", 7);
        break;
    }

    setPixmap(frames[0]);
}

void jugadorCombate::actualizarAnimacion()
{
    if (frames.isEmpty()) return;
    frameActual = (frameActual + 1) % frames.size();
    setPixmap(frames[frameActual]);
}


void jugadorCombate::inicializarHabilidades()
{
    // Habilidad 2: Ataque Rápido
    Habilidad ataqueRapido;
    ataqueRapido.nombre = "Ataque Rápido";
    ataqueRapido.danio = 12;
    ataqueRapido.costoEnergia = 5;
    ataqueRapido.descripcion = "Un ataque veloz con menor daño";
    habilidades.append(ataqueRapido);

    // Habilidad 3: Golpe Crítico
    Habilidad golpeCritico;
    golpeCritico.nombre = "Golpe Crítico";
    golpeCritico.danio = 35;
    golpeCritico.costoEnergia = 20;
    golpeCritico.descripcion = "Un golpe devastador que consume mucho maná";
    habilidades.append(golpeCritico);

    // Habilidad 4: Curación Menor
    Habilidad curacionMenor;
    curacionMenor.nombre = "Curación Menor";
    curacionMenor.danio = -20; // Negativo porque cura
    curacionMenor.costoEnergia = 15;
    curacionMenor.descripcion = "Restaura 20 puntos de vida";
    habilidades.append(curacionMenor);
}

void jugadorCombate::mover(int dx, int dy)
{
    //Combate por turnos, por lo tanto no hay movimientos, sin embargo, se pueden agregar animaciones
}

void jugadorCombate::actualizarEstado()
{
    //Actualizar animaciones
}

int jugadorCombate::ataqueBasico()

{
    reproducirAnimacion(ATAQUE_BASICO);
    defendiendo = false;

    int variacion = QRandomGenerator::global()->bounded(-3, 4);
    int danio = ataque + variacion;

    QTimer::singleShot(800, this, [this]() { reproducirAnimacion(QUIETO); });

    return danio > 0 ? danio : 1; // Mínimo 1 de daño
}

int jugadorCombate::usarHabilidad(int indiceHabilidad)
{
    if (indiceHabilidad < 0 || indiceHabilidad >= habilidades.size()) {
        return 0;
    }

    Habilidad &hab = habilidades[indiceHabilidad];

    // Verificar si tiene suficiente energia
    if (energia < hab.costoEnergia) {
        return -1; // Código de error: no hay suficiente energia
    }

    // Consumir energia
    energia -= hab.costoEnergia;
    emit energiaCambio(energia, energiaMaxima);

    defendiendo = false;

    // Si es una habilidad de curación (daño negativo)
    if (hab.danio < 0) {
        reproducirAnimacion(CURACION);
        curar(-hab.danio); // Convertir a positivo
        return 0; // No causa daño al enemigo
    } else if (hab.nombre == "Ataque Rápido") {
        reproducirAnimacion(ATAQUE_RAPIDO);
    } else if (hab.nombre == "Golpe Crítico") {
        reproducirAnimacion(GOLPE_CRITICO);
    }

    QTimer::singleShot(800, this, [this]() { reproducirAnimacion(QUIETO); });

    // Si es una habilidad de ataque
    int variacion = QRandomGenerator::global()->bounded(-5, 6);
    int danio = hab.danio + variacion;

    return danio > 0 ? danio : 1;
}

void jugadorCombate::defender()
{
    reproducirAnimacion(DEFENSA);
    defendiendo = true;

    // Recuperar un poco de maná al defender
    energia += 5;
    if (energia > energiaMaxima) {
        energia = energiaMaxima;
    }

    emit energiaCambio(energia, energiaMaxima);

    QTimer::singleShot(1000, this, [this]() { reproducirAnimacion(QUIETO); });
}

void jugadorCombate::recibirDanio(int danio)
{
    int danioReal = danio;

    // Si está defendiendo, reduce el daño
    if (defendiendo) {
        danioReal = danio - defensa;
        if (danioReal < 1) danioReal = 1; // Mínimo 1 de daño
        defendiendo = false;
    }

    vida -= danioReal;
    if (vida < 0) vida = 0;

    emit vidaCambio(vida, vidaMaxima);

    if (vida <= 0) {
        emit murio();
    }
}

void jugadorCombate::curar(int cantidad)
{
    reproducirAnimacion(CURACION);
    vida += cantidad;
    if (vida > vidaMaxima) {
        vida = vidaMaxima;
    }

    emit vidaCambio(vida, vidaMaxima);
    QTimer::singleShot(1000, this, [this]() { reproducirAnimacion(QUIETO); });
}

bool jugadorCombate::estaVivo() const
{
    return vida > 0;
}


