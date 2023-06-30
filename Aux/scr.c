#include <stdlib.h>

int main() {
    const char* command = "scrot screenshot.png";
    int result = system(command);
    
    if (result == -1) {
        printf("Erro ao executar o comando\n");
        return 1;
    }
    
    printf("Captura de tela salva com sucesso\n");
    return 0;
}
