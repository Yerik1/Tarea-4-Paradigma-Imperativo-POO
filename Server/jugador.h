#ifndef JUGADOR_H
#define JUGADOR_H


typedef struct Partida Partida;

typedef struct {
    int fila;
    int columna;
} Jugador;

void inicializar_jugador(Partida* game, int idx, int fila, int columna);
void mover_jugador(Partida* game, int idx, int movimiento);

#endif // JUGADOR_H
