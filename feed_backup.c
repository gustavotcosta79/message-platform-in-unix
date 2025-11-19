#include "shared.h"
#include <errno.h>

void error_exit(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

void imprimir_topicos (const Topico array_topicos[], int num_topicos)
{
    if (num_topicos == 0)
    {
        printf("Nenhum utilizador registado.\n");
        return;
    }

    printf("Lista de Topicos:\n");
    for (int i = 0; i <num_topicos; i++)
    {

        printf("\nTopicos: %d:\n", i + 1);
        printf("  Nome: %s\n", array_topicos[i].nome);
    }
}

void leComando(char *comando, char *argumento1, char *argumento2, char *argumento3,TDADOS *dados)
{

    if (strcmp(comando, "topics") == 0)
    {
        dados->tipo = 2;
        
    }

    else if (strncmp(comando, "msg", 3) == 0)
    {
        dados->tipo = 3;
        char comando_msg[4];
        if (sscanf(comando, "%3s %s %s %s", comando_msg, argumento1, argumento2, argumento3) != 4)
        {
            printf("Formato invalido para o comando'msg'. Deve ser msg <topico> <duracao> <mensagem> ");
        }
        else
        {
            // enviarMensagem(argumento1,argumenro2,argumento3 );
            printf("A enviar mensagem");
        }
    }
    else if (strncmp(comando, "subscribe", 9) == 0)
    {
        dados->tipo = 4;
        char comando_subscribe[9];
        if (sscanf(comando, "%9s %s", comando_subscribe, argumento1) != 2)
        {
            printf("Formato invalido para o comando 'subscribe'. Deve ser subscribe <topico>");
        }
        else
        {
            printf("Foi subscrito no topico %s", argumento1);
        }
    }
    else if (strncmp(comando, "unsubscribe", 11) == 0)
    {
        dados->tipo = 5;
        char comando_subscribe[11];

        if (sscanf(comando, "%11s %s", comando_subscribe, argumento1) != 2)
        {
            printf("Formato invalido para o comando 'unsubscribe'. Deve ser unsubscribe <topico>");
        }
    }

    else if (strcmp(comando, "exit") == 0)
    {
        dados->tipo = 6;
        dados->stop= 1;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2 || strlen(argv[1]) >= 50)
    {
        error_exit("Nome de utilizador invalido\n");
    }

    // Utilizador user;
    TDADOS d;
    // int sair = 0;
    d.stop = 1;
    d.tipo = 1;

    
    TDADOS resposta_servidor;
    char comando[MAX_COMANDO], argumento1[MAX_COMANDO], argumento2[MAX_COMANDO], argumento3[MAX_COMANDO];

    // Preencher a estrutura com os dados do utilizador
    strcpy(d.user.username, argv[1]);
    // snprintf(user.username, sizeof(user.username), "%s", argv[1]);
    snprintf(d.user.pipe_cliente, sizeof(d.user.pipe_cliente), PIPE_CLIENTE, getpid());
    printf("%s\n\n", d.user.pipe_cliente);

    if (mkfifo(d.user.pipe_cliente, 0666) == -1)
    { // Cria o FIFO do cliente
        if (errno == EEXIST)
        {
            error_exit("PIPE ja existe.\n");
        }

        error_exit("Erro ao criar pipe.\n");
    }

    // 3º - enviar a informacao do user para o manager
    int fd_envia = open(PIPE_SERVIDOR, O_WRONLY);
    if (fd_envia == -1)
    {
        error_exit("Erro ao abrir pipe do servidor\n");
    }
    if (write(fd_envia, &d, sizeof(d)) == -1)
    {
        error_exit("Erro ao enviar mensagem.\n");
    }

    close(fd_envia);

    // 6º - abrir para leitura (resposta do servidor)
    int fd_resposta = open(d.user.pipe_cliente, O_RDONLY);
    if (fd_resposta == -1)
    {
        unlink(d.user.pipe_cliente);
        error_exit("Erro ao abrir o pipe do cliente.");
    }

    int size2 = read(fd_resposta, &resposta_servidor, sizeof(resposta_servidor));

    if (size2 > 0){
    printf("[servidor]: %s", resposta_servidor.resposta);
    }
    else {
        error_exit ("Erro ao receber resposta do servidor\n");
    }
    close(fd_resposta);

    if (resposta_servidor.sucesso == 1)
    {
        do
        {
            scanf("%s", comando);

            leComando(comando, argumento1, argumento2, argumento3,&d);
            if (d.stop == 0)
            {
                int fd_envia = open(PIPE_SERVIDOR, O_WRONLY);
                if (fd_envia == -1)
                {
                    error_exit("Erro ao abrir pipe do servidor\n");
                }
                if (write(fd_envia, comando, sizeof(comando)) == -1)
                {
                    error_exit("Erro ao enviar mensagem.\n");
                }              
                close(fd_envia);

            }
        } while (d.stop);
    }

    // close(fd_resposta);
    unlink(d.user.pipe_cliente);

    return 0;
}