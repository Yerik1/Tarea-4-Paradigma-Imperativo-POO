#include <stdio.h>
#include "mapa.h"
#include "enemigos.h"
#include "game_state.h"

#define RESET   "\x1b[0m"
#define ROJO    "\x1b[31m"
#define VERDE   "\x1b[32m"
#define AMARILLO "\x1b[33m"
#define AZUL    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define BLANCO  "\x1b[37m"

// Todas las funciones ahora operan sobre GameState*

void inicializar_mapa(GameState* game) {
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 20; j++) {
            game->matriz[i][j] = 0;
            if(i % 2 == 0 && i > 9) game->matriz[i][j] = 2;
            if(i == 0 && (j < 8 || j > 11)) game->matriz[i][j] = 3;
            if(i % 2 == 0 && i <= 9 && i != 0){
                if(i == 2 || i == 6){
                    if(j > 7 && j < 12){
                        game->matriz[i][j] = 3;
                    }
                } else {
                    if(j > 2 && j < 8){
                        game->matriz[i][j] = 3;
                    }
                    if(j > 11 && j < 17){
                        game->matriz[i][j] = 3;
                    }
                }
            }
        }
    }
}

void colocar_en_mapa(GameState* game, int fila, int columna, int valor) {
    game->matriz[fila][columna] = valor;
}

void eliminar_del_mapa(GameState* game, int fila, int columna) {
    if (game->matriz[fila][columna] == 9 || game->matriz[fila][columna] == 10 || game->matriz[fila][columna] == 11 || game->matriz[fila][columna] == 12) {
        if (fila >= 0 && (game->matriz[fila][columna] == 9 || game->matriz[fila][columna] == 10 || game->matriz[fila][columna] == 11 || game->matriz[fila][columna] == 12)) {
            if ((fila % 2 == 0 && fila > 9) || game->matriz[fila][columna] == 2) {
                game->matriz[fila][columna] = 2;
            } else if (game->matriz[fila][columna] == 3) {
                game->matriz[fila][columna] = 3;
            } else {
                game->matriz[fila][columna] = 0;
            }
        } else {
            game->matriz[fila][columna] = 0;
        }
    } else if (game->matriz[fila][columna] == 1) {
        game->matriz[fila][columna] = 0;
    }
}

int obtener_mapa(GameState* game, int fila, int columna) {
    return game->matriz[fila][columna];
}

void imprimir_mapa(GameState* game) {
    // Aquí deberías actualizar enemigos y jugadores en el mapa si es necesario
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 20; j++) {
            if (game->matriz[i][j] == 1) {
                printf(VERDE "J " RESET);
            } else if (game->matriz[i][j] == 2) {
                printf(AZUL "# " RESET);
            } else if (game->matriz[i][j] == 3) {
                printf(MAGENTA "B " RESET);
            } else if (game->matriz[i][j] == 9) {
                printf(ROJO "Y " RESET);
            } else if (game->matriz[i][j] == 10) {
                printf(BLANCO "F " RESET);
            } else if (game->matriz[i][j] == 11) {
                printf(AMARILLO "A " RESET);
            } else if (game->matriz[i][j] == 15) {
                printf(AMARILLO "B " RESET);
            } else if (game->matriz[i][j] == 16) {
                printf("N ");
            } else if (game->matriz[i][j] == 13) {
                printf(MAGENTA "B " RESET);
            } else if (game->matriz[i][j] == 14) {
                printf("L ");
            } else if (game->matriz[i][j] == 12) {
                printf(CYAN "H " RESET);
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}
