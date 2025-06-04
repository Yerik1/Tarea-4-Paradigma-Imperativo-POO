#include "vidas.h"
#include "jugador.h"

static int vidas_jugador = 3;

void inicializar_vidas(int vidas) {
    vidas_jugador = vidas;
}

int obtener_vidas() {
    return vidas_jugador;
}

void perder_vida(Jugador* jugador) {
    if (vidas_jugador > 0) {
        vidas_jugador--;
        // Reiniciar jugador al nivel 1 (fila 27, columna 1)
        jugador->fila = 27;
        jugador->columna = 1;
    }
}

int esta_vivo() {
    return vidas_jugador > 0;
}
