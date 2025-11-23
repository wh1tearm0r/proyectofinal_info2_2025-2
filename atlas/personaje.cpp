#include "personaje.h"

personaje::personaje() {}

personaje::personaje(bool estaVivo, int cantidadVida)
    : estaVivo(estaVivo),
    cantidadVida(cantidadVida)
{}
