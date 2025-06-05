#include <stdio.h>
#include "vidas.h"
#include "jugador.h"
#include "mapa.h"

static int vidas_jugador[2] = {3, 3};

void inicializar_vidas(int vidas) {
    vidas_jugador[0] = vidas;
    vidas_jugador[1] = vidas;
}

int obtener_vidas(int idx) {
    return vidas_jugador[idx];
}

void perder_vida(Jugador* jugador, int idx) {
    if (vidas_jugador[idx] > 0) {
        vidas_jugador[idx]--;
        colocar_en_mapa(jugador->fila, jugador->columna, 1);
    }
}

int esta_vivo(int idx) {
    return vidas_jugador[idx] > 0;
}

void reiniciar_ambos_jugadores_si_uno_muere(Jugador* jugadores, int num_jugadores) {
    int alguno_sin_vidas = 0;
    for (int i = 0; i < num_jugadores; i++) {
        if (!esta_vivo(i)) {
            alguno_sin_vidas = 1;
            break;
        }
    }
    if (alguno_sin_vidas) {
        for (int i = 0; i < num_jugadores; i++) {
            jugadores[i].fila = 27;
            jugadores[i].columna = 1 + i; // posición diferente para cada jugador
            colocar_en_mapa(jugadores[i].fila, jugadores[i].columna, 1);
        }
        printf("Ambos jugadores han sido devueltos al primer nivel por quedarse sin vidas.\n");
    }
}

// Si hay dos jugadores y uno le lleva más de 3 niveles al otro, el que está más abajo pierde una vida y sube a la fila del más alto
void penalizar_jugador_rezagado(Jugador* jugadores, int num_jugadores) {
    if (num_jugadores < 2) return;
    int idx_mas_alto = (jugadores[0].fila < jugadores[1].fila) ? 0 : 1;
    int idx_mas_bajo = 1 - idx_mas_alto;
    int diferencia = jugadores[idx_mas_bajo].fila - jugadores[idx_mas_alto].fila;
    if (diferencia > 3) {
        printf("El jugador %d está rezagado por más de 3 niveles. Pierde una vida y sube a la fila del jugador más alto.\n", idx_mas_bajo+1);
        perder_vida(&jugadores[idx_mas_bajo], idx_mas_bajo);
        jugadores[idx_mas_bajo].fila = jugadores[idx_mas_alto].fila;
        colocar_en_mapa(jugadores[idx_mas_bajo].fila, jugadores[idx_mas_bajo].columna, 1);
    }
}

void sumar_vida_a_ambos_jugadores() {
    for (int i = 0; i < 2; i++) {
        if (vidas_jugador[i] < 99) vidas_jugador[i]++;
    }
}
