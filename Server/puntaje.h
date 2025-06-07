#ifndef PUNTAJE_H
#define PUNTAJE_H
#include "Juego.h"

void sumar_puntaje(Partida* game, int puntos);
int obtener_puntaje(Partida* game);
void reiniciar_puntaje(Partida* game);
void actualizar_puntaje_por_fruta(Partida* game, int fila, int columna);

#endif
