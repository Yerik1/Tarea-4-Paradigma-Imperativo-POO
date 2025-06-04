#ifndef ENEMIGOS_H
#define ENEMIGOS_H

typedef enum { YETI, FOCA, AVE, HIELO } TipoEnemigo;
typedef enum { IZQUIERDA, DERECHA, ARRIBA, ABAJO } Direccion;

typedef struct {
    TipoEnemigo tipo;
    int fila;
    int columna;
    Direccion direccion;
    int activo; // 0 si no está en juego, 1 si sí
} Enemigo;

#define MAX_ENEMIGOS 20

extern Enemigo enemigos[MAX_ENEMIGOS];
extern const int MAX_ENEMIGOS_CONST;

void inicializar_enemigos();
void crear_enemigo(TipoEnemigo tipo, int fila, int columna, Direccion direccion);
void mover_enemigos();
void imprimir_enemigos();
void enviar_enemigos_por_socket(int socket_fd); // si usas sockets más adelante
void crear_yeti(int fila, Direccion direccion);
void crear_foca(int fila, Direccion direccion);
void crear_ave(int fila);
void crear_hielo(int fila, int columna);

#endif
