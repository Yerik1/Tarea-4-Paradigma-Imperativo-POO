#include <stdio.h>
#include "mapa.h"
#include "jugador.h"
#include "enemigos.h"
#include "vidas.h"
#include "frutas.h"
#include "puntaje.h"

int main() {
    Jugador jugadores[2];
    int num_jugadores = 2;
    inicializar_mapa();
    inicializar_vidas(3);
    inicializar_jugador(&jugadores[0], 1, 9); // Jugador 1 inicia abajo, columna 9
    inicializar_jugador(&jugadores[1], 1, 10); // Jugador 2 inicia abajo, columna 10

    mover_jugador(jugadores, 0, 3, num_jugadores); // Jugador 1 salta
    imprimir_mapa(jugadores, num_jugadores);

    // ...puedes seguir simulando más movimientos y mostrando el mapa...

    return 0;
}