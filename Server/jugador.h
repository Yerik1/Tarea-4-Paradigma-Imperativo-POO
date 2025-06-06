#ifndef JUGADOR_H
#define JUGADOR_H

typedef struct {
    int fila;
    int columna;
    // ...otros campos si los tienes...
} Jugador;

void inicializar_jugador(Jugador* jugador, int fila, int columna);
void mover_jugador(Jugador* jugadores, int idx, int movimiento, int num_jugadores);

#endif
