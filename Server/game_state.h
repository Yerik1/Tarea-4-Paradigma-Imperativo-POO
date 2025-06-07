#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "jugador.h"
#include "enemigos.h"
// Si en el futuro tienes structs para frutas, bonus, etc., inclúyelos aquí
// #include "frutas.h"

// Estructura que encapsula TODO el estado de un juego independiente

typedef struct GameState {
    int matriz[28][20];
    Jugador jugadores[2];
    int num_jugadores;
    int vidas_jugador[2];
    int puntaje;
    Enemigo enemigos[MAX_ENEMIGOS];
    int velocidad_enemigos; // Nueva variable para velocidad de enemigos
    // Si tienes un arreglo de frutas, agrégalo aquí
    // Fruta frutas[MAX_FRUTAS];
    // Agrega aquí cualquier otro estado necesario (bonus, temporizadores, etc.)
} GameState;

#endif
