#ifndef JUEGO_H
#define JUEGO_H

#include "jugador.h"


typedef struct {
    Jugador jugadores[2];
    int num_jugadores;
} Partida;

void iniciar_juego(Partida* juego);
void turno_juego(Partida* juego);
void mover(Partida* juego, Jugador* jugador, int movimiento, int aire );

#endif // JUEGO_H
