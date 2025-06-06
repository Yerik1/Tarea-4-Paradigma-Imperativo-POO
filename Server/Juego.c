#include "Juego.h"
#include "mapa.h"
#include "enemigos.h"
#include "vidas.h"
#include "frutas.h"
#include "puntaje.h"
#
#include <stdio.h>

void iniciar_juego(Partida* juego) {
    juego->num_jugadores = 2;

    inicializar_mapa();
    generar_lechugas();
    inicializar_vidas(3);

    inicializar_jugador(&juego->jugadores[0], 27, 1); // Jugador 1
    inicializar_jugador(&juego->jugadores[1], 27, 2); // Jugador 2

    inicializar_enemigos();
    crear_foca(11, DERECHA);
    crear_yeti(13, IZQUIERDA);
    crear_ave(15);
    crear_hielo(11, 12);
    imprimir_mapa(juego->jugadores, juego->num_jugadores);
}

void turno_juego(Partida* juego) {
    mover_jugador(&juego->jugadores[0], 1, 0); // Jugador 1 derecha
    mover_jugador(&juego->jugadores[1], 2, 1); // Jugador 2 izquierda

    printf("Puntaje actual: %d\n", obtener_puntaje());
    printf("Vidas Jugador 1: %d\n", obtener_vidas(0));
    printf("Vidas Jugador 2: %d\n", obtener_vidas(1));

    imprimir_mapa(juego->jugadores, juego->num_jugadores);
}

void mover(Partida* juego, Jugador* jugador, int movimiento, int aire ) {
    mover_jugador(jugador, movimiento, aire);
    imprimir_mapa(juego->jugadores, juego->num_jugadores);
}

