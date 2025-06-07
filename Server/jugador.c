#include <stdio.h>
#include "jugador.h"
#include "mapa.h"
#include "puntaje.h"
#include "vidas.h"
#include "enemigos.h"
#include "frutas.h"
#include "game_state.h"

// Prototipos para evitar warnings
void actualizar_puntaje_por_fruta(GameState* game, int fila, int columna);
void penalizar_jugador_rezagado_avanzado(GameState* game);

void inicializar_jugador(GameState* game, int idx, int fila, int columna) {
    game->jugadores[idx].fila = fila;
    game->jugadores[idx].columna = columna;
    game->num_jugadores = 2; // O el valor que corresponda
    game->matriz[fila][columna] = 1;
}

void mover_jugador(GameState* game, int idx, int movimiento) {
    Jugador* jugador = &game->jugadores[idx];
    int nuevaFila = jugador->fila;
    int nuevaColumna = jugador->columna;
    static int en_aire[2] = {0, 0}; // 0 = suelo, 1 = en el aire, uno por jugador

    // --- Lógica de salto vertical ---
    if (movimiento == 3 && en_aire[idx] == 0) { // salto vertical
        // Si el jugador está en la fila 1, solo puede saltar 1 espacio hacia arriba
        if (nuevaFila == 1) {
            int bloque_arriba = game->matriz[nuevaFila - 1][nuevaColumna];
            if (bloque_arriba == 3) {
                game->matriz[nuevaFila][nuevaColumna] = 1; //mantener al jugador en su posición
            } else if (bloque_arriba == 2) {
                // Bloque normal, se puede romper
                printf("el personaje rompio un bloque\n");
                game->matriz[nuevaFila - 1][nuevaColumna] = 0;
            } else if (bloque_arriba == 0) {
                // Revisar si hay enemigo o hielo en la casilla intermedia
                for (int i = 0; i < MAX_ENEMIGOS; i++) {
                    if (game->enemigos[i].activo && game->enemigos[i].fila == nuevaFila - 1 && game->enemigos[i].columna == nuevaColumna) {
                        int puntos = 0;
                        if (game->enemigos[i].tipo == AVE) {
                            puntos = 800;
                            printf("¡Enemigo derrotado durante el salto! +800 puntos\n");
                        } else if (game->enemigos[i].tipo == FOCA || game->enemigos[i].tipo == YETI) {
                            puntos = 400;
                            printf("¡Enemigo derrotado durante el salto! +400 puntos\n");
                        } else if (game->enemigos[i].tipo == HIELO) {
                            puntos = 10;
                            printf("¡Bloque de hielo destruido durante el salto! +10 puntos\n");
                        }
                        if (puntos > 0) {
                            sumar_puntaje(game, puntos);
                        }
                        game->enemigos[i].activo = 0;
                        game->matriz[nuevaFila - 1][nuevaColumna] = 0;
                        game->matriz[nuevaFila - 1][nuevaColumna] = 1;
                        break;
                    }
                }
                nuevaFila -= 1;
                en_aire[idx] = 1;
            }
        } else if (nuevaFila >= 2) {
            // Revisar si hay enemigo o hielo en la casilla intermedia (fila-1)
            for (int i = 0; i < MAX_ENEMIGOS; i++) {
                if (game->enemigos[i].activo && game->enemigos[i].fila == nuevaFila - 1 && game->enemigos[i].columna == nuevaColumna) {
                    int puntos = 0;
                    if (game->enemigos[i].tipo == AVE) {
                        puntos = 800;
                        printf("¡Enemigo derrotado durante el salto! +800 puntos\n");
                    } else if (game->enemigos[i].tipo == FOCA || game->enemigos[i].tipo == YETI) {
                        puntos = 400;
                        printf("¡Enemigo derrotado durante el salto! +400 puntos\n");
                    } else if (game->enemigos[i].tipo == HIELO) {
                        puntos = 10;
                        printf("¡Bloque de hielo destruido durante el salto! +10 puntos\n");
                    }
                    if (puntos > 0) {
                        sumar_puntaje(game, puntos);
                    }
                    game->enemigos[i].activo = 0;
                    game->matriz[nuevaFila - 1][nuevaColumna] = 0;
                    game->matriz[nuevaFila - 1][nuevaColumna] = 1;
                    break;
                }
            }
            // Revisar si hay enemigo o hielo en la casilla destino (fila-2)
            for (int i = 0; i < MAX_ENEMIGOS; i++) {
                if (game->enemigos[i].activo && game->enemigos[i].fila == nuevaFila - 2 && game->enemigos[i].columna == nuevaColumna) {
                    int puntos = 0;
                    if (game->enemigos[i].tipo == AVE) {
                        puntos = 800;
                        printf("¡Enemigo derrotado durante el salto! +800 puntos\n");
                    } else if (game->enemigos[i].tipo == FOCA || game->enemigos[i].tipo == YETI) {
                        puntos = 400;
                        printf("¡Enemigo derrotado durante el salto! +400 puntos\n");
                    } else if (game->enemigos[i].tipo == HIELO) {
                        puntos = 10;
                        printf("¡Bloque de hielo destruido durante el salto! +10 puntos\n");
                    }
                    if (puntos > 0) {
                        sumar_puntaje(game, puntos);
                    }
                    game->enemigos[i].activo = 0;
                    game->matriz[nuevaFila - 2][nuevaColumna] = 0;
                    game->matriz[nuevaFila - 2][nuevaColumna] = 1;
                    break;
                }
            }
            int bloque_arriba = game->matriz[nuevaFila - 1][nuevaColumna];
            if (bloque_arriba == 3) {
                game->matriz[nuevaFila][nuevaColumna] = 1; //mantener al jugador en su posición
            } else if (bloque_arriba == 2) {
                // Bloque normal, se puede romper
                printf("el personaje rompio un bloque\n");
                game->matriz[nuevaFila - 1][nuevaColumna] = 0;
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
                int abajo = game->matriz[nuevaFila + 1][nuevaColumna];
                if (abajo == 2 || abajo == 3) break;
                nuevaFila++;
            }
            en_aire[idx] = 0;
        } else if (movimiento == 2 && nuevaColumna > 0) { // izquierda
            nuevaColumna--;
            // Dejar caer hasta tocar bloque 2 o 3, detenerse una posición antes
            while (nuevaFila < 27) {
                int abajo = game->matriz[nuevaFila + 1][nuevaColumna];
                if (abajo == 2 || abajo == 3) break;
                nuevaFila++;
            }
            en_aire[idx] = 0;
        } else { // cualquier otro movimiento
            // Dejar caer hasta tocar bloque 2 o 3, detenerse una posición antes
            while (nuevaFila < 27) {
                int abajo = game->matriz[nuevaFila + 1][nuevaColumna];
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
                    game->matriz[nuevaFila - 1][nuevaColumna + 1] != 2 &&
                    game->matriz[nuevaFila - 2][nuevaColumna + 2] != 2) {
                    // Revisar casilla intermedia (fila-1, columna+1)
                    for (int i = 0; i < MAX_ENEMIGOS; i++) {
                        if (game->enemigos[i].activo && game->enemigos[i].fila == nuevaFila - 1 && game->enemigos[i].columna == nuevaColumna + 1) {
                            int puntos = 0;
                            if (game->enemigos[i].tipo == AVE) {
                                puntos = 800;
                                printf("¡Enemigo derrotado durante el salto! +800 puntos\n");
                            } else if (game->enemigos[i].tipo == FOCA || game->enemigos[i].tipo == YETI) {
                                puntos = 400;
                                printf("¡Enemigo derrotado durante el salto! +400 puntos\n");
                            } else if (game->enemigos[i].tipo == HIELO) {
                                puntos = 10;
                                printf("¡Bloque de hielo destruido durante el salto! +10 puntos\n");
                            }
                            if (puntos > 0) {
                                sumar_puntaje(game, puntos);
                            }
                            game->enemigos[i].activo = 0;
                            game->matriz[nuevaFila - 1][nuevaColumna + 1] = 0;
                            game->matriz[nuevaFila - 1][nuevaColumna + 1] = 1;
                            break;
                        }
                    }
                    // Revisar casilla destino (fila-2, columna+2)
                    for (int i = 0; i < MAX_ENEMIGOS; i++) {
                        if (game->enemigos[i].activo && game->enemigos[i].fila == nuevaFila - 2 && game->enemigos[i].columna == nuevaColumna + 2) {
                            int puntos = 0;
                            if (game->enemigos[i].tipo == AVE) {
                                puntos = 800;
                                printf("¡Enemigo derrotado durante el salto! +800 puntos\n");
                            } else if (game->enemigos[i].tipo == FOCA || game->enemigos[i].tipo == YETI) {
                                puntos = 400;
                                printf("¡Enemigo derrotado durante el salto! +400 puntos\n");
                            } else if (game->enemigos[i].tipo == HIELO) {
                                puntos = 10;
                                printf("¡Bloque de hielo destruido durante el salto! +10 puntos\n");
                            }
                            if (puntos > 0) {
                                sumar_puntaje(game, puntos);
                            }
                            game->enemigos[i].activo = 0;
                            game->matriz[nuevaFila - 2][nuevaColumna + 2] = 0;
                            game->matriz[nuevaFila - 2][nuevaColumna + 2] = 1;
                            break;
                        }
                    }
                    nuevaFila -= 2;
                    nuevaColumna += 2;
                }
                break;
            // Salto diagonal izquierda (case 5)
            case 5:
                if (nuevaFila >= 2 && nuevaColumna >= 2 &&
                    game->matriz[nuevaFila - 1][nuevaColumna - 1] != 2 &&
                    game->matriz[nuevaFila - 2][nuevaColumna - 2] != 2) {
                    // Revisar casilla intermedia (fila-1, columna-1)
                    for (int i = 0; i < MAX_ENEMIGOS; i++) {
                        if (game->enemigos[i].activo && game->enemigos[i].fila == nuevaFila - 1 && game->enemigos[i].columna == nuevaColumna - 1) {
                            int puntos = 0;
                            if (game->enemigos[i].tipo == AVE) {
                                puntos = 800;
                                printf("¡Enemigo derrotado durante el salto! +800 puntos\n");
                            } else if (game->enemigos[i].tipo == FOCA || game->enemigos[i].tipo == YETI) {
                                puntos = 400;
                                printf("¡Enemigo derrotado durante el salto! +400 puntos\n");
                            } else if (game->enemigos[i].tipo == HIELO) {
                                puntos = 10;
                                printf("¡Bloque de hielo destruido durante el salto! +10 puntos\n");
                            }
                            if (puntos > 0) {
                                sumar_puntaje(game, puntos);
                            }
                            game->enemigos[i].activo = 0;
                            game->matriz[nuevaFila - 1][nuevaColumna - 1] = 0;
                            game->matriz[nuevaFila - 1][nuevaColumna - 1] = 1;
                            break;
                        }
                    }
                    // Revisar casilla destino (fila-2, columna-2)
                    for (int i = 0; i < MAX_ENEMIGOS; i++) {
                        if (game->enemigos[i].activo && game->enemigos[i].fila == nuevaFila - 2 && game->enemigos[i].columna == nuevaColumna - 2) {
                            int puntos = 0;
                            if (game->enemigos[i].tipo == AVE) {
                                puntos = 800;
                                printf("¡Enemigo derrotado durante el salto! +800 puntos\n");
                            } else if (game->enemigos[i].tipo == FOCA || game->enemigos[i].tipo == YETI) {
                                puntos = 400;
                                printf("¡Enemigo derrotado durante el salto! +400 puntos\n");
                            } else if (game->enemigos[i].tipo == HIELO) {
                                puntos = 10;
                                printf("¡Bloque de hielo destruido durante el salto! +10 puntos\n");
                            }
                            if (puntos > 0) {
                                sumar_puntaje(game, puntos);
                            }
                            game->enemigos[i].activo = 0;
                            game->matriz[nuevaFila - 2][nuevaColumna - 2] = 0;
                            game->matriz[nuevaFila - 2][nuevaColumna - 2] = 1;
                            break;
                        }
                    }
                    nuevaFila -= 2;
                    nuevaColumna -= 2;
                }
                break;
            case 6:
                // Atacar: si hay un enemigo en la misma casilla, eliminarlo y sumar puntos
                for (int i = 0; i < MAX_ENEMIGOS; i++) {
                    if (game->enemigos[i].activo &&
                        game->enemigos[i].fila == jugador->fila &&
                        game->enemigos[i].columna == jugador->columna) {
                        int puntos = 0;
                        if (game->enemigos[i].tipo == AVE) {
                            puntos = 800;
                        } else if (game->enemigos[i].tipo == FOCA || game->enemigos[i].tipo == YETI) {
                            puntos = 400;
                        } else if (game->enemigos[i].tipo == HIELO) {
                            puntos = 10;
                            printf("¡Bloque de hielo destruido! +10 puntos\n");
                        }
                        if (puntos > 0 && game->enemigos[i].tipo != HIELO) {
                            printf("¡Enemigo derrotado! +%d puntos\n", puntos);
                        }
                        if (puntos > 0) {
                            sumar_puntaje(game, puntos);
                        }
                        game->enemigos[i].activo = 0;
                        game->matriz[jugador->fila][jugador->columna] = 1; // Recoloca al jugador
                        break; // Solo un enemigo por ataque
                    }
                }
                break;
            default:
                break;
        }
    }

    // Eliminar del mapa anterior SOLO si el otro jugador no está en la misma casilla
    int otro_idx = (idx == 0) ? 1 : 0;
    if (!(jugador->fila == game->jugadores[otro_idx].fila && jugador->columna == game->jugadores[otro_idx].columna)) {
        game->matriz[jugador->fila][jugador->columna] = 0;
    }
    jugador->fila = nuevaFila;
    jugador->columna = nuevaColumna;
    // --- Scoring logic: check if player lands on a fruit ---
    int valor_casilla = game->matriz[jugador->fila][jugador->columna];
    if (valor_casilla == 13 || valor_casilla == 14 || valor_casilla == 15 || valor_casilla == 16) {
        actualizar_puntaje_por_fruta(game, jugador->fila, jugador->columna); // Adaptar para usar game si es necesario
        printf("¡Fruta recogida! Puntaje actual: %d\n", game->puntaje);
        game->matriz[jugador->fila][jugador->columna] = 0;
    }
    game->matriz[jugador->fila][jugador->columna] = 1;

    // --- Extra: si el jugador llega a la fila 9, gana una vida solo una vez ---
    static int vida_ganada_en_fila9[2] = {0, 0};
    if (jugador->fila == 9 && !vida_ganada_en_fila9[idx]) {
        vida_ganada_en_fila9[idx] = 1;
        // sumar_vida_jugador(idx); // Adaptar para usar game si es necesario
        printf("¡El jugador %d gana una vida por llegar a la fila 9!\n", idx+1);
    }

    // --- Mensaje de victoria si el jugador llega a la fila 0 ---
    if (jugador->fila == 0) {
        printf("¡Felicidades Jugador %d, has llegado a la cima!\n", idx + 1);
        printf("Puntaje total: %d\n", game->puntaje);
        printf("Vidas Jugador 1: %d\n", game->vidas_jugador[0]);
        printf("Vidas Jugador 2: %d\n", game->vidas_jugador[1]);
        // Aumentar velocidad de enemigos solo en el juego actual
        game->velocidad_enemigos++;
        printf("¡La velocidad de los enemigos ha aumentado!\n");
        // Reiniciar jugador al inicio
        printf("¡El jugador vuelve a empezar desde el inicio!\n");
        game->matriz[jugador->fila][jugador->columna] = 0;
        jugador->fila = 27;
        jugador->columna = 1 + idx;
        game->matriz[jugador->fila][jugador->columna] = 1;
    }

    // Llama a la penalización después de cada movimiento
    penalizar_jugador_rezagado_avanzado(game);
}
