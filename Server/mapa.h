#ifndef MAPA_H
#define MAPA_H

typedef struct GameState GameState;

void inicializar_mapa(GameState* game);
void colocar_en_mapa(GameState* game, int fila, int columna, int valor);
void eliminar_del_mapa(GameState* game, int fila, int columna);
int obtener_mapa(GameState* game, int fila, int columna);
void imprimir_mapa(GameState* game);

#endif