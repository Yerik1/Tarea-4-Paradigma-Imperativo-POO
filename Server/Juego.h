#ifndef JUEGO_H
#define JUEGO_H

#include "jugador.h"
#include "enemigos.h"


typedef struct Partida{
    int matriz[28][20];
    Jugador jugadores[2];
    int num_jugadores;
    int vidas_jugador[2];
    int puntaje;
    Enemigo enemigos[MAX_ENEMIGOS];
    int velocidad_enemigos;
} Partida;

void iniciar_juego(Partida* juego);
void turno_juego(Partida* juego);
void mover(Partida* juego, int jugador, int movimiento);

#endif // JUEGO_H
