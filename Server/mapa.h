#ifndef MAPA_H
#define MAPA_H

#include "Juego.h"

void inicializar_mapa(Partida* game);
void colocar_en_mapa(Partida* game, int fila, int columna, int valor);
void eliminar_del_mapa(Partida* game, int fila, int columna);
int obtener_mapa(Partida* game, int fila, int columna);
void imprimir_mapa(Partida* game);

#endif