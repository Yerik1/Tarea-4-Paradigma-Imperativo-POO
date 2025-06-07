#ifndef PUNTAJE_H
#define PUNTAJE_H
typedef struct GameState GameState;

void sumar_puntaje(GameState* game, int puntos);
int obtener_puntaje(GameState* game);
void reiniciar_puntaje(GameState* game);
void actualizar_puntaje_por_fruta(GameState* game, int fila, int columna);

#endif
