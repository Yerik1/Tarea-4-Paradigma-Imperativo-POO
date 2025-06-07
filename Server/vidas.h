#ifndef VIDAS_H
#define VIDAS_H

#include "jugador.h"

void inicializar_vidas(int vidas);
int obtener_vidas(int idx);
void perder_vida(Jugador* jugador, int idx);
int esta_vivo(int idx);
void reiniciar_ambos_jugadores_si_uno_muere(Jugador* jugadores, int num_jugadores);
void reiniciar_juego_si_uno_muere(Jugador* jugadores, int num_jugadores);
void penalizar_jugador_rezagado(Jugador* jugadores, int num_jugadores);
void sumar_vida_a_ambos_jugadores();

#endif
