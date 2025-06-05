#include <stdio.h>
#include "mapa.h"
#include "jugador.h"
#include "enemigos.h"
#include "vidas.h"
#include "frutas.h"
#include "puntaje.h"

int main() {
    Jugador jugadores[2];
    int num_jugadores = 2; // Ahora dos jugadores
    inicializar_mapa();
    generar_lechugas();
    inicializar_vidas(3);
    inicializar_jugador(&jugadores[0], 27, 1); // Jugador 1
    inicializar_jugador(&jugadores[1], 27, 2); // Jugador 2
    // Ejemplo de movimientos independientes
    mover_jugador(&jugadores[0], 1, 0); // Jugador 1 a la derecha
    mover_jugador(&jugadores[1], 2, 1); // Jugador 2 a la izquierda
    printf("turno 1\n");
    printf("Puntaje actual: %d\n", obtener_puntaje());
    printf("Vidas Jugador 1: %d\n", obtener_vidas(0));
    printf("Vidas Jugador 2: %d\n", obtener_vidas(1));
    inicializar_enemigos();
    crear_foca(11, DERECHA);
    crear_yeti(13, IZQUIERDA);
    crear_ave(15);
    crear_hielo(11, 12);
    printf("turno 2\n");
    imprimir_mapa(jugadores, num_jugadores);
    
    
    /*
    int i;
    for(i = 0; i < 12; i++) {
        mover_jugador(&jugadores[0], 3); // Mover a la derecha
        mover_jugador(&jugadores[0], 3); // Mover hacia abajo
        mover_jugador(&jugadores[0], 1); // Mover hacia abajo
        imprimir_mapa(jugadores, num_jugadores);
    }
    */

    return 0;
}