#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#define PIPE_SERVIDOR "pipe_servidor"
#define PIPE_CLIENTE "pipe_cliente_%d"
#define MAX_UTILIZADORES 5
#define MAX_TOPICOS 20
#define MAX_LEN_MSG 300
#define MAX_COMANDO 100

typedef struct
{
    char nome[20];
    int bloqueado;
    int nMsg_persistentes;
} Topico;

typedef struct
{   
    char username[50];
    char pipe_cliente[100];
    int topicos_subscritos[MAX_TOPICOS];//!!! ver se é preciso guardar o nome do topico em vez do indice
    int num_topicos_subs;
} Utilizador;

typedef struct
{
    char topico[20];
    char autor[50];
    int duracao;
    char conteudo[300];
} Mensagem;

typedef struct
{
    Utilizador user;
    Utilizador array_users[MAX_UTILIZADORES];
    int num_utilizadores;
    
    int stop;

    int id_msg;
    Mensagem mensagem;
    int id_topico;
    Topico topico;
    Topico array_topicos[MAX_TOPICOS];
    int num_topicos;

    // pthread_mutex_t *m;
    char resposta[MAX_COMANDO];
    int sucesso;
    int indice_topico;
    int tipo;

    const char *arg1;
    const char *arg2;
    const char *arg3;
    
} TDADOS;
//tipo 1 -> dados para adicionar utilizador
//tipo 2 -> topicos
//tipo 3 -> Mensagem
//tipo 4 -> subscribe
//tipo 5 -> unsubscribe

/*



typedef struct
{
    int pipe_a_criado;
    int pipe_b_criado;

    int pipe_a_fd;
    int pipe_a_fd;

    pthread_t thread_a_id
    
} DadosControl;

void fechaEncerra(DadosControl * pdc, const char * msg){
    perror(msg);
    if(pdc -> thread_a_id != 1){
        ...
    }


}


*/








typedef struct
{
    TDADOS dados;
    int tipo;
} Data;



#endif
