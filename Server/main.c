#include <stdio.h>
#include "mapa.h"
#include "jugador.h"
#include "enemigos.h"

int main() {
    Jugador jugador1;
    inicializar_mapa();
    inicializar_jugador(&jugador1, 27, 1); // Posición inicial (nivel 1, columna 1)
    inicializar_enemigos();

    // Crear los 4 enemigos en filas y columnas válidas de forma simplificada
    crear_yeti(15, DERECHA);    // Yeti sale de la izquierda
    crear_hielo(11, 4); // Hielo en fila 20, columna 5

    imprimir_mapa_actual();

    // Simular 8 turnos de movimiento de enemigos
    for (int turno = 1; turno <= 15; turno++) {
        printf("\nTurno %d:\n", turno);
        mover_enemigos();
        imprimir_mapa_actual();
    }

    return 0;
}