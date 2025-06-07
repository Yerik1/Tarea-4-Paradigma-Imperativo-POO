#ifndef VIDAS_H
#define VIDAS_H

typedef struct GameState GameState;

void inicializar_vidas(GameState* game, int vidas);
int obtener_vidas(GameState* game, int idx);
void perder_vida(GameState* game, int idx);
int esta_vivo(GameState* game, int idx);
void reiniciar_ambos_jugadores_si_uno_muere(GameState* game);
void reiniciar_juego_si_uno_muere(GameState* game);
void penalizar_jugador_rezagado(GameState* game);
void penalizar_jugador_rezagado_avanzado(GameState* game);
void sumar_vida_a_ambos_jugadores(GameState* game);
void sumar_vida_jugador(GameState* game, int idx);

#endif
