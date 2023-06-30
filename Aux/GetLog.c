#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Código de teste para pegar as informações do log do kernel. No ubuntu, há um arquivo de texto do log do kernel.

int main() {
    int fd;
    fd_set fds;
    struct timeval timeout;

    // Abrir o arquivo /dev/kmsg
    fd = open("/dev/kmsg", O_RDONLY);
    if (fd < 0) {
        perror("Erro ao abrir o arquivo /dev/kmsg");
        exit(1);
    }

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

    return 0;
}
