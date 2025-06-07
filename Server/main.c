#include <stdio.h>
#include "game_state.h"
#include "mapa.h"
#include "jugador.h"
#include "enemigos.h"
#include "vidas.h"
#include "frutas.h"
#include "puntaje.h"

int main() {
    GameState game1, game2;
    int juego_activo = 1;
    GameState* game = &game1;
    // Inicialización de ambos juegos
    for (int i = 0; i < 2; ++i) {
        GameState* g = (i == 0) ? &game1 : &game2;
        g->num_jugadores = 2;
        inicializar_mapa(g);
        inicializar_vidas(g, 3);
        inicializar_jugador(g, 0, 27, 9);
        inicializar_jugador(g, 1, 27, 10);
        inicializar_enemigos(g);
        generar_naranjas(g);
    }
    imprimir_mapa(game);
    int proximos_movimientos[2];
    int opcion;
    do {
        printf("\nJuego activo: %d\n", juego_activo);
        printf("1. Mover derecha\n");
        printf("2. Mover izquierda\n");
        printf("3. Saltar\n");
        printf("4. Salto diagonal derecha\n");
        printf("5. Salto diagonal izquierda\n");
        printf("6. Golpear\n");
        printf("7. Crear enemigo\n");
        printf("8. Generar fruta\n");
        printf("9. Cambiar de juego\n");
        printf("0. Salir\n");
        printf("Opción: ");
        scanf("%d", &opcion);
        if (opcion == 0) break;
        if (opcion == 9) {
            juego_activo = (juego_activo == 1) ? 2 : 1;
            game = (juego_activo == 1) ? &game1 : &game2;
            imprimir_mapa(game);
            continue;
        }
        if (opcion == 7) {
            int tipo, fila, columna, direccion;
            printf("Tipo de enemigo (0=Yeti, 1=Foca, 2=Ave, 3=Hielo): ");
            scanf("%d", &tipo);
            printf("Fila: ");
            scanf("%d", &fila);
            printf("Columna: ");
            scanf("%d", &columna);
            if (tipo == 0) {
                printf("Direccion (0=DERECHA, 1=IZQUIERDA): ");
                scanf("%d", &direccion);
                crear_yeti(game, fila, direccion);
            } else if (tipo == 1) {
                printf("Direccion (0=DERECHA, 1=IZQUIERDA): ");
                scanf("%d", &direccion);
                crear_foca(game, fila, direccion);
            } else if (tipo == 2) {
                crear_ave(game, fila);
            } else if (tipo == 3) {
                crear_hielo(game, fila, columna);
            } else {
                printf("Tipo de enemigo no válido\n");
            }
            imprimir_mapa(game);
            continue;
        }
        if (opcion == 8) {
            int fruta;
            printf("Tipo de fruta (1=Naranja, 2=Lechuga, 3=Banano, 4=Berenjena): ");
            scanf("%d", &fruta);
            if (fruta == 1) generar_naranjas(game);
            else if (fruta == 2) generar_lechugas(game);
            else if (fruta == 3) generar_bananos(game);
            else if (fruta == 4) generar_berenjenas(game);
            else printf("Tipo de fruta no válido\n");
            imprimir_mapa(game);
            continue;
        }
        proximos_movimientos[0] = opcion;
        proximos_movimientos[1] = 0;
        mover_jugador(game, 0, proximos_movimientos[0]);
        mover_enemigos(game, proximos_movimientos);
        printf("\n--- Estado actual ---\n");
        printf("Vidas Jugador 1: %d\n", obtener_vidas(game, 0));
        printf("Vidas Jugador 2: %d\n", obtener_vidas(game, 1));
        printf("Puntaje: %d\n", obtener_puntaje(game));
        imprimir_mapa(game);
        reiniciar_juego_si_uno_muere(game);
    } while (1);
    return 0;
}