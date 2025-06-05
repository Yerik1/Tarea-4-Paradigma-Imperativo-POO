#ifndef JUGADOR_H
#define JUGADOR_H

typedef struct {
    int fila;
    int columna;
} Jugador;

void inicializar_jugador(Jugador* jugador, int fila, int columna);
void mover_jugador(Jugador* jugador, int movimiento, int idx);

#endif
