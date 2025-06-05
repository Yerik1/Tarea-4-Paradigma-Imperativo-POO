#include <stdio.h>
#include "jugador.h"
#include "mapa.h"
#include "puntaje.h"
#include "vidas.h"

void inicializar_jugador(Jugador* jugador, int fila, int columna) {
    jugador->fila = fila;
    jugador->columna = columna;
    colocar_en_mapa(fila, columna, 1);
}

void mover_jugador(Jugador* jugador, int movimiento, int idx) {
    int nuevaFila = jugador->fila;
    int nuevaColumna = jugador->columna;
    static int en_aire[2] = {0, 0}; // 0 = suelo, 1 = en el aire, uno por jugador

    // --- Lógica de salto vertical ---
    if (movimiento == 3 && en_aire[idx] == 0) { // salto vertical
        if (nuevaFila >= 2) {
            int bloque_arriba = obtener_mapa(nuevaFila - 1, nuevaColumna);
            if (bloque_arriba == 3) {
                colocar_en_mapa(nuevaFila, nuevaColumna, 1); //mantener al jugador en su posición
            } else if (bloque_arriba == 2) {
                // Bloque normal, se puede romper
                printf("el personaje rompio un bloque\n");
                colocar_en_mapa(nuevaFila - 1, nuevaColumna, 0);
            } else if (bloque_arriba == 0) {
                nuevaFila -= 2;
                en_aire[idx] = 1;
            }
        }
    }
    // --- Lógica de movimiento en el aire ---
    else if (en_aire[idx] == 1) {
        if (movimiento == 1 && nuevaColumna < 19) { // derecha
            nuevaColumna++;
            // Dejar caer hasta tocar bloque 2 o 3
            while (nuevaFila < 27 && obtener_mapa(nuevaFila + 1, nuevaColumna) != 2 && obtener_mapa(nuevaFila + 1, nuevaColumna) != 3) {
                nuevaFila++;
            }
            // Si cae sobre bloque 3, se detiene una posición antes
            if (nuevaFila < 27 && obtener_mapa(nuevaFila + 1, nuevaColumna) == 3) {
                // Ya está una posición antes
            }
            en_aire[idx] = 0;
        } else if (movimiento == 2 && nuevaColumna > 0) { // izquierda
            nuevaColumna--;
            // Dejar caer hasta tocar bloque 2 o 3
            while (nuevaFila < 27 && obtener_mapa(nuevaFila + 1, nuevaColumna) != 2 && obtener_mapa(nuevaFila + 1, nuevaColumna) != 3) {
                nuevaFila++;
            }
            if (nuevaFila < 27 && obtener_mapa(nuevaFila + 1, nuevaColumna) == 3) {
                // Ya está una posición antes
            }
            en_aire[idx] = 0;
        } else { // cualquier otro movimiento
            // Dejar caer hasta tocar bloque 2 o 3
            while (nuevaFila < 27 && obtener_mapa(nuevaFila + 1, nuevaColumna) != 2 && obtener_mapa(nuevaFila + 1, nuevaColumna) != 3) {
                nuevaFila++;
            }
            if (nuevaFila < 27 && obtener_mapa(nuevaFila + 1, nuevaColumna) == 3) {
                // Ya está una posición antes
            }
            en_aire[idx] = 0;
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
            // Salto diagonal derecha (case 4)
            case 4:
                if (nuevaFila >= 2 && nuevaColumna <= 17 &&
                    obtener_mapa(nuevaFila - 1, nuevaColumna + 1) != 2 &&
                    obtener_mapa(nuevaFila - 2, nuevaColumna + 2) != 2) {
                    nuevaFila -= 2;
                    nuevaColumna += 2;
                }
                break;
            // Salto diagonal izquierda (case 5)
            case 5:
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
    // --- Scoring logic: check if player lands on a fruit ---
    int valor_casilla = obtener_mapa(jugador->fila, jugador->columna);
    if (valor_casilla == 13 || valor_casilla == 14 || valor_casilla == 15 || valor_casilla == 16) {
        actualizar_puntaje_por_fruta(jugador->fila, jugador->columna);
        printf("¡Fruta recogida! Puntaje actual: %d\n", obtener_puntaje());
        eliminar_del_mapa(jugador->fila, jugador->columna); // Remove fruit after collection
    }
    colocar_en_mapa(jugador->fila, jugador->columna, 1);
    // --- Extra: if player reaches column 9, add a life to both players ---
    if (jugador->columna == 9) {
        sumar_vida_a_ambos_jugadores();
        printf("¡Ambos jugadores ganan una vida por llegar a la columna 9!\n");
    }
    // --- Win condition: if player reaches column 0, reset to row 27, column 1 ---
    if (jugador->columna == 0) {
        printf("¡Felicidades! Has ganado el juego. El jugador vuelve a empezar desde el inicio.\n");
        eliminar_del_mapa(jugador->fila, jugador->columna);
        jugador->fila = 27;
        jugador->columna = 1 + idx; // posición predefinida diferente para cada jugador
        colocar_en_mapa(jugador->fila, jugador->columna, 1);
    }
}
