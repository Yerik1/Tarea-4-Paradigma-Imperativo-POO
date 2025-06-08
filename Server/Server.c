#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include "Server.h"
#include "Juego.h"

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTES 6
#define MAX_JUEGOS 2
#define MAX_JUGADORES_POR_JUEGO 1
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
            if (juegos[i].activo == 0) {
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
            mover(&juegos[juego_id-1].juego,jugador-1, dx); // Puedes adaptar el segundo parámetro según necesites
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

#include <windows.h>  // Para Sleep
#include <process.h>  // Para _beginthreadex

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTES 6
#define PUERTO 12345

SOCKET clientes[MAX_CLIENTES];
CRITICAL_SECTION cs;  // Para proteger acceso a clientes[]

unsigned __stdcall recibir_comandos(void* arg) {
    char buffer[1024];
    while (1) {

        for (int i = 0; i < MAX_CLIENTES; i++) {
            SOCKET s = clientes[i];
            if (s != 0) {
                int valread = recv(s, buffer, sizeof(buffer) - 1, MSG_PEEK);
                if (valread > 0) {
                    valread = recv(s, buffer, sizeof(buffer) - 1, 0);
                    buffer[valread] = '\0';
                    printf("Comando recibido: %s", buffer);
                    procesar_comando(s, buffer);
                } else if (valread == 0 || WSAGetLastError() == WSAECONNRESET) {
                    printf("Cliente desconectado\n");
                    closesocket(s);
                    clientes[i] = 0;
                }
            }
        }
        Sleep(50);
    }
    return 0;
}

void enviar_actualizacion(SOCKET socket_cliente, int matriz[28][20], int vidas[2], int puntaje) {
    char mensaje[4096];  // Buffer suficientemente grande
    strcpy(mensaje, "{\"matriz\":[");

    for (int i = 0; i < 28; ++i) {
        strcat(mensaje, "[");
        for (int j = 0; j < 20; ++j) {
            char temp[8];
            sprintf(temp, "%d", matriz[i][j]);
            strcat(mensaje, temp);
            if (j < 19) strcat(mensaje, ",");
        }
        strcat(mensaje, "]");
        if (i < 27) strcat(mensaje, ",");
    }

    // Agrega campos adicionales
    char final[128];
    sprintf(final, "],\"vidas1\":%d,\"vidas2\":%d,\"puntaje\":%d}", vidas[0],vidas[1], puntaje);
    strcat(mensaje, final);


    // Enviar
    send(socket_cliente, mensaje, strlen(mensaje), 0);

}

unsigned __stdcall enviar_actualizaciones(void* arg) {
    SOCKET destinos[MAX_CLIENTES];
    int total_destinos;

    while (1) {

        total_destinos = 0;

        for (int i = 0; i < MAX_CLIENTES; i++) {
            if (clientes[i] != 0 &&
               (clientes[i] == juegos[0].jugadores[0] ||
                clientes[i] == juegos[0].observadores[0] ||
                clientes[i] == juegos[0].observadores[1])) {
                destinos[total_destinos++] = clientes[i];
                }else if (clientes[i] != 0 &&
               (clientes[i] == juegos[1].jugadores[0] ||
                clientes[i] == juegos[1].observadores[0] ||
                clientes[i] == juegos[1].observadores[1])) {
                    destinos[total_destinos++] = clientes[i];
                }

        }

        // Enviar fuera del lock
        for (int i = 0; i < total_destinos; i++) {
            enviar_actualizacion(destinos[i],
                                 juegos[0].juego.matriz,
                                 juegos[0].juego.vidas_jugador,
                                 juegos[0].juego.puntaje);
        }

        Sleep(200);
    }

    return 0;
}

// Estructura para pasar múltiples argumentos al hilo
typedef struct {
    SOCKET socket;
    int index;
} ClienteArgs;

// Función que manejará cada cliente de forma independiente
unsigned __stdcall manejar_cliente(void* arg) {
    ClienteArgs* args = (ClienteArgs*)arg;
    SOCKET cliente = args->socket;
    int idx = args->index;
    free(arg); // Liberamos memoria reservada

    char buffer[1024];
    int recibido;

    while (1) {
        recibido = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        if (recibido <= 0) {
            printf("Cliente %d desconectado.\n", idx);
            closesocket(cliente);

            EnterCriticalSection(&cs);
            clientes[idx] = 0;
            LeaveCriticalSection(&cs);
            break;
        }

        buffer[recibido] = '\0';  // Asegurar terminación
        printf("[Cliente %d] Comando recibido: %s\n", idx, buffer);

        procesar_comando(cliente, buffer);
    }

    return 0;
}

void start() {
    WSADATA wsa;
    SOCKET servidor;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(client_addr);

    InitializeCriticalSection(&cs);
    WSAStartup(MAKEWORD(2, 2), &wsa);
    servidor = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PUERTO);

    bind(servidor, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(servidor, MAX_CLIENTES);

    printf("Servidor esperando conexiones...\n");

    // Puedes dejar tus hilos globales también si son útiles
    _beginthreadex(NULL, 0, recibir_comandos, NULL, 0, NULL);
    _beginthreadex(NULL, 0, enviar_actualizaciones, NULL, 0, NULL);

    while (1) {
        SOCKET nuevo = accept(servidor, (struct sockaddr*)&client_addr, &addrlen);
        if (nuevo != INVALID_SOCKET) {
            EnterCriticalSection(&cs);
            for (int i = 0; i < MAX_CLIENTES; i++) {
                if (clientes[i] == 0) {
                    clientes[i] = nuevo;
                    send(nuevo, "Conectado al servidor.\n", 24, 0);

                    // Crear argumentos para el nuevo hilo
                    ClienteArgs* args = malloc(sizeof(ClienteArgs));
                    args->socket = nuevo;
                    args->index = i;

                    _beginthreadex(NULL, 0, manejar_cliente, args, 0, NULL);
                    break;
                }
            }
            LeaveCriticalSection(&cs);
        }
    }

    DeleteCriticalSection(&cs);
    WSACleanup();
}
