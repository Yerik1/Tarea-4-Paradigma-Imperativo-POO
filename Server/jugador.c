#include <stdio.h>
#include "jugador.h"
#include "mapa.h"
#include "puntaje.h"
#include "vidas.h"
#include "enemigos.h"
#include "frutas.h" // Para actualizar_puntaje_por_fruta
#include "vidas.h"  // Para penalizar_jugador_rezagado_avanzado

// Prototipos para evitar warnings
void actualizar_puntaje_por_fruta(int fila, int columna);
void penalizar_jugador_rezagado_avanzado(Jugador* jugadores, int num_jugadores);

void inicializar_jugador(Jugador* jugador, int fila, int columna) {
    jugador->fila = fila;
    jugador->columna = columna;
    colocar_en_mapa(fila, columna, 1);
}

void mover_jugador(Jugador* jugadores, int idx, int movimiento, int num_jugadores) {
    Jugador* jugador = &jugadores[idx];
    int nuevaFila = jugador->fila;
    int nuevaColumna = jugador->columna;
    static int en_aire[2] = {0, 0}; // 0 = suelo, 1 = en el aire, uno por jugador

    // --- Lógica de salto vertical ---
    if (movimiento == 3 && en_aire[idx] == 0) { // salto vertical
        // Si el jugador está en la fila 1, solo puede saltar 1 espacio hacia arriba
        if (nuevaFila == 1) {
            int bloque_arriba = obtener_mapa(nuevaFila - 1, nuevaColumna);
            if (bloque_arriba == 3) {
                colocar_en_mapa(nuevaFila, nuevaColumna, 1); //mantener al jugador en su posición
            } else if (bloque_arriba == 2) {
                // Bloque normal, se puede romper
                printf("el personaje rompio un bloque\n");
                colocar_en_mapa(nuevaFila - 1, nuevaColumna, 0);
            } else if (bloque_arriba == 0) {
                nuevaFila -= 1;
                en_aire[idx] = 1;
            }
        } else if (nuevaFila >= 2) {
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
            // Dejar caer hasta tocar bloque 2 o 3, detenerse una posición antes
            while (nuevaFila < 27) {
                int abajo = obtener_mapa(nuevaFila + 1, nuevaColumna);
                if (abajo == 2 || abajo == 3) break;
                nuevaFila++;
            }
            en_aire[idx] = 0;
        } else if (movimiento == 2 && nuevaColumna > 0) { // izquierda
            nuevaColumna--;
            // Dejar caer hasta tocar bloque 2 o 3, detenerse una posición antes
            while (nuevaFila < 27) {
                int abajo = obtener_mapa(nuevaFila + 1, nuevaColumna);
                if (abajo == 2 || abajo == 3) break;
                nuevaFila++;
            }
            en_aire[idx] = 0;
        } else { // cualquier otro movimiento
            // Dejar caer hasta tocar bloque 2 o 3, detenerse una posición antes
            while (nuevaFila < 27) {
                int abajo = obtener_mapa(nuevaFila + 1, nuevaColumna);
                if (abajo == 2 || abajo == 3) break;
                nuevaFila++;
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
                // Atacar: si hay un enemigo en la misma casilla, eliminarlo y sumar puntos
                for (int i = 0; i < MAX_ENEMIGOS_CONST; i++) {
                    if (enemigos[i].activo &&
                        enemigos[i].fila == jugador->fila &&
                        enemigos[i].columna == jugador->columna) {
                        int puntos = 0;
                        if (enemigos[i].tipo == AVE) {
                            puntos = 800;
                        } else if (enemigos[i].tipo == FOCA || enemigos[i].tipo == YETI) {
                            puntos = 400;
                        }
                        if (puntos > 0) {
                            printf("¡Enemigo derrotado! +%d puntos\n", puntos);
                            sumar_puntaje(puntos);
                        }
                        enemigos[i].activo = 0;
                        eliminar_del_mapa(jugador->fila, jugador->columna);
                        colocar_en_mapa(jugador->fila, jugador->columna, 1); // Recoloca al jugador
                        break; // Solo un enemigo por ataque
                    }
                }
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

    // --- Extra: si el jugador llega a la fila 9, gana una vida solo una vez ---
    static int vida_ganada_en_fila9[2] = {0, 0};
    if (jugador->fila == 9 && !vida_ganada_en_fila9[idx]) {
        vida_ganada_en_fila9[idx] = 1;
        extern void sumar_vida_jugador(int idx);
        sumar_vida_jugador(idx);
        printf("¡El jugador %d gana una vida por llegar a la fila 9!\n", idx+1);
    }

    // --- Mensaje de victoria si el jugador llega a la fila 0 ---
    if (jugador->fila == 0) {
        printf("¡Felicidades Jugador %d, has llegado a la cima!\n", idx + 1);
        printf("Puntaje total: %d\n", obtener_puntaje());
        printf("Vidas Jugador 1: %d\n", obtener_vidas(0));
        printf("Vidas Jugador 2: %d\n", obtener_vidas(1));
    }

    // --- Win condition: if player reaches column 0, reset to row 27, column 1 ---
    if (jugador->columna == 0) {
        printf("¡Felicidades! Has ganado el juego. El jugador vuelve a empezar desde el inicio.\n");
        eliminar_del_mapa(jugador->fila, jugador->columna);
        jugador->fila = 27;
        jugador->columna = 1 + idx;
        colocar_en_mapa(jugador->fila, jugador->columna, 1);
    }

    // Llama a la penalización después de cada movimiento
    penalizar_jugador_rezagado_avanzado(jugadores, num_jugadores);
}
