#include <stdio.h>
#include "vidas.h"
#include "jugador.h"
#include "mapa.h"
#include "puntaje.h"
#include "game_state.h"

void inicializar_vidas(GameState* game, int vidas) {
    game->vidas_jugador[0] = vidas;
    game->vidas_jugador[1] = vidas;
}

int obtener_vidas(GameState* game, int idx) {
    return game->vidas_jugador[idx];
}

void perder_vida(GameState* game, int idx) {
    if (game->vidas_jugador[idx] > 0) {
        game->vidas_jugador[idx]--;
        // Recoloca al jugador en el mapa si es necesario
        colocar_en_mapa(game, game->jugadores[idx].fila, game->jugadores[idx].columna, 1);
    }
}

int esta_vivo(GameState* game, int idx) {
    return game->vidas_jugador[idx] > 0;
}

void reiniciar_ambos_jugadores_si_uno_muere(GameState* game) {
    int alguno_sin_vidas = 0;
    for (int i = 0; i < game->num_jugadores; i++) {
        if (!esta_vivo(game, i)) {
            alguno_sin_vidas = 1;
            break;
        }
    }
    if (alguno_sin_vidas) {
        for (int i = 0; i < game->num_jugadores; i++) {
            game->jugadores[i].fila = 27;
            game->jugadores[i].columna = 1 + i;
            colocar_en_mapa(game, game->jugadores[i].fila, game->jugadores[i].columna, 1);
        }
        printf("Ambos jugadores han sido devueltos al primer nivel por quedarse sin vidas.\n");
    }
}

void penalizar_jugador_rezagado(GameState* game) {
    if (game->num_jugadores < 2) return;
    int idx_mas_alto = (game->jugadores[0].fila < game->jugadores[1].fila) ? 0 : 1;
    int idx_mas_bajo = 1 - idx_mas_alto;
    int diferencia = game->jugadores[idx_mas_bajo].fila - game->jugadores[idx_mas_alto].fila;
    if (diferencia > 3) {
        printf("El jugador %d está rezagado por más de 3 niveles. Pierde una vida y sube a la fila del jugador más alto.\n", idx_mas_bajo+1);
        perder_vida(game, idx_mas_bajo);
        game->jugadores[idx_mas_bajo].fila = game->jugadores[idx_mas_alto].fila;
        colocar_en_mapa(game, game->jugadores[idx_mas_bajo].fila, game->jugadores[idx_mas_bajo].columna, 1);
    }
}

void penalizar_jugador_rezagado_avanzado(GameState* game) {
    if (game->num_jugadores < 2) return;
    int idx_mas_alto = (game->jugadores[0].fila < game->jugadores[1].fila) ? 0 : 1;
    int idx_mas_bajo = 1 - idx_mas_alto;
    int diferencia = game->jugadores[idx_mas_bajo].fila - game->jugadores[idx_mas_alto].fila;
    if (diferencia > 5) {
        printf("El jugador %d está rezagado por más de 5 filas. Pierde una vida y sube a la fila del jugador más alto.\n", idx_mas_bajo+1);
        perder_vida(game, idx_mas_bajo);
        eliminar_del_mapa(game, game->jugadores[idx_mas_bajo].fila, game->jugadores[idx_mas_bajo].columna);
        int fila_destino = game->jugadores[idx_mas_alto].fila;
        int columna_destino = -1;
        for (int col = 0; col < 20; col++) {
            if (obtener_mapa(game, fila_destino, col) == 0) {
                columna_destino = col;
                break;
            }
        }
        if (columna_destino == -1) {
            printf("No hay casilla vacía en la fila %d para colocar al jugador rezagado.\n", fila_destino);
            return;
        }
        int fila_final = fila_destino;
        while (fila_final < 27) {
            int abajo = obtener_mapa(game, fila_final + 1, columna_destino);
            if (abajo == 2 || abajo == 3) break;
            fila_final++;
        }
        game->jugadores[idx_mas_bajo].fila = fila_final;
        game->jugadores[idx_mas_bajo].columna = columna_destino;
        colocar_en_mapa(game, game->jugadores[idx_mas_bajo].fila, game->jugadores[idx_mas_bajo].columna, 1);
    }
}

void sumar_vida_a_ambos_jugadores(GameState* game) {
    for (int i = 0; i < 2; i++) {
        if (game->vidas_jugador[i] < 99) game->vidas_jugador[i]++;
    }
}

void sumar_vida_jugador(GameState* game, int idx) {
    if (game->vidas_jugador[idx] < 99) game->vidas_jugador[idx]++;
}

void reiniciar_juego_si_uno_muere(GameState* game) {
    if (obtener_vidas(game, 0) == 0 || obtener_vidas(game, 1) == 0) {
        printf("\n¡Has perdido! Ambos jugadores vuelven al inicio.\n");
        colocar_en_mapa(game, game->jugadores[0].fila, game->jugadores[0].columna, 0);
        colocar_en_mapa(game, game->jugadores[1].fila, game->jugadores[1].columna, 0);
        inicializar_vidas(game, 3);
        reiniciar_puntaje(game);
        game->jugadores[0].fila = 27;
        game->jugadores[1].fila = 27;
        colocar_en_mapa(game, game->jugadores[0].fila, game->jugadores[0].columna, 1);
        colocar_en_mapa(game, game->jugadores[1].fila, game->jugadores[1].columna, 1);
        // Aquí deberías reiniciar enemigos y frutas para este game si es necesario
        // inicializar_enemigos(game);
        // generar_naranjas(game);
        // generar_lechugas(game);
        imprimir_mapa(game);
    }
}

