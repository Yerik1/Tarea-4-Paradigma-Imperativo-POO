#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "jugador.h"
#include "enemigos.h"

typedef struct {
    int matriz[28][20];
    Jugador jugadores[2];
    int num_jugadores;
    int vidas_jugador[2];
    int puntaje;
    Enemigo enemigos[MAX_ENEMIGOS];
    // Agrega aquí cualquier otro estado necesario (frutas, bonus, etc.)
} GameState;

#endif
