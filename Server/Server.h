#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include "Juego.h"

#define MAX_CLIENTES 6
#define MAX_JUEGOS 2
#define MAX_JUGADORES_POR_JUEGO 2
#define MAX_OBSERVADORES_POR_JUEGO 2

// Estructura para representar un juego
typedef struct {
    Partida juego;
    SOCKET jugadores[MAX_JUGADORES_POR_JUEGO];
    SOCKET observadores[MAX_OBSERVADORES_POR_JUEGO];
    int num_jugadores;
    int num_observadores;
    int activo;
} Juego;

// Arreglo global de juegos
extern Juego juegos[MAX_JUEGOS];

// Funciones principales
void inicializar_juegos();
void procesar_comando(SOCKET cliente, const char* buffer);
void start();

#endif
