#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 1234

// Código usado para testar a criação do socket entre máquina virtual e Windows.

int main() {
    int sockfd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char buffer[1024];
    int connection_closed = 0;

    // Cria o socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Erro ao criar o socket");
        exit(1);
    }

    // Configura os detalhes do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Associa o socket ao endereço e porta
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erro ao fazer o bind");
        exit(1);
    }

    // Aguarda por conexões
    if (listen(sockfd, 1) == -1) {
        perror("Erro ao ouvir por conexões");
        exit(1);
    }

    printf("Servidor aguardando por conexões...\n");

    // Aceita a conexão do cliente
    client_addr_len = sizeof(client_addr);
    client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_sock == -1) {
        perror("Erro ao aceitar a conexão");
        exit(1);
    }

    printf("Cliente conectado\n");
    char prefixo[] = "Máquina virtual recebeu: ";
    while (!connection_closed) {
        // Recebe dados do cliente
        int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            perror("Erro ao receber dados");
            exit(1);
        } else if (bytes_received == 0) {
            printf("Conexão encerrada pelo cliente\n");
            connection_closed = 1; // Encerra o laço do servidor
            continue;
        }

        printf("Recebido: %s\n", buffer);

        // Envia uma resposta para o cliente
        // Move o conteúdo atual para o final do buffer
        memmove(buffer + strlen(prefixo), buffer, strlen(buffer) + 1);
        // Copia o prefixo para o começo da string buffer
        strncpy(buffer, prefixo, strlen(prefixo));
        if (send(client_sock, buffer, strlen(buffer) + 10, 0) == -1) {
            perror("Erro ao enviar resposta");
            exit(1);
        }
    }

    // Fecha a conexão com o cliente
    close(client_sock);

    // Fecha o socket do servidor
    close(sockfd);

    return 0;
}
