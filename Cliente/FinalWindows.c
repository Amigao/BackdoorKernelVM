#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <conio.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1" // Substitua pelo endereço IP da sua máquina virtual
#define SERVER_PORT 1234

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Inicializa o Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Falha ao inicializar o Winsock");
        exit(1);
    }

    // Cria o socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        perror("Falha ao criar o socket");
        WSACleanup();
        exit(1);
    }

    // Configura os detalhes do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Conecta ao servidor na máquina virtual Ubuntu
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("Falha ao conectar");
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    } else{
        printf("Conectado!\n");
    }

    while (1) {
        // Verifica se houve entrada do usuário
        if (_kbhit()) {
            int input = _getch();
            if (input == '1') {
                // Se a entrada for '1', sai do loop
                break;
            }
        }

        // Recebe dados da máquina virtual
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived == SOCKET_ERROR) {
            perror("Falha ao receber dados");
            closesocket(clientSocket);
            WSACleanup();
            exit(1);
        } else if (bytesReceived == 0) {
            printf("Conexao encerrada pelo servidor.\n");
            break;
        } else {
            printf("%s", buffer);
        }
    }

    // Fecha a conexão
    closesocket(clientSocket);

    // Finaliza o Winsock
    WSACleanup();

    return 0;
}
