#include <stdio.h>
#include "mapa.h"
#include "jugador.h"
#include "enemigos.h"
#include "vidas.h"
#include "frutas.h"
#include "puntaje.h"

int main() {
    Jugador jugadores[2];
    int num_jugadores = 2;
    inicializar_mapa();
    inicializar_vidas(3);
    inicializar_jugador(&jugadores[0], 27, 9); // Jugador 1 inicia abajo, columna 9
    inicializar_jugador(&jugadores[1], 27, 10); // Jugador 2 inicia abajo, columna 10

    inicializar_enemigos(); // Inicializa el array de enemigos

    // Inicializa algunas frutas para probar
    generar_naranjas();
    imprimir_mapa(jugadores, num_jugadores);

    int proximos_movimientos[2];
    int opcion;
    do {
        printf("\nElige tu movimiento para el Jugador 1:\n");
        printf("1. Mover derecha\n");
        printf("2. Mover izquierda\n");
        printf("3. Saltar\n");
        printf("4. Salto diagonal derecha\n");
        printf("5. Salto diagonal izquierda\n");
        printf("6. Golpear\n");
        printf("7. Crear enemigo\n");
        printf("8. Generar fruta\n");
        printf("0. Salir\n");
        printf("Opción: ");
        scanf("%d", &opcion);
        if (opcion == 0) break;
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
                crear_yeti(fila, direccion);
            } else if (tipo == 1) {
                printf("Direccion (0=DERECHA, 1=IZQUIERDA): ");
                scanf("%d", &direccion);
                crear_foca(fila, direccion);
            } else if (tipo == 2) {
                crear_ave(fila);
            } else if (tipo == 3) {
                crear_hielo(fila, columna);
            } else {
                printf("Tipo de enemigo no válido\n");
            }
            imprimir_mapa(jugadores, num_jugadores);
            continue;
        }
        if (opcion == 8) {
            int fruta;
            printf("Tipo de fruta (1=Naranja, 2=Lechuga, 3=Banano, 4=Berenjena): ");
            scanf("%d", &fruta);
            if (fruta == 1) generar_naranjas();
            else if (fruta == 2) generar_lechugas();
            else if (fruta == 3) generar_bananos();
            else if (fruta == 4) generar_berenjenas();
            else printf("Tipo de fruta no válido\n");
            imprimir_mapa(jugadores, num_jugadores);
            continue;
        }
        proximos_movimientos[0] = opcion;
        proximos_movimientos[1] = 0; // Jugador 2 no se mueve

        mover_jugador(jugadores, 0, proximos_movimientos[0], num_jugadores);
        // mover_jugador(jugadores, 1, proximos_movimientos[1], num_jugadores);
        mover_enemigos(jugadores, num_jugadores, proximos_movimientos);

        printf("\n--- Estado actual ---\n");
        printf("Vidas Jugador 1: %d\n", obtener_vidas(0));
        printf("Puntaje: %d\n", obtener_puntaje());
        imprimir_mapa(jugadores, num_jugadores);

        reiniciar_juego_si_uno_muere(jugadores, num_jugadores);
    } while (1);

    return 0;
}