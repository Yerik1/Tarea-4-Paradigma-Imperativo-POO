#include "frutas.h"
#include "mapa.h"
#include <stdio.h>

void generar_naranjas() {
    colocar_en_mapa(7, 5, 16);
    colocar_en_mapa(7, 14, 16);
    colocar_en_mapa(3, 5, 16);
    colocar_en_mapa(3, 14, 16);
}

void generar_bananos() {
    colocar_en_mapa(7, 5, 15);
    colocar_en_mapa(7, 14, 15);
    colocar_en_mapa(3, 5, 15);
    colocar_en_mapa(3, 14, 15);
}

void generar_berenjenas() {
    colocar_en_mapa(7, 5, 13);
    colocar_en_mapa(7, 14, 13);
    colocar_en_mapa(3, 5, 13);
    colocar_en_mapa(3, 14, 13);
}

void generar_lechugas() {
    colocar_en_mapa(7, 5, 14);
    colocar_en_mapa(7, 14, 14);
    colocar_en_mapa(3, 5, 14);
    colocar_en_mapa(3, 14, 14);
}
