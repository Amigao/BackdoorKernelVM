Aqui está o guia completo:

# BackdoorKernelVM - Configurando a Comunicação entre Máquinas Virtuais e Windows

Este guia descreve as configurações necessárias para estabelecer a comunicação entre máquinas virtuais (VMs) e o sistema operacional Windows. Siga os passos abaixo para configurar corretamente a comunicação entre o servidor (Máquina Virtual Ubuntu) e o cliente (Windows).

## Configuração da Rede Virtual

1. Crie uma rede virtual associada à VM. Essa rede será uma entidade separada da rede física do sistema hospedeiro.

2. Configure regras de encaminhamento para direcionar o tráfego de entrada para a VM desejada. Essas regras garantem que o tráfego seja manipulado e encaminhado corretamente.

## IP Genérico e Network Address Translation (NAT)

1. As VMs geralmente possuem endereços IP internos válidos somente dentro da rede virtual. Para permitir a comunicação com o ambiente externo, crie um IP genérico (endereço IP público) mapeado para o endereço IP interno da VM.

2. Utilize uma rede do tipo Network Address Translation (NAT) para simplificar a configuração. Com o NAT, as VMs podem compartilhar um único endereço IP externo, enquanto cada VM possui um endereço IP interno. A tradução de endereço é realizada para permitir a comunicação com o ambiente externo.

   No nosso caso, estamos usando a máquina virtual da Oracle (VirtualBox); siga o passo a passo disponível em [aqui](https://stackoverflow.com/questions/48138413/how-to-connect-through-socket-to-virtual-machine) para configurar a rede NAT do nosso socket.

## Lidando com o Firewall do Windows

1. Observe que o firewall do Windows pode bloquear o tráfego de rede entrante ou saída, o que pode impactar a comunicação entre a VM e o Windows.

2. Certifique-se de configurar as permissões adequadas no firewall do Windows. Isso pode envolver a abertura de portas específicas ou a criação de regras para permitir o tráfego entre a VM e o Windows. Consulte a documentação oficial do Windows para obter instruções específicas de configuração do firewall.

## Execução do Servidor (Máquina Virtual Ubuntu)

Certifique-se de que o servidor esteja em execução antes de iniciar o cliente. O cliente precisa se conectar a um servidor ativo para estabelecer uma comunicação adequada.

1. Acesse a Máquina Virtual Ubuntu onde o servidor será executado.

2. Abra um terminal e navegue até o diretório onde o código-fonte do servidor está localizado.

3. Utilize o comando `make` para compilar o código e gerar o executável do servidor.

4. Após compilar com sucesso, execute o servidor com o comando `./server`. O servidor começará a aguardar por conexões de clientes.

5. Mantenha o servidor em execução durante o tempo necessário para a comunicação com o cliente.

## Execução do Cliente (Windows)

Após compilar e executar o cliente, ele tentará estabelecer uma conexão com o servidor. Certifique-se de que o servidor esteja em execução e acessível pela rede.

1. Acesse o sistema operacional Windows onde o cliente será executado.

2.

 Abra um prompt de comando ou PowerShell e navegue até o diretório onde o código-fonte do cliente está localizado.

3. Utilize o comando adequado para compilar o código-fonte do cliente, dependendo da linguagem de programação e ferramentas utilizadas.

4. Após compilar com sucesso, execute o cliente com o comando apropriado.

5. O cliente tentará estabelecer uma conexão com o servidor. Se a conexão for estabelecida com sucesso, a comunicação entre o cliente e o servidor estará em andamento.

6. Para encerrar a conexão entre o cliente e o servidor, basta pressionar o botão "1" no teclado enquanto o cliente estiver em execução. Isso enviará um comando para o cliente solicitar o encerramento da conexão com o servidor. O cliente tratará esse comando e encerrará a conexão adequadamente.

É importante lembrar que tanto o servidor quanto o cliente devem estar em execução para estabelecer a comunicação, e o encerramento da conexão pelo cliente pode interromper a comunicação entre as duas partes.

Certifique-se de seguir todas as etapas corretamente e adaptar as configurações de acordo com o seu ambiente específico. Consulte a documentação relevante do software de virtualização e do sistema operacional para obter informações adicionais, se necessário.
