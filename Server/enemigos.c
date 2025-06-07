#include <stdio.h>
#include "enemigos.h"
#include "mapa.h"  // para colocar y borrar en el mapa
#include "jugador.h" // para acceder al jugador y sus vidas
#include "vidas.h"

Enemigo enemigos[MAX_ENEMIGOS];
const int MAX_ENEMIGOS_CONST = MAX_ENEMIGOS;

void inicializar_enemigos() {
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        enemigos[i].activo = 0;
    }
}



void crear_enemigo(TipoEnemigo tipo, int fila, int columna, Direccion direccion) {
    int col = columna;
    if (tipo == AVE) {
        // Solo puede salir de la izquierda (columna 0), filas >= 11 y con 0
        if (fila < 11 || fila > 27) {
            printf("No se puede crear AVE en fila %d: nivel prohibido\n", fila);
            return;
        }
        col = 0;
        if (obtener_mapa(fila, col) != 0) {
            printf("No se puede crear AVE en fila %d, columna %d: no es espacio vacío\n", fila, col);
            return;
        }
    } else if (tipo == FOCA || tipo == YETI) {
        // Solo puede salir de los lados (columna 0 o 19), filas >= 10 y con 0
        if (fila < 10 || fila > 27) {
            printf("No se puede crear FOCA/YETI en fila %d: nivel prohibido\n", fila);
            return;
        }
        if (col != 0 && col != 19) {
            printf("FOCA/YETI solo pueden salir de columna 0 o 19\n");
            return;
        }
        if (obtener_mapa(fila, col) != 0) {
            printf("No se puede crear FOCA/YETI en fila %d, columna %d: no es espacio vacío\n", fila, col);
            return;
        }
        // Nueva restricción: la base (bloque debajo) debe ser un bloque # (2)
        if (fila + 1 < 28 && obtener_mapa(fila + 1, col) != 2) {
            printf("No se puede crear FOCA/YETI en fila %d, columna %d: la base no es un bloque #\n", fila, col);
            return;
        }
    } else if (tipo == HIELO) {
        // Hielo puede salir de cualquier fila/columna válida fuera del bonus
        if (fila < 10 || fila > 27) {
            printf("No se puede crear HIELO en fila %d: nivel prohibido\n", fila);
            return;
        }
        if (obtener_mapa(fila, col) != 0) {
            printf("No se puede crear HIELO en fila %d, columna %d: no es espacio vacío\n", fila, col);
            return;
        }
    }
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        if (enemigos[i].activo == 0) {
            enemigos[i].tipo = tipo;
            enemigos[i].fila = fila;
            enemigos[i].columna = col;
            enemigos[i].direccion = direccion;
            enemigos[i].activo = 1;
            int valor_mapa = 9; // Yeti por defecto
            if (tipo == FOCA) valor_mapa = 10;
            else if (tipo == AVE) valor_mapa = 11;
            else if (tipo == HIELO) valor_mapa = 12;
            colocar_en_mapa(fila, col, valor_mapa);
            printf("Enemigo creado: tipo=%d en fila=%d col=%d\n", tipo, fila, col);
            break;
        }
    }
}

