#include <stdio.h>
#include "mapa.h"
#include "enemigos.h"

#define RESET   "\x1b[0m"
#define ROJO    "\x1b[31m"
#define VERDE   "\x1b[32m"
#define AMARILLO "\x1b[33m"
#define AZUL    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define BLANCO  "\x1b[37m"

static int matriz[28][20];

void inicializar_mapa() {
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 20; j++) {
            matriz[i][j] = 0;
            if(i % 2 == 0 && i > 9) matriz[i][j] = 2;
            if(i == 0 && (j < 8 || j > 11)) matriz[i][j] = 3;
            if(i % 2 == 0 && i <= 9 && i != 0){
                if(i == 2 || i == 6){
                    if(j > 7 && j < 12){
                        matriz[i][j] = 3; // colocar bloques en el centro para filas 2 y 6
                    }
                } else {
                    if(j > 2 && j < 8){
                        matriz[i][j] = 3; // colocar bloques a la izquierda
                    }
                    if(j > 11 && j < 17){
                        matriz[i][j] = 3; // colocar bloques a la derecha
                    }
                }
            }
        }
    }
}

void colocar_en_mapa(int fila, int columna, int valor) {
    matriz[fila][columna] = valor;
    
}

void eliminar_del_mapa(int fila, int columna) {
    // Si hay un enemigo (9, 10, 11, 12) y debajo había un bloque, restaura el bloque
    if (matriz[fila][columna] == 9 || matriz[fila][columna] == 10 || matriz[fila][columna] == 11 || matriz[fila][columna] == 12) {
        // Si originalmente había un bloque, restaurar el bloque
        // Revisar si debajo hay un bloque (solo para filas > 0)
        if (fila >= 0 && (matriz[fila][columna] == 9 || matriz[fila][columna] == 10 || matriz[fila][columna] == 11 || matriz[fila][columna] == 12)) {
            // Si la posición actual es un enemigo, pero originalmente era bloque, restaurar bloque
            // Revisar si la fila es par y mayor a 9 (bloques #) o si es bloque B
            if ((fila % 2 == 0 && fila > 9) || matriz[fila][columna] == 2) {
                matriz[fila][columna] = 2; // bloque #
            } else if (matriz[fila][columna] == 3) {
                matriz[fila][columna] = 3; // bloque B
            } else {
                matriz[fila][columna] = 0;
            }
        } else {
            matriz[fila][columna] = 0;
        }
    } else if (matriz[fila][columna] == 1) {
        matriz[fila][columna] = 0;
    }
}

int obtener_mapa(int fila, int columna) {
    return matriz[fila][columna];
}

void actualizar_mapa_enemigos() {
    // Limpia todas las posiciones de enemigos en la matriz
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 20; j++) {
            if (matriz[i][j] == 9 || matriz[i][j] == 10 || matriz[i][j] == 11 || matriz[i][j] == 12) {
                matriz[i][j] = 0;
            }
        }
    }
    // Coloca solo el primer enemigo activo en cada casilla
    for (int i = 0; i < MAX_ENEMIGOS_CONST; i++) {
        if (enemigos[i].activo) {
            int f = enemigos[i].fila;
            int c = enemigos[i].columna;
            // Solo coloca si la casilla no tiene ya un enemigo
            if (!(matriz[f][c] == 9 || matriz[f][c] == 10 || matriz[f][c] == 11 || matriz[f][c] == 12)) {
                int valor_mapa = 9;
                if (enemigos[i].tipo == FOCA) valor_mapa = 10;
                else if (enemigos[i].tipo == AVE) valor_mapa = 11;
                else if (enemigos[i].tipo == HIELO) valor_mapa = 12;
                matriz[f][c] = valor_mapa;
            }
            // Si ya hay un enemigo, no se sobreescribe, así solo se muestra uno
        }
    }
}

void imprimir_mapa(Jugador* jugadores, int num_jugadores) {
    actualizar_mapa_enemigos();
    // Coloca a los jugadores en el mapa después de los enemigos
    for (int j = 0; j < num_jugadores; j++) {
        colocar_en_mapa(jugadores[j].fila, jugadores[j].columna, 1);
    }
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 20; j++) {
            if (matriz[i][j] == 1) {
                printf(VERDE "J " RESET);
            } else if (matriz[i][j] == 2) {
                printf(AZUL "# " RESET);
            } else if (matriz[i][j] == 3) {
                printf(MAGENTA "B " RESET);
            } else if (matriz[i][j] == 9) {
                printf(ROJO "Y " RESET);
            } else if (matriz[i][j] == 10) {
                printf(BLANCO "F " RESET);
            } else if (matriz[i][j] == 11) {
                printf(AMARILLO "A " RESET); // Ave en amarillo
            } else if (matriz[i][j] == 15) {
                printf(AMARILLO "B " RESET); // Banano en amarillo
            } else if (matriz[i][j] == 16) {
                printf("N "); // Naranja
            } else if (matriz[i][j] == 13) {
                printf(MAGENTA "B " RESET); // Berenjena en magenta
            } else if (matriz[i][j] == 14) {
                printf("L "); // Lechuga en verde
            } else if (matriz[i][j] == 12) {
                printf(CYAN "H " RESET);
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}
