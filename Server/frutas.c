#include "frutas.h"
#include "mapa.h"
#include "Juego.h"
#include <stdio.h>

void generar_naranjas(Partida* game) {
    colocar_en_mapa(game, 7, 5, 16);
    colocar_en_mapa(game, 7, 14, 16);
    colocar_en_mapa(game, 3, 5, 16);
    colocar_en_mapa(game, 3, 14, 16);
}

void generar_bananos(Partida* game) {
    colocar_en_mapa(game, 7, 5, 15);
    colocar_en_mapa(game, 7, 14, 15);
    colocar_en_mapa(game, 3, 5, 15);
    colocar_en_mapa(game, 3, 14, 15);
}

void generar_berenjenas(Partida* game) {
    colocar_en_mapa(game, 7, 5, 13);
    colocar_en_mapa(game, 7, 14, 13);
    colocar_en_mapa(game, 3, 5, 13);
    colocar_en_mapa(game, 3, 14, 13);
}


void generar_lechugas(Partida* game) {
    colocar_en_mapa(game, 7, 5, 14);
    colocar_en_mapa(game, 7, 14, 14);
    colocar_en_mapa(game, 3, 5, 14);
    colocar_en_mapa(game, 3, 14, 14);
}
