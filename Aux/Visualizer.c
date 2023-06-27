#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define OUTPUT_FILE "screenshot.raw" // Nome do arquivo de saída
#define SCREEN_WIDTH 800            // Largura da janela
#define SCREEN_HEIGHT 600           // Altura da janela

void displayImage() {
    // Inicializa o SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Falha ao inicializar o SDL: %s\n", SDL_GetError());
        return;
    }

    // Cria uma janela
    SDL_Window *window = SDL_CreateWindow("Visualização do Arquivo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Falha ao criar a janela: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    // Cria um renderer para desenhar na janela
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Falha ao criar o renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Carrega o arquivo de imagem
    FILE *image_file = fopen(OUTPUT_FILE, "rb");
    if (image_file == NULL) {
        printf("Falha ao abrir o arquivo de imagem: %s\n", OUTPUT_FILE);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Obtém o tamanho do arquivo
    fseek(image_file, 0, SEEK_END);
    long file_size = ftell(image_file);
    fseek(image_file, 0, SEEK_SET);

    // Lê os dados do arquivo para um buffer
    uint8_t *image_buffer = (uint8_t *)malloc(file_size);
    if (image_buffer == NULL) {
        printf("Falha ao alocar memória para o buffer de imagem\n");
        fclose(image_file);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    fread(image_buffer, 1, file_size, image_file);
    fclose(image_file);

    // Cria uma textura a partir dos dados da imagem
    SDL_Surface *image_surface = SDL_CreateRGBSurfaceFrom(image_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, 8 * sizeof(uint8_t),
                                                          SCREEN_WIDTH * sizeof(uint8_t), 0, 0, 0, 0);
    if (image_surface == NULL) {
        printf("Falha ao criar a superfície de imagem: %s\n", SDL_GetError());
        free(image_buffer);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);
    free(image_buffer);

    if (image_texture == NULL) {
        printf("Falha ao criar a textura de imagem: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Limpa o renderer
    SDL_RenderClear(renderer);

    // Desenha a textura na janela
    SDL_RenderCopy(renderer, image_texture, NULL, NULL);

    // Atualiza a janela
    SDL_RenderPresent(renderer);

    // Aguarda o usuário fechar a janela
    int quit = 0;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    // Libera recursos e finaliza o SDL
    SDL_DestroyTexture(image_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    displayImage();
    return 0;
}
