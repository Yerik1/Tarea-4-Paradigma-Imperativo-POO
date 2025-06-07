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

    inicializar_mapa(juego);
    inicializar_vidas(juego,3);
    inicializar_jugador(juego,0, 27, 1); // Jugador 1
    inicializar_jugador(juego,1, 27, 2); // Jugador 2
    generar_lechugas(juego);




    inicializar_enemigos(juego);
    crear_foca(juego,11, DERECHA);
    crear_yeti(juego,13, IZQUIERDA);
    crear_ave(juego,15);
    crear_hielo(juego,11, 12);
    imprimir_mapa(juego);
}

void turno_juego(Partida* juego) {
    mover_jugador(juego, 1, 0); // Jugador 1 derecha
    mover_jugador(juego, 2, 1); // Jugador 2 izquierda

    printf("Puntaje actual: %d\n", obtener_puntaje(juego));
    printf("Vidas Jugador 1: %d\n", obtener_vidas(juego,0));
    printf("Vidas Jugador 2: %d\n", obtener_vidas(juego,1));

    imprimir_mapa(juego);
}

void mover(Partida* juego, int jugador, int movimiento) {
    mover_jugador(juego, jugador, movimiento);
    imprimir_mapa(juego);
}

