#ifndef MAPA_H
#define MAPA_H

#include "jugador.h"

void inicializar_mapa();
void colocar_en_mapa(int fila, int columna, int valor);
void eliminar_del_mapa(int fila, int columna);
int obtener_mapa(int fila, int columna);
void imprimir_mapa(Jugador* jugadores, int num_jugadores);
void ver_mapa();

#endif