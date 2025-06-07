#include "puntaje.h"
#include "mapa.h"
#include "Juego.h"

void sumar_puntaje(Partida* game, int puntos) {
    game->puntaje += puntos;
}

int obtener_puntaje(Partida* game) {
    return game->puntaje;
}

void reiniciar_puntaje(Partida* game) {
    game->puntaje = 0;
}

void actualizar_puntaje_por_fruta(Partida* game, int fila, int columna) {
    int valor = game->matriz[fila][columna];
    switch (valor) {
        case 16: // Naranja
            game->puntaje += 100;
            break;
        case 15: // Banano
            game->puntaje += 200;
            break;
        case 13: // Berenjena
            game->puntaje += 300;
            break;
        case 14: // Lechuga
            game->puntaje += 400;
            break;
        default:
            break;
    }
}
