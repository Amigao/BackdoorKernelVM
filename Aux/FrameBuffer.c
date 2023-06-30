#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <linux/fb.h>

#define FRAMEBUFFER_DEVICE "/dev/fb0"

void captureScreen() {
    int fb_fd = open(FRAMEBUFFER_DEVICE, O_RDWR);

    if (fb_fd < 0) {
        perror("Falha ao abrir o dispositivo do framebuffer");
        return;
    }

    // Verifica se o framebuffer está pronto para leitura
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fb_fd, &fds);
    
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int ready = select(fb_fd + 1, &fds, NULL, NULL, &timeout);
    if (ready == -1) {
        perror("Falha ao verificar o estado do framebuffer");
        close(fb_fd);
        return;
    } else if (ready == 0) {
        printf("Timeout: O framebuffer não está pronto para leitura\n");
        close(fb_fd);
        return;
    }

    struct fb_var_screeninfo var_info;
    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &var_info) < 0) {
        perror("Falha ao obter informações do framebuffer");
        close(fb_fd);
        return;
    }

    size_t screen_size = var_info.yres_virtual * var_info.xres_virtual * (var_info.bits_per_pixel / 8);

    uint8_t *screen_buffer = malloc(screen_size);
    if (screen_buffer == NULL) {
        perror("Falha ao alocar memória para a captura de tela");
        close(fb_fd);
        return;
    }

    ssize_t bytes_read = read(fb_fd, screen_buffer, screen_size);
    if (bytes_read != screen_size) {
        perror("Falha ao ler os dados do framebuffer");
        free(screen_buffer);
        close(fb_fd);
        return;
    }

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

    free(screen_buffer);
    close(fb_fd);
}

int main() {
    captureScreen();
    return 0;
}
