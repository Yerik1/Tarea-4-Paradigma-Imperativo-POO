#include <stdio.h>
#include "jugador.h"
#include "mapa.h"

void inicializar_jugador(Jugador* jugador, int fila, int columna) {
    jugador->fila = fila;
    jugador->columna = columna;
    colocar_en_mapa(fila, columna, 1);
}

void mover_jugador(Jugador* jugador, int movimiento) {
    int nuevaFila = jugador->fila;
    int nuevaColumna = jugador->columna;

    switch(movimiento) {
        case 1: // derecha
            printf("Movimiento: Derecha\n");
            if (nuevaColumna < 19) nuevaColumna++;
            break;
        case 2: // izquierda
            printf("Movimiento: Izquierda\n");
            if (nuevaColumna > 0) nuevaColumna--;
            break;
        case 3: // salto vertical
            printf("Movimiento: Salto\n");
            if (nuevaFila >= 2) {
                if (obtener_mapa(nuevaFila - 1, nuevaColumna) == 2) {
                    printf("el personaje rompio un bloque\n");
                    colocar_en_mapa(nuevaFila - 1, nuevaColumna, 0);
                } else {
                    nuevaFila -= 2;
                    while (nuevaFila < 27 && obtener_mapa(nuevaFila + 1, nuevaColumna) != 2) {
                        nuevaFila++;
                    }
                }
            }
            break;
        case 4: // salto diagonal derecha
            printf("Movimiento: Brinco diagonal derecha\n");
            if (nuevaFila >= 2 && nuevaColumna <= 17 &&
                obtener_mapa(nuevaFila - 1, nuevaColumna + 1) != 2 &&
                obtener_mapa(nuevaFila - 2, nuevaColumna + 2) != 2) {
                nuevaFila -= 2;
                nuevaColumna += 2;
                while (nuevaFila < 27 && obtener_mapa(nuevaFila + 1, nuevaColumna) != 2) {
                    nuevaFila++;
                }
            } else {
                printf("No se puede hacer brinco diagonal derecha\n");
            }
            break;
        case 5: // salto diagonal izquierda
            printf("Movimiento: Brinco diagonal izquierda\n");
            if (nuevaFila >= 2 && nuevaColumna >= 2 &&
                obtener_mapa(nuevaFila - 1, nuevaColumna - 1) != 2 &&
                obtener_mapa(nuevaFila - 2, nuevaColumna - 2) != 2) {
                nuevaFila -= 2;
                nuevaColumna -= 2;
                while (nuevaFila < 27 && obtener_mapa(nuevaFila + 1, nuevaColumna) != 2) {
                    nuevaFila++;
                }
            } else {
                printf("No se puede hacer brinco diagonal izquierda\n");
            }
            break;
        case 6:
            printf("Movimiento: Atacar\n");
            break;
        default:
            printf("Movimiento no reconocido\n");
    }

    eliminar_del_mapa(jugador->fila, jugador->columna);
    jugador->fila = nuevaFila;
    jugador->columna = nuevaColumna;
    colocar_en_mapa(jugador->fila, jugador->columna, 1);
}
