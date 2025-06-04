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
    static int en_aire = 0; // 0 = suelo, 1 = en el aire

    // --- Lógica de salto vertical ---
    if (movimiento == 3 && en_aire == 0) { // salto vertical
        if (nuevaFila >= 2 && obtener_mapa(nuevaFila - 1, nuevaColumna) == 0) {
            nuevaFila -= 2;
            en_aire = 1;
        } else if (nuevaFila >= 2 && obtener_mapa(nuevaFila - 1, nuevaColumna) == 2) {
            printf("el personaje rompio un bloque\n");
            colocar_en_mapa(nuevaFila - 1, nuevaColumna, 0);
        }
    }
    // --- Lógica de movimiento en el aire ---
    else if (en_aire == 1) {
        if (movimiento == 1 && nuevaColumna < 19) { // derecha
            nuevaColumna++;
            // Dejar caer hasta tocar bloque 2
            while (nuevaFila < 27 && obtener_mapa(nuevaFila + 1, nuevaColumna) != 2) {
                nuevaFila++;
            }
            en_aire = 0;
        } else if (movimiento == 2 && nuevaColumna > 0) { // izquierda
            nuevaColumna--;
            // Dejar caer hasta tocar bloque 2
            while (nuevaFila < 27 && obtener_mapa(nuevaFila + 1, nuevaColumna) != 2) {
                nuevaFila++;
            }
            en_aire = 0;
        } else { // cualquier otro movimiento
            // Dejar caer hasta tocar bloque 2
            while (nuevaFila < 27 && obtener_mapa(nuevaFila + 1, nuevaColumna) != 2) {
                nuevaFila++;
            }
            en_aire = 0;
        }
    }
    // --- Lógica normal si no está en el aire ---
    else {
        switch(movimiento) {
            case 1: // derecha
                if (nuevaColumna < 19) nuevaColumna++;
                break;
            case 2: // izquierda
                if (nuevaColumna > 0) nuevaColumna--;
                break;
            case 4: // salto diagonal derecha
                if (nuevaFila >= 2 && nuevaColumna <= 17 &&
                    obtener_mapa(nuevaFila - 1, nuevaColumna + 1) != 2 &&
                    obtener_mapa(nuevaFila - 2, nuevaColumna + 2) != 2) {
                    nuevaFila -= 2;
                    nuevaColumna += 2;
                }
                break;
            case 5: // salto diagonal izquierda
                if (nuevaFila >= 2 && nuevaColumna >= 2 &&
                    obtener_mapa(nuevaFila - 1, nuevaColumna - 1) != 2 &&
                    obtener_mapa(nuevaFila - 2, nuevaColumna - 2) != 2) {
                    nuevaFila -= 2;
                    nuevaColumna -= 2;
                }
                break;
            case 6:
                // Atacar (no implementado)
                break;
            default:
                break;
        }
    }

    eliminar_del_mapa(jugador->fila, jugador->columna);
    jugador->fila = nuevaFila;
    jugador->columna = nuevaColumna;
    colocar_en_mapa(jugador->fila, jugador->columna, 1);
}
