#include "enemigo.h"
#include <QRandomGenerator>

Enemigo::Enemigo(QString nombre, int vidaMax, int ataque, int defensa,
                 QString rutaSprite, int nivel, QGraphicsItem *parent)
    : Personaje(parent), nombre(nombre), vidaMaxima(vidaMax),
    ataque(ataque), defensa(defensa), nivel(nivel), defendiendo(false)
{
    this->vida = vidaMax;

    QPixmap sprite(rutaSprite);
    if (sprite.isNull()) {
        // Si no hay sprite, crear uno por defecto
        sprite = QPixmap(150, 150);
        sprite.fill(QColor(200, 50, 50)); // Rojo oscuro
    }
    setPixmap(sprite.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    inicializarHabilidades();
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
    defendiendo = false;
    ultimaAccion = nombre + " ataca!";

    // Daño base + variación aleatoria (±20%)
    int variacion = QRandomGenerator::global()->bounded(-ataque/5, ataque/5 + 1);
    int danio = ataque + variacion;

    return danio > 0 ? danio : 1;
}

int Enemigo::usarHabilidadAleatoria()
{
    if (habilidades.isEmpty()) {
        return atacar();
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
    defendiendo = true;
}

bool Enemigo::estaVivo() const
{
    return vida > 0;
}
