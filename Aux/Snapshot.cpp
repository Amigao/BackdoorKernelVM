#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <opencv2/opencv.hpp>

#define FRAMEBUFFER_DEVICE "/dev/fb0" // Caminho para o dispositivo do framebuffer
#define OUTPUT_IMAGE_FILE "screenshot.jpg" // Caminho para o arquivo de imagem de saída

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

    // Mapeia o framebuffer na memória
    uint8_t *screen_buffer = (uint8_t *)mmap(NULL, screen_size, PROT_READ, MAP_SHARED, fb_fd, 0);
    if (screen_buffer == MAP_FAILED) {
        perror("Falha ao mapear o framebuffer");
        close(fb_fd);
        return;
    }

    // Converte o formato do framebuffer para o formato esperado pelo OpenCV
    cv::Mat image(var_info.yres_virtual, var_info.xres_virtual, CV_8UC4, screen_buffer);

    // Salva a imagem no arquivo de saída
    cv::imwrite(OUTPUT_IMAGE_FILE, image);

    // Desfaz o mapeamento do framebuffer
    munmap(screen_buffer, screen_size);
    close(fb_fd);

    printf("Captura de tela salva como '%s'\n", OUTPUT_IMAGE_FILE);
}

int main() {
    captureScreen();
    return 0;
}
