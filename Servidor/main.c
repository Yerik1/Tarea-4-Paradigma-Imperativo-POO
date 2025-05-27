// servidor_win.c
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // Solo necesario en MSVC, no en MinGW

int main() {
    WSADATA wsa;
    SOCKET servidor, cliente;
    struct sockaddr_in server_addr, client_addr;
    int client_size;
    char buffer[1024];

    printf("Iniciando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error en WSAStartup: %d\n", WSAGetLastError());
        return 1;
    }

    servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor == INVALID_SOCKET) {
        printf("Error creando socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    if (bind(servidor, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Error en bind: %d\n", WSAGetLastError());
        closesocket(servidor);
        WSACleanup();
        return 1;
    }

    listen(servidor, 3);
    printf("Servidor esperando conexiones en el puerto 12345...\n");

    client_size = sizeof(client_addr);
    cliente = accept(servidor, (struct sockaddr*)&client_addr, &client_size);
    if (cliente == INVALID_SOCKET) {
        printf("Error en accept: %d\n", WSAGetLastError());
        closesocket(servidor);
        WSACleanup();
        return 1;
    }

    int recibido = recv(cliente, buffer, sizeof(buffer) - 1, 0);
    if (recibido > 0) {
        buffer[recibido] = '\0';
        printf("Mensaje recibido: %s\n", buffer);
        send(cliente, "Hola desde el servidor", 23, 0);
    }

    closesocket(cliente);
    closesocket(servidor);
    WSACleanup();
    return 0;
}


