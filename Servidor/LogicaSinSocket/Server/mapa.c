#include <stdio.h>
#include "mapa.h"

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
            if(i % 2 == 0 && i <= 9 && i != 0) {
                if ((i == 2 || i == 6) && (j > 7 && j < 12)) matriz[i][j] = 3;
                if ((j > 2 && j < 7) || (j > 12 && j < 17)) matriz[i][j] = 3;
            }
        }
    }
}

void colocar_en_mapa(int fila, int columna, int valor) {
    matriz[fila][columna] = valor;
}

void eliminar_del_mapa(int fila, int columna) {
    if (matriz[fila][columna] == 1) {
        matriz[fila][columna] = 0;
    }
}

int obtener_mapa(int fila, int columna) {
    return matriz[fila][columna];
}

void imprimir_mapa_actual() {
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 20; j++) {
            if (matriz[i][j] == 1) {
                printf(VERDE "J " RESET); // Jugador en verde
            } else if (matriz[i][j] == 2) {
                printf(AZUL "# " RESET); // Bloque en azul
            } else if (matriz[i][j] == 3) {
                printf(MAGENTA "B " RESET); // Otro tipo de bloque en magenta
            } else if (matriz[i][j] == 9) {
                printf(ROJO "E " RESET); // Enemigo en rojo
            } else {
                printf(". "); // Espacio vacío
            }
        }
        printf("\n");
    }
}
