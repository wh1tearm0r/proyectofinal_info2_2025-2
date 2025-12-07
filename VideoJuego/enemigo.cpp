#include "enemigo.h"
#include <QRandomGenerator>
#include <QTimer>
#include <QDebug>

Enemigo::Enemigo(QString nombre, int vidaMax, int ataque, int defensa,
                 QString rutaSprite, int nivel, QGraphicsItem *parent)
    : Personaje(parent), nombre(nombre), vidaMaxima(vidaMax),
    ataque(ataque), defensa(defensa), nivel(nivel), defendiendo(false),
    estadoActual(QUIETO), frameActual(0)
{
    vida = vidaMaxima;

    // Animación inicial (quieto)
    cargarAnimacion(":/imagenes/Texxturas/spritessoldadoquieto.png", 3);
    setPixmap(frames[0]);

    // Timer de animación
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Enemigo::actualizarAnimacion);
    timerAnimacion->start(120);

    inicializarHabilidades();
}

void Enemigo::cargarAnimacion(const QString &ruta, int columnas)
{
    frames.clear();
    QPixmap spriteSheet(ruta);
    if (spriteSheet.isNull()) {
        qWarning() << "Error al cargar sprite enemigo:" << ruta;
        QPixmap placeholder(120, 120);
        placeholder.fill(Qt::darkRed);
        frames.append(placeholder);
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

void Enemigo::reproducirAnimacion(EstadoAnimacion nuevoEstado)
{
    estadoActual = nuevoEstado;
    frameActual = 0;

    switch (estadoActual) {
    case QUIETO:
        cargarAnimacion(":/imagenes/Texxturas/spritessoldadoquieto.png", 3);
        break;
    case ATAQUE:
        cargarAnimacion(":/imagenes/Texxturas/spritessoldadoataques.png", 6);
        break;
    case DEFENSA:
        cargarAnimacion(":/imagenes/Texxturas/spritessoldadodefender.png", 6);
        break;
    case CURACION:
        cargarAnimacion(":/imagenes/Texxturas/spritessoldadocuracion.png", 6);
        break;
    case CRITICO:
        cargarAnimacion(":/imagenes/Texxturas/spritessoldadocritico.png", 6);
        break;
    case RAPIDO:
        cargarAnimacion(":/imagenes/Texxturas/spritessoldadoataquerapido.png", 6);
        break;
    }

    setPixmap(frames[0]);
}

void Enemigo::actualizarAnimacion()
{
    if (frames.isEmpty()) return;
    frameActual = (frameActual + 1) % frames.size();
    setPixmap(frames[frameActual]);
}

void Enemigo::inicializarHabilidades()
{
    habilidades.clear();

    // Las habilidades varían según el nivel del enemigo
    if (nivel >= 1) {
        HabilidadEnemigo hab1;
        hab1.nombre = "Golpe Salvaje";
        hab1.danioBase = ataque + 10;
        hab1.probabilidad = 30;
        hab1.descripcion = "Un golpe poderoso";
        habilidades.append(hab1);
    }

    if (nivel >= 2) {
        HabilidadEnemigo hab2;
        hab2.nombre = "Embestida";
        hab2.danioBase = ataque + 15;
        hab2.probabilidad = 20;
        hab2.descripcion = "Una embestida brutal";
        habilidades.append(hab2);
    }

    if (nivel >= 3) {
        HabilidadEnemigo hab3;
        hab3.nombre = "Ataque Devastador";
        hab3.danioBase = ataque + 25;
        hab3.probabilidad = 15;
        hab3.descripcion = "El ataque más poderoso";
        habilidades.append(hab3);
    }
}

void Enemigo::mover(int dx, int dy)
{
    // No se mueve en combate por turnos
}

void Enemigo::actualizarEstado()
{
    // Actualizar animación si es necesario
}

int Enemigo::decidirAccion()
{
    // El enemigo decide qué hacer basándose en su IA
    int vidaPorcentaje = (vida * 100) / vidaMaxima;

    // Si tiene poca vida (menos del 30%), puede defenderse
    if (vidaPorcentaje < 30) {
        int decision = QRandomGenerator::global()->bounded(0, 100);
        if (decision < 40) { // 40% de probabilidad de defenderse
            defender();
            ultimaAccion = nombre + " se defiende!";
            return 0; // No hace daño
        }
    }

    // Intentar usar una habilidad especial
    int intentoHabilidad = QRandomGenerator::global()->bounded(0, 100);

    for (const HabilidadEnemigo &hab : habilidades) {
        if (intentoHabilidad < hab.probabilidad) {
            int danio = usarHabilidadAleatoria();
            return danio;
        }
    }

    // Si no usa habilidad, ataque normal
    return atacar();
}

int Enemigo::atacar()
{
    reproducirAnimacion(ATAQUE);

    defendiendo = false;
    ultimaAccion = nombre + " ataca!";

    // Daño base + variación aleatoria (±20%)
    int variacion = QRandomGenerator::global()->bounded(-ataque/5, ataque/5 + 1);
    int danio = ataque + variacion;
    QTimer::singleShot(1000, this, [this]() { reproducirAnimacion(QUIETO); });

    return danio > 0 ? danio : 1;
}

int Enemigo::usarHabilidadAleatoria()
{
    if (habilidades.isEmpty()) {
        return atacar();
        reproducirAnimacion(ATAQUE);
        QTimer::singleShot(1000, this, [this]() { reproducirAnimacion(QUIETO); });
    }

    // Seleccionar una habilidad aleatoria
    int indice = QRandomGenerator::global()->bounded(0, habilidades.size());
    HabilidadEnemigo &hab = habilidades[indice];

    defendiendo = false;
    ultimaAccion = nombre + " usa " + hab.nombre + "!";

    // Calcular daño con variación
    int variacion = QRandomGenerator::global()->bounded(-5, 6);
    int danio = hab.danioBase + variacion;

    return danio > 0 ? danio : 1;
}

void Enemigo::recibirDanio(int danio)
{
    int danioReal = danio;

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

void Enemigo::defender()
{
    reproducirAnimacion(DEFENSA);
    defendiendo = true;
    QTimer::singleShot(1000, this, [this]() { reproducirAnimacion(QUIETO); });
}

bool Enemigo::estaVivo() const
{
    return vida > 0;
}
