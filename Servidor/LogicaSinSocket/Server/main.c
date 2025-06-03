#include <stdio.h>
#include "mapa.h"
#include "jugador.h"
#include "enemigos.h"

/* 
int main() {
    Jugador jugador1;
    
    inicializar_mapa();                     // Genera el mapa con bloques
    inicializar_jugador(&jugador1, 27, 1);  // Pone al jugador en la posición inicial
    
    imprimir_mapa_actual();

    mover_jugador(&jugador1, 3);  // salto
    mover_jugador(&jugador1, 1);  // derecha
    mover_jugador(&jugador1, 3);  // salto
    mover_jugador(&jugador1, 2);  // izquierda
    mover_jugador(&jugador1, 2);  // izquierda
    mover_jugador(&jugador1, 4);  // salto diagonal derecha

    imprimir_mapa_actual();

    return 0;
}
*/
int main() {
    inicializar_mapa();
    inicializar_enemigos();
    // Ejemplo: crear algunos enemigos válidos
    crear_enemigo(YETI, 13, 5, DERECHA);
    crear_enemigo(FOCA, 15, 10, IZQUIERDA);
    crear_enemigo(AVE, 21, 3, ARRIBA);
    imprimir_mapa_actual();
    imprimir_enemigos();

    // Simulación de turnos
    for (int turno = 1; turno <= 5; turno++) {
        printf("\n--- TURNO %d ---\n", turno);
        mover_enemigos();
        imprimir_mapa_actual();
        imprimir_enemigos();
    }

    return 0;
}