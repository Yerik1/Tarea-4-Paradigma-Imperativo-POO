#include <stdio.h>
#include "enemigos.h"
#include "mapa.h"
#include "jugador.h"
#include "vidas.h"
#include "game_state.h"

void inicializar_enemigos(GameState* game) {
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        game->enemigos[i].activo = 0;
    }
}

void crear_enemigo(GameState* game, TipoEnemigo tipo, int fila, int columna, Direccion direccion) {
    int col = columna;
    if (tipo == AVE) {
        if (fila < 11 || fila > 27) {
            printf("No se puede crear AVE en fila %d: nivel prohibido\n", fila);
            return;
        }
        col = 0;
        if (obtener_mapa(game, fila, col) != 0) {
            printf("No se puede crear AVE en fila %d, columna %d: no es espacio vacío\n", fila, col);
            return;
        }
    } else if (tipo == FOCA || tipo == YETI) {
        if (fila < 10 || fila > 27) {
            printf("No se puede crear FOCA/YETI en fila %d: nivel prohibido\n", fila);
            return;
        }
        if (col != 0 && col != 19) {
            printf("FOCA/YETI solo pueden salir de columna 0 o 19\n");
            return;
        }
        if (obtener_mapa(game, fila, col) != 0) {
            printf("No se puede crear FOCA/YETI en fila %d, columna %d: no es espacio vacío\n", fila, col);
            return;
        }
        if (fila + 1 < 28 && obtener_mapa(game, fila + 1, col) != 2) {
            printf("No se puede crear FOCA/YETI en fila %d, columna %d: la base no es un bloque #\n", fila, col);
            return;
        }
    } else if (tipo == HIELO) {
        if (fila < 10 || fila > 27) {
            printf("No se puede crear HIELO en fila %d: nivel prohibido\n", fila);
            return;
        }
        if (obtener_mapa(game, fila, col) != 0) {
            printf("No se puede crear HIELO en fila %d, columna %d: no es espacio vacío\n", fila, col);
            return;
        }
    }
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        if (game->enemigos[i].activo == 0) {
            game->enemigos[i].tipo = tipo;
            game->enemigos[i].fila = fila;
            game->enemigos[i].columna = col;
            game->enemigos[i].direccion = direccion;
            game->enemigos[i].activo = 1;
            int valor_mapa = 9;
            if (tipo == FOCA) valor_mapa = 10;
            else if (tipo == AVE) valor_mapa = 11;
            else if (tipo == HIELO) valor_mapa = 12;
            colocar_en_mapa(game, fila, col, valor_mapa);
            printf("Enemigo creado: tipo=%d en fila=%d col=%d\n", tipo, fila, col);
            break;
        }
    }
}

void mover_enemigos(GameState* game, int* proximos_movimientos) {
    static int ave_zigzag_fase[MAX_ENEMIGOS] = {0};
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        if (game->enemigos[i].activo) {
            int nueva_fila = game->enemigos[i].fila;
            int nueva_col = game->enemigos[i].columna;
            TipoEnemigo tipo = game->enemigos[i].tipo;
            if (tipo == YETI || tipo == FOCA) {
                int direccion = (game->enemigos[i].direccion == DERECHA) ? 1 : -1;
                int siguiente_col = nueva_col + direccion;
                if (siguiente_col < 0 || siguiente_col > 19) {
                    game->enemigos[i].activo = 0;
                    eliminar_del_mapa(game, game->enemigos[i].fila, game->enemigos[i].columna);
                    continue;
                }
                if (nueva_fila + 1 < 28 && obtener_mapa(game, nueva_fila + 1, siguiente_col) != 2) {
                    game->enemigos[i].direccion = (game->enemigos[i].direccion == DERECHA) ? IZQUIERDA : DERECHA;
                    direccion = (game->enemigos[i].direccion == DERECHA) ? 1 : -1;
                    siguiente_col = nueva_col + direccion;
                    if (siguiente_col < 0 || siguiente_col > 19) {
                        game->enemigos[i].activo = 0;
                        eliminar_del_mapa(game, game->enemigos[i].fila, game->enemigos[i].columna);
                        continue;
                    }
                    if (nueva_fila + 1 < 28 && obtener_mapa(game, nueva_fila + 1, siguiente_col) != 2) {
                        game->enemigos[i].activo = 0;
                        eliminar_del_mapa(game, game->enemigos[i].fila, game->enemigos[i].columna);
                        continue;
                    }
                }
                nueva_col = siguiente_col;
            } else if (tipo == AVE) {
                int fase = ave_zigzag_fase[i];
                if (fase == 0 || fase == 1) {
                    nueva_fila--;
                    nueva_col++;
                } else {
                    nueva_fila++;
                    nueva_col++;
                }
                ave_zigzag_fase[i] = (fase + 1) % 4;
                if (nueva_fila < 10) nueva_fila = 10;
                if (nueva_fila > 27) nueva_fila = 27;
                if (nueva_col >= 19) {
                    game->enemigos[i].activo = 0;
                    eliminar_del_mapa(game, game->enemigos[i].fila, game->enemigos[i].columna);
                    continue;
                }
            } else if (tipo == HIELO) {
                nueva_fila++;
            }
            eliminar_del_mapa(game, game->enemigos[i].fila, game->enemigos[i].columna);
            if (nueva_fila >= 0 && nueva_fila < 28 && nueva_col >= 0 && nueva_col < 20) {
                game->enemigos[i].fila = nueva_fila;
                game->enemigos[i].columna = nueva_col;
                int valor_mapa = 9;
                if (tipo == FOCA) valor_mapa = 10;
                else if (tipo == AVE) valor_mapa = 11;
                else if (tipo == HIELO) valor_mapa = 12;
                colocar_en_mapa(game, nueva_fila, nueva_col, valor_mapa);
            } else {
                game->enemigos[i].activo = 0;
            }
        }
    }
    verificar_colision_jugadores_enemigos(game, proximos_movimientos);
}

void imprimir_enemigos(GameState* game) {
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        if (game->enemigos[i].activo) {
            printf("Enemigo %d: tipo=%d pos=(%d,%d)\n", i, game->enemigos[i].tipo, game->enemigos[i].fila, game->enemigos[i].columna);
        }
    }
}

void crear_yeti(GameState* game, int fila, Direccion direccion) {
    int columna = (direccion == DERECHA) ? 0 : 19;
    crear_enemigo(game, YETI, fila, columna, direccion);
}

void crear_foca(GameState* game, int fila, Direccion direccion) {
    int columna = (direccion == DERECHA) ? 0 : 19;
    crear_enemigo(game, FOCA, fila, columna, direccion);
}

void crear_ave(GameState* game, int fila) {
    crear_enemigo(game, AVE, fila, 0, ARRIBA);
}

void crear_hielo(GameState* game, int fila, int columna) {
    crear_enemigo(game, HIELO, fila, columna, ABAJO);
}

void verificar_colision_jugadores_enemigos(GameState* game, int* proximos_movimientos) {
    for (int j = 0; j < game->num_jugadores; j++) {
        int colision = 0;
        for (int i = 0; i < MAX_ENEMIGOS; i++) {
            if (game->enemigos[i].activo &&
                game->enemigos[i].fila == game->jugadores[j].fila &&
                game->enemigos[i].columna == game->jugadores[j].columna) {
                if (proximos_movimientos[j] != 6) {
                    printf("vidas restantes del jugador %d: %d\n", j+1, game->vidas_jugador[j]);
                    perder_vida(game, j);
                    colision = 1;
                }
                break;
            }
        }
        if (colision) {
            colocar_en_mapa(game, game->jugadores[j].fila, game->jugadores[j].columna, 1);
        }
    }
}
