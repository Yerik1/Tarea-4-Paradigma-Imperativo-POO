#include "puntaje.h"
#include "mapa.h"

static int puntaje = 0;

void sumar_puntaje(int puntos) {
    puntaje += puntos;
}

int obtener_puntaje() {
    return puntaje;
}

void actualizar_puntaje_por_fruta(int fila, int columna) {
    int valor = obtener_mapa(fila, columna);
    switch (valor) {
        case 16: // Naranja
            puntaje += 100;
            break;
        case 15: // Banano
            puntaje += 200;
            break;
        case 13: // Berenjena
            puntaje += 300;
            break;
        case 14: // Lechuga
            puntaje += 400;
            break;
        default:
            break;
    }
}
