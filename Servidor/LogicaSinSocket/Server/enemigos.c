#include <stdio.h>
#include "enemigos.h"
#include "mapa.h"  // para colocar y borrar en el mapa

static Enemigo enemigos[MAX_ENEMIGOS];

void inicializar_enemigos() {
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        enemigos[i].activo = 0;
    }
}



void crear_enemigo(TipoEnemigo tipo, int fila, int columna, Direccion direccion) {
    // Restricciones para la creación de enemigos
    // No se pueden crear en la fila 27 (nivel 1), ni en filas 0-9 (nivel bonus),
    // ni en filas donde haya un 2 o 3 en el mapa
    if (fila == 27 || fila < 11) {
        printf("No se puede crear enemigo en fila %d: nivel prohibido\n", fila);
        return;
    }
    int valor_mapa = obtener_mapa(fila, columna);
    if (valor_mapa == 2 || valor_mapa == 3) {
        printf("No se puede crear enemigo en fila %d, columna %d: hay un bloque prohibido (2 o 3)\n", fila, columna);
        return;
    }
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        if (enemigos[i].activo == 0) {
            enemigos[i].tipo = tipo;
            enemigos[i].fila = fila;
            enemigos[i].columna = columna;
            enemigos[i].direccion = direccion;
            enemigos[i].activo = 1;
            colocar_en_mapa(fila, columna, 9); // 9 = representa enemigo en el mapa
            printf("Enemigo creado: tipo=%d en fila=%d col=%d\n", tipo, fila, columna);
            break;
        }
    }
}

void mover_enemigos() {
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        if (enemigos[i].activo) {
            int nueva_fila = enemigos[i].fila;
            int nueva_col = enemigos[i].columna;

            // Lógica básica por tipo
            if (enemigos[i].tipo == YETI || enemigos[i].tipo == FOCA) {
                nueva_col += (enemigos[i].direccion == DERECHA) ? 1 : -1;
            } else if (enemigos[i].tipo == AVE) {
                nueva_col++;
                nueva_fila += (enemigos[i].direccion == ARRIBA) ? -1 : 1;
            } else if (enemigos[i].tipo == HIELO) {
                nueva_fila++;
            }

            // Eliminar anterior
            eliminar_del_mapa(enemigos[i].fila, enemigos[i].columna);

            // Validar límites
            if (nueva_fila >= 0 && nueva_fila < 28 &&
                nueva_col >= 0 && nueva_col < 20) {
                enemigos[i].fila = nueva_fila;
                enemigos[i].columna = nueva_col;
                colocar_en_mapa(nueva_fila, nueva_col, 9); // 9 = enemigo
            } else {
                enemigos[i].activo = 0; // sale del mapa
            }
        }
    }
}

void imprimir_enemigos() {
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        if (enemigos[i].activo) {
            printf("Enemigo %d: tipo=%d pos=(%d,%d)\n", i, enemigos[i].tipo, enemigos[i].fila, enemigos[i].columna);
        }
    }
}
