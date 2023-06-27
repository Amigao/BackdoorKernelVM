#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#define FRAMEBUFFER_DEVICE "/dev/fb0" // Caminho para o dispositivo do framebuffer

void captureScreen() {
    int fb_fd = open(FRAMEBUFFER_DEVICE, O_RDWR); // Abre o dispositivo do framebuffer

    if (fb_fd < 0) {
        perror("Falha ao abrir o dispositivo do framebuffer");
        return;
    }

    // Obtém informações do framebuffer
    struct fb_var_screeninfo var_info;
    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &var_info) < 0) {
        perror("Falha ao obter informações do framebuffer");
        close(fb_fd);
        return;
    }

    // Calcula o tamanho da captura de tela em bytes
    size_t screen_size = var_info.yres_virtual * var_info.xres_virtual * (var_info.bits_per_pixel / 8);

    // Aloca um buffer para armazenar os dados da captura de tela
    uint8_t *screen_buffer = malloc(screen_size);
    if (screen_buffer == NULL) {
        perror("Falha ao alocar memória para a captura de tela");
        close(fb_fd);
        return;
    }

    // Lê os dados do framebuffer para o buffer da captura de tela
    ssize_t bytes_read = read(fb_fd, screen_buffer, screen_size);
    if (bytes_read != screen_size) {
        perror("Falha ao ler os dados do framebuffer");
        free(screen_buffer);
        close(fb_fd);
        return;
    }

    // Verifica se a imagem capturada está vazia
    int is_empty = 1;
    for (size_t i = 0; i < screen_size; i++) {
        if (screen_buffer[i] != 0) {
            is_empty = 0;
            break;
        }
    }

    if (is_empty) {
        printf("A imagem capturada está vazia\n");
    } else {
        printf("A imagem capturada não está vazia\n");
    }

    // Libera a memória alocada e fecha o dispositivo do framebuffer
    free(screen_buffer);
    close(fb_fd);
}

int main() {
    captureScreen();
    return 0;
}
