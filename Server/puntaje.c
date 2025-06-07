#include "puntaje.h"
#include "mapa.h"
#include "game_state.h"

void sumar_puntaje(GameState* game, int puntos) {
    game->puntaje += puntos;
}

int obtener_puntaje(GameState* game) {
    return game->puntaje;
}

void reiniciar_puntaje(GameState* game) {
    game->puntaje = 0;
}

void actualizar_puntaje_por_fruta(GameState* game, int fila, int columna) {
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
