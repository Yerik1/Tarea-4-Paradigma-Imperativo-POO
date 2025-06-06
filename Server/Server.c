#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include "Server.h"
#include "Juego.h"

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTES 6
#define MAX_JUEGOS 2
#define MAX_JUGADORES_POR_JUEGO 2
#define MAX_OBSERVADORES_POR_JUEGO 2

Juego juegos[MAX_JUEGOS];

void inicializar_juegos() {
    for (int i = 0; i < MAX_JUEGOS; ++i) {
        juegos[i].num_jugadores = 0;
        juegos[i].num_observadores = 0;
        juegos[i].activo = 0;
        for (int j = 0; j < MAX_JUGADORES_POR_JUEGO; ++j) juegos[i].jugadores[j] = INVALID_SOCKET;
        for (int j = 0; j < MAX_OBSERVADORES_POR_JUEGO; ++j) juegos[i].observadores[j] = INVALID_SOCKET;
    }
}

void procesar_comando(SOCKET cliente, const char* buffer) {
    char respuesta[256];
    int juego_id, jugador;
    char direccion[16];

    if (strncmp(buffer, "Crear Juego", 11) == 0) {
        for (int i = 0; i < MAX_JUEGOS; ++i) {
            if (juegos[i].num_jugadores < MAX_JUGADORES_POR_JUEGO) {
                juegos[i].jugadores[juegos[i].num_jugadores++] = cliente;
                juegos[i].activo = 1;
                iniciar_juego(&juegos[i].juego);
                printf("\n");
                sprintf(respuesta, "Jugador asignado al juego %d\n", i + 1);
                send(cliente, respuesta, strlen(respuesta), 0);
                return;
            }
        }
        send(cliente, "No hay espacio para más juegos.\n", 33, 0);
    }
    else if (sscanf(buffer, "Observar Juego %d", &juego_id) == 1) {
        if (juego_id >= 1 && juego_id <= MAX_JUEGOS) {
            Juego* juego = &juegos[juego_id - 1];
            if (juego->num_observadores < MAX_OBSERVADORES_POR_JUEGO) {
                juego->observadores[juego->num_observadores++] = cliente;
                sprintf(respuesta, "Observando juego %d\n", juego_id);
                send(cliente, respuesta, strlen(respuesta), 0);
            } else {
                send(cliente, "Juego ya tiene 2 observadores.\n", 31, 0);
            }
        } else {
            send(cliente, "Número de juego inválido.\n", 26, 0);
        }
    }
    else if (sscanf(buffer, "Mover Jugador %d %s %d", &jugador, direccion, &juego_id) == 3) {
        if (juego_id >= 1 && juego_id <= MAX_JUEGOS && (jugador == 1 || jugador == 2)) {
            Juego* juego = &juegos[juego_id - 1];
            int dx = 0, dy = 0;

            // Convertir la dirección en movimiento
            if (strcmp(direccion, "derecha") == 0) {
                dx = 1;  // Código 1
                dy = 0;
            } else if (strcmp(direccion, "izquierda") == 0) {
                dx = 2;  // Código 2
                dy = 0;
            } else if (strcmp(direccion, "arriba") == 0) {
                dx = 3;  // Código 3
                dy = 1;
            } else if (strcmp(direccion, "abajo") == 0) {
                dx = 4;  // Código 4
                dy = 0;
            } else {
                send(cliente, "Dirección inválida.\n", 22, 0);
                return;
            }

            // Mover al jugador correspondiente
            mover(&juegos[juego_id-1].juego,&juegos[juego_id-1].juego.jugadores[jugador - 1], dx, dy); // Puedes adaptar el segundo parámetro según necesites
            printf("\n");


            sprintf(respuesta, "Jugador %d del juego %d se mueve a %s \n", jugador, juego_id, direccion);
            send(cliente, respuesta, strlen(respuesta), 0);
        } else {
            send(cliente, "Movimiento inválido.\n", 22, 0);
        }
    }
    else if (strncmp(buffer, "Cerrar Juego", 12) == 0) {
        for (int i = 0; i < MAX_JUEGOS; ++i) {
            Juego* juego = &juegos[i];
            int encontrado = 0;
            for (int j = 0; j < juego->num_jugadores; ++j) {
                if (juego->jugadores[j] == cliente) {
                    juego->jugadores[j] = INVALID_SOCKET;
                    juego->num_jugadores--;
                    encontrado = 1;
                }
            }
            for (int j = 0; j < juego->num_observadores; ++j) {
                if (juego->observadores[j] == cliente) {
                    juego->observadores[j] = INVALID_SOCKET;
                    juego->num_observadores--;
                    encontrado = 1;
                }
            }
            if (juego->num_jugadores == 0 && juego->num_observadores == 0) {
                juego->activo = 0;
            }
            if (encontrado) {
                send(cliente, "Conexión cerrada del juego.\n", 29, 0);
                return;
            }
        }
        send(cliente, "No estás en ningún juego.\n", 27, 0);
    }
    else {
        send(cliente, "Comando no reconocido.\n", 24, 0);
    }
}

void start() {
    WSADATA wsa;
    SOCKET servidor, clientes[MAX_CLIENTES];
    struct sockaddr_in server_addr, client_addr;
    int max_sd, actividad, addrlen, new_socket, valread;
    fd_set readfds;
    char buffer[1024];

    inicializar_juegos();
    for (int i = 0; i < MAX_CLIENTES; i++) clientes[i] = 0;

    printf("Iniciando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error en WSAStartup: %d\n", WSAGetLastError());
        return;
    }

    servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor == INVALID_SOCKET) {
        printf("Error creando socket: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    if (bind(servidor, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Error en bind: %d\n", WSAGetLastError());
        closesocket(servidor);
        WSACleanup();
        return;
    }

    listen(servidor, 6);
    printf("Servidor esperando conexiones en el puerto 12345...\n");

    addrlen = sizeof(client_addr);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(servidor, &readfds);
        max_sd = servidor;

        for (int i = 0; i < MAX_CLIENTES; i++) {
            SOCKET s = clientes[i];
            if (s > 0) FD_SET(s, &readfds);
            if (s > max_sd) max_sd = s;
        }

        actividad = select(0, &readfds, NULL, NULL, NULL);
        if (actividad == SOCKET_ERROR) {
            printf("Error en select: %d\n", WSAGetLastError());
            break;
        }

        if (FD_ISSET(servidor, &readfds)) {
            new_socket = accept(servidor, (struct sockaddr*)&client_addr, &addrlen);
            if (new_socket != INVALID_SOCKET) {
                for (int i = 0; i < MAX_CLIENTES; i++) {
                    if (clientes[i] == 0) {
                        clientes[i] = new_socket;
                        send(new_socket, "Bienvenido al servidor de juegos.\n", 34, 0);
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTES; i++) {
            SOCKET s = clientes[i];
            if (FD_ISSET(s, &readfds)) {
                valread = recv(s, buffer, sizeof(buffer) - 1, 0);
                if (valread <= 0) {
                    closesocket(s);
                    clientes[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    procesar_comando(s, buffer);
                }
            }
        }
    }

    closesocket(servidor);
    WSACleanup();
}