void mover_enemigos(Jugador* jugadores, int num_jugadores, int* proximos_movimientos) {
    static int ave_zigzag_fase[MAX_ENEMIGOS] = {0}; // 0: sube1, 1: sube2, 2: baja1, 3: baja2
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        if (enemigos[i].activo) {
            int nueva_fila = enemigos[i].fila;
            int nueva_col = enemigos[i].columna;
            TipoEnemigo tipo = enemigos[i].tipo;

            if (tipo == YETI || tipo == FOCA) {
                int direccion = (enemigos[i].direccion == DERECHA) ? 1 : -1;
                int siguiente_col = nueva_col + direccion;
                // Verificar si se sale del mapa
                if (siguiente_col < 0 || siguiente_col > 19) {
                    enemigos[i].activo = 0;
                    eliminar_del_mapa(enemigos[i].fila, enemigos[i].columna);
                    continue;
                }
                // Verificar que la base (bloque debajo) sea un bloque # (2)
                if (nueva_fila + 1 < 28 && obtener_mapa(nueva_fila + 1, siguiente_col) != 2) {
                    // Cambiar dirección y calcular nueva columna
                    enemigos[i].direccion = (enemigos[i].direccion == DERECHA) ? IZQUIERDA : DERECHA;
                    direccion = (enemigos[i].direccion == DERECHA) ? 1 : -1;
                    siguiente_col = nueva_col + direccion;
                    // Si al devolverse se sale del mapa, desaparece
                    if (siguiente_col < 0 || siguiente_col > 19) {
                        enemigos[i].activo = 0;
                        eliminar_del_mapa(enemigos[i].fila, enemigos[i].columna);
                        continue;
                    }
                    // Si la base sigue sin ser bloque #, desaparece
                    if (nueva_fila + 1 < 28 && obtener_mapa(nueva_fila + 1, siguiente_col) != 2) {
                        enemigos[i].activo = 0;
                        eliminar_del_mapa(enemigos[i].fila, enemigos[i].columna);
                        continue;
                    }
                }
                nueva_col = siguiente_col;
            } else if (tipo == AVE) {
                // Zigzag: sube 2 veces (1 fila y 1 col), baja 2 veces (1 fila y 1 col), repite
                int fase = ave_zigzag_fase[i];
                if (fase == 0 || fase == 1) { // sube
                    nueva_fila--;
                    nueva_col++;
                } else { // baja
                    nueva_fila++;
                    nueva_col++;
                }
                ave_zigzag_fase[i] = (fase + 1) % 4;
                // Limitar a los bordes
                if (nueva_fila < 10) nueva_fila = 10;
                if (nueva_fila > 27) nueva_fila = 27;
                // Si llega a la columna 19 o más, desaparece
                if (nueva_col >= 19) {
                    enemigos[i].activo = 0;
                    eliminar_del_mapa(enemigos[i].fila, enemigos[i].columna);
                    continue;
                }
            } else if (tipo == HIELO) {
                nueva_fila++;
            }

            // Eliminar anterior
            eliminar_del_mapa(enemigos[i].fila, enemigos[i].columna);

            // Validar límites
            if (nueva_fila >= 0 && nueva_fila < 28 &&
                nueva_col >= 0 && nueva_col < 20) {
                enemigos[i].fila = nueva_fila;
                enemigos[i].columna = nueva_col;
                int valor_mapa = 9;
                if (tipo == FOCA) valor_mapa = 10;
                else if (tipo == AVE) valor_mapa = 11;
                else if (tipo == HIELO) valor_mapa = 12;
                colocar_en_mapa(nueva_fila, nueva_col, valor_mapa);
            } else {
                enemigos[i].activo = 0; // sale del mapa
            }
        }
    }
    // Llamar automáticamente a la verificación de colisiones después de mover enemigos
    // NOTA: proximos_movimientos debe ser pasado como argumento
    verificar_colision_jugadores_enemigos(jugadores, num_jugadores, proximos_movimientos);
}

void imprimir_enemigos() {
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        if (enemigos[i].activo) {
            printf("Enemigo %d: tipo=%d pos=(%d,%d)\n", i, enemigos[i].tipo, enemigos[i].fila, enemigos[i].columna);
        }
    }
}

// Funciones helper para crear enemigos de forma simple
void crear_yeti(int fila, Direccion direccion) {
    int columna = (direccion == DERECHA) ? 0 : 19;
    crear_enemigo(YETI, fila, columna, direccion);
}

void crear_foca(int fila, Direccion direccion) {
    int columna = (direccion == DERECHA) ? 0 : 19;
    crear_enemigo(FOCA, fila, columna, direccion);
}

void crear_ave(int fila) {
    crear_enemigo(AVE, fila, 0, ARRIBA);
}

void crear_hielo(int fila, int columna) {
    crear_enemigo(HIELO, fila, columna, ABAJO);
}

void verificar_colision_jugadores_enemigos(Jugador* jugadores, int num_jugadores, int* proximos_movimientos) {
    for (int j = 0; j < num_jugadores; j++) {
        int colision = 0;
        for (int i = 0; i < MAX_ENEMIGOS_CONST; i++) {
            if (enemigos[i].activo &&
                enemigos[i].fila == jugadores[j].fila &&
                enemigos[i].columna == jugadores[j].columna) {
                if (proximos_movimientos[j] != 6) {
                    printf("vidas restantes del jugador %d: %d\n", j+1, obtener_vidas(j));
                    perder_vida(&jugadores[j], j);
                    colision = 1;
                }
                break;
            }
        }
        if (colision) {
            colocar_en_mapa(jugadores[j].fila, jugadores[j].columna, 1);
        }
    }
}
