#ifndef VIDAS_H
#define VIDAS_H

#include "Juego.h"

void inicializar_vidas(Partida* game, int vidas);
int obtener_vidas(Partida* game, int idx);
void perder_vida(Partida* game, int idx);
int esta_vivo(Partida* game, int idx);
void reiniciar_ambos_jugadores_si_uno_muere(Partida* game);
void reiniciar_juego_si_uno_muere(Partida* game);
void penalizar_jugador_rezagado(Partida* game);
void penalizar_jugador_rezagado_avanzado(Partida* game);
void sumar_vida_a_ambos_jugadores(Partida* game);
void sumar_vida_jugador(Partida* game, int idx);

#endif
