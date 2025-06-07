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

// Penaliza al jugador rezagado si la diferencia de filas es mayor a 5
void penalizar_jugador_rezagado_avanzado(Jugador* jugadores, int num_jugadores) {
    if (num_jugadores < 2) return;
    int idx_mas_alto = (jugadores[0].fila < jugadores[1].fila) ? 0 : 1;
    int idx_mas_bajo = 1 - idx_mas_alto;
    int diferencia = jugadores[idx_mas_bajo].fila - jugadores[idx_mas_alto].fila;
    if (diferencia > 5) {
        printf("El jugador %d está rezagado por más de 5 filas. Pierde una vida y sube a la fila del jugador más alto.\n", idx_mas_bajo+1);
        perder_vida(&jugadores[idx_mas_bajo], idx_mas_bajo);

        // Borra la posición antigua del jugador rezagado (pone un 0)
        eliminar_del_mapa(jugadores[idx_mas_bajo].fila, jugadores[idx_mas_bajo].columna);

        // Buscar una columna vacía (0) en la fila del jugador más alto
        int fila_destino = jugadores[idx_mas_alto].fila;
        int columna_destino = -1;
        for (int col = 0; col < 20; col++) {
            if (obtener_mapa(fila_destino, col) == 0) {
                columna_destino = col;
                break;
            }
        }
        if (columna_destino == -1) {
            printf("No hay casilla vacía en la fila %d para colocar al jugador rezagado.\n", fila_destino);
            return;
        }

        // Simular caída hasta tocar bloque 2 o 3
        int fila_final = fila_destino;
        while (fila_final < 27) {
            int abajo = obtener_mapa(fila_final + 1, columna_destino);
            if (abajo == 2 || abajo == 3) break;
            fila_final++;
        }

        jugadores[idx_mas_bajo].fila = fila_final;
        jugadores[idx_mas_bajo].columna = columna_destino;
        colocar_en_mapa(jugadores[idx_mas_bajo].fila, jugadores[idx_mas_bajo].columna, 1);
    }
}

void sumar_vida_a_ambos_jugadores() {
    for (int i = 0; i < 2; i++) {
        if (vidas_jugador[i] < 99) vidas_jugador[i]++;
    }
}

void sumar_vida_jugador(int idx) {
    if (vidas_jugador[idx] < 99) vidas_jugador[idx]++;
}

void reiniciar_juego_si_uno_muere(Jugador* jugadores, int num_jugadores) {
    if (obtener_vidas(0) == 0 || obtener_vidas(1) == 0) {
        printf("\n¡Has perdido! Ambos jugadores vuelven al inicio.\n");
        inicializar_vidas(3);
        extern void reiniciar_puntaje();
        reiniciar_puntaje();
        jugadores[0].fila = 27;
        jugadores[1].fila = 27;
        colocar_en_mapa(jugadores[0].fila, jugadores[0].columna, 1);
        colocar_en_mapa(jugadores[1].fila, jugadores[1].columna, 1);
        extern void inicializar_enemigos();
        inicializar_enemigos();
        extern void generar_naranjas();
        extern void generar_lechugas();
        generar_naranjas();
        generar_lechugas();
        imprimir_mapa(jugadores, num_jugadores);
    }
}

