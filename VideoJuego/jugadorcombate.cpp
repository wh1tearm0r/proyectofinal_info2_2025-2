#include "jugadorcombate.h"
#include <QRandomGenerator>

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
    QPixmap sprite(":/imagenes/Texxturas/SpriteQuieto.png");
    setPixmap(sprite.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Inicializar habilidades
    inicializarHabilidades();
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
    defendiendo = false;

    int variacion = QRandomGenerator::global()->bounded(-3, 4);
    int danio = ataque + variacion;

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
        curar(-hab.danio); // Convertir a positivo
        return 0; // No causa daño al enemigo
    }

    // Si es una habilidad de ataque
    int variacion = QRandomGenerator::global()->bounded(-5, 6);
    int danio = hab.danio + variacion;

    return danio > 0 ? danio : 1;
}

void jugadorCombate::defender()
{
    defendiendo = true;

    // Recuperar un poco de maná al defender
    energia += 5;
    if (energia > energiaMaxima) {
        energia = energiaMaxima;
    }

    emit energiaCambio(energia, energiaMaxima);
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
    vida += cantidad;
    if (vida > vidaMaxima) {
        vida = vidaMaxima;
    }

    emit vidaCambio(vida, vidaMaxima);
}

bool jugadorCombate::estaVivo() const
{
    return vida > 0;
}


