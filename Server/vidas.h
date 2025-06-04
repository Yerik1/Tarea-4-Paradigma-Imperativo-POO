#ifndef VIDAS_H
#define VIDAS_H

#include "jugador.h"

void inicializar_vidas(int vidas);
int obtener_vidas();
void perder_vida(Jugador* jugador);
int esta_vivo();

#endif
