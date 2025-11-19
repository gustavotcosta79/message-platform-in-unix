#include "shared.h"
#include <errno.h>
#include <pthread.h>

int verificar_ou_cria_topico(TDADOS *pdados, Topico topico, Utilizador user)
{

    if (pdados->num_topicos >= MAX_TOPICOS)
    { // verificar o limite dos topicos
        printf("Limite de topicos criados atingido!!");
        snprintf(pdados->resposta, 100, "Limite de topicos criados atingido!\n");

        return -1;
    }

    for (int i = 0; i < pdados->num_topicos; i++)
    { // verifica se o topico ja existe
        if (strcmp(pdados->array_topicos[i].nome, topico.nome) == 0)
        {

            pdados->indice_topico = i;
            for (int j = 0; j < pdados->num_utilizadores; j++)
            {
                if (strcmp(pdados->array_users[j].username, user.username) == 0)
                {
                    pdados->array_users[j].topicos_subscritos[pdados->array_users[j].num_topicos_subs] = i;
                    pdados->array_users[j].num_topicos_subs++;
                }
            }
            return 0;
        }
    }

    if (strlen(topico.nome) >= 20)
    {
        printf("Nome do topico demasiado grande!!");
        return -1;
    }
    strcpy(pdados->array_topicos[pdados->num_topicos].nome, topico.nome);
    pdados->array_topicos[pdados->num_topicos].bloqueado = 0; // desbloqueia o topico
    pdados->num_topicos++;
    printf("Topico %s criado com sucesso!", topico.nome);
    snprintf(pdados->resposta, 100, "Topico criado com sucesso!\n");
    pdados->indice_topico = pdados->num_topicos - 1;

    return 0; // topico criado
}

int remover_subscricao_topico(TDADOS *pdados, const char *nome_topico, Utilizador user)
{
    int indice_topico_encontrado = -1;

    // Verificar se o tópico existe
    for (int i = 0; i < pdados->num_topicos; i++)
    {
        if (strcmp(pdados->array_topicos[i].nome, nome_topico) == 0)
        {
            indice_topico_encontrado = i;
            break;
        }
    }

    if (indice_topico_encontrado == -1)
    {
        printf("Topico '%s' nao encontrado.\n", nome_topico);
        snprintf(pdados->resposta, 100, "Topico '%s' nao encontrado.\n", nome_topico);
        return -1; // Tópico não encontrado
    }

    // Verificar se o utilizador está subscrito ao tópico
    int subscrito = 0;
    for (int i = 0; i < user.num_topicos_subs; i++)
    {
        if (user.topicos_subscritos[i] == indice_topico_encontrado)
        {
            subscrito = 1;
            // Remover a subscrição do utilizador 1234->  134

            for (int j = i; j < user.num_topicos_subs - 1; j++)
            {
                user.topicos_subscritos[j] = user.topicos_subscritos[j + 1];
            }
            user.num_topicos_subs--;
            break;
        }
    }

    if (!subscrito)
    {
        snprintf(pdados->resposta, 100, "Utilizador '%s' nao esta subscrito no topico '%s'.\n", user.username, nome_topico);
        return -2; // Utilizador não está subscrito
    }

    // Verificar se outros utilizadores ainda estão subscritos
    int subscritores_restantes = 0;
    for (int i = 0; i < pdados->num_utilizadores; i++)
    { // percorre numero de user
        for (int j = 0; j < pdados->array_users[i].num_topicos_subs; j++)
        { // percorre a cada user o seu numero de topicos
            if (pdados->array_users[i].topicos_subscritos[j] == indice_topico_encontrado)
            {                               // compara todos os topicos de todos os users com o indice do topico encontrado
                subscritores_restantes = 1; // existem  users com este topico(temos de remove los)
                break;
            }
        }
        if (subscritores_restantes)
            break;
    }
    /*
        // Verificar se há mensagens persistentes no tópico
            int mensagens_presentes = 0;
            for (int i = 0; i < pdados->num_mensagens; i++) {
                if (strcmp(pdados->mensagens[i].topico, nome_topico) == 0) {
                    mensagens_presentes = 1;
                    break;
                }
            }
    */

    // Se não houver subscritores restantes e nenhuma mensagem, apagar o tópico
    if (!subscritores_restantes)
    { //&& !mensagens_presentes
        for (int i = indice_topico_encontrado; i < pdados->num_topicos - 1; i++)
        { // remove o topico, pois ja nao ha subscritores
            pdados->array_topicos[i] = pdados->array_topicos[i + 1];
        }
        pdados->num_topicos--;
        printf("Topico '%s' foi removido.\n", nome_topico);
        snprintf(pdados->resposta, 100, "Topico '%s' foi removido por nao ter mais subscritores e mensagens.\n", nome_topico);
        return 0; // Sucesso ao remover o tópico
    }

    // Apenas removeu a subscrição, mas o tópico ainda existe
    snprintf(pdados->resposta, 100, "Utilizador '%s' deixou de subscrever o topico '%s'.\n", user.username, nome_topico);
    printf("Utilizador '%s' deixou de subscrever o topico '%s'.\n", user.username, nome_topico);
    return 1; // Subscrição removida, mas o tópico permanece
}

int adicionar_utilizador(TDADOS *pdados, Utilizador novo)
{
    if (pdados->num_utilizadores >= MAX_UTILIZADORES)
    {
        snprintf(pdados->resposta, 100, "Numero maximo de utilizadores atingido.\n");
        return 0;
    }

    for (int i = 0; i < pdados->num_utilizadores; i++)
    {
        if (strcmp(pdados->array_users[i].username, novo.username) == 0)
        {
            snprintf(pdados->resposta, 100, "O nome %s ja esta a ser usado.\n", novo.username);
            return 0;
        }
    }

    // Adiciona o utilizador
    pdados->array_users[pdados->num_utilizadores] = novo;
    (pdados->num_utilizadores)++;
    snprintf(pdados->resposta, 100, "User %s adicionado com sucesso.\n", novo.username);

    return 1; // Sucesso
}

void imprimir_utilizadores(const Utilizador array_users[], int num_utilizadores)
{
    if (num_utilizadores == 0)
    {
        printf("Nenhum utilizador registado.\n");
        return;
    }

    printf("Lista de utilizadores registados:\n");
    for (int i = 0; i < num_utilizadores; i++)
    {

        printf("\nUtilizador %d:\n", i + 1);
        printf("  Nome: %s\n", array_users[i].username);
    }
}

void imprimir_topicos(const Topico array_topicos[], int num_topicos)
{
    if (num_topicos == 0)
    {
        printf("Nenhum utilizador registado.\n");
        return;
    }

    printf("Lista de Topicos:\n");
    for (int i = 0; i < num_topicos; i++)
    {

        printf("\nTopicos: %d:\n", i + 1);
        printf("  Nome: %s\n", array_topicos[i].nome);
    }
}

void leComando(char *comando, char *argumento1, int *sair, Utilizador array_users[], int num_utilizadores)
{
    if (strcmp(comando, "users") == 0)
    {
        imprimir_utilizadores(array_users, num_utilizadores);
    }

    else if (strncmp(comando, "remove", 6) == 0)
    {
        char comando_msg[6];
        if (sscanf(comando, "%6s %s", comando_msg, argumento1) != 2)
        {
            printf("Formato invalido para o comando'remove'. Deve ser remove <username>");
        }
        else
        {
            for (int i = 0; i < num_utilizadores; i++)
            {
                if (strcmp(array_users[i].username, argumento1) == 0)
                {
                    if (i == num_utilizadores - 1)
                    {
                        num_utilizadores--;
                    }
                    else
                    {
                        array_users[i] = array_users[num_utilizadores - 1];
                        num_utilizadores--;
                    }
                }
            }
        }
    }
    else if (strcmp(comando, "topics") == 0)
    {
    }
    else if (strncmp(comando, "show", 4) == 0)
    {
        char comando_show[4];
        if (sscanf(comando, "%4s %s", comando_show, argumento1) != 2)
        {
            printf("Formato invalido para o comando'show'. Deve ser show <topico>");
        }
    }
    else if (strncmp(comando, "lock", 4) == 0)
    {
        char comando_lock[4];
        if (sscanf(comando, "%6s %s", comando_lock, argumento1) != 2)
        {
            printf("Formato invalido para o comando'lock'. Deve ser lock <topico>");
        }
    }
    else if (strncmp(comando, "unlock", 6) == 0)
    {
        char comando_unlock[6];
        if (sscanf(comando, "%6s %s", comando_unlock, argumento1) != 2)
        {
            printf("Formato invalido para o comando'unlock'. Deve ser unlock <topico>");
        }

        else if (strcmp(comando, "close") == 0)
        {
            *sair = 1;
        }
    }
}

void error_exit(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

void *threadNP(void *dados)
{

    TDADOS *pdados = (TDADOS *)dados;

    int fd_recebe, fd_envia;

    // 1º - criar pipe do servidor
    if (mkfifo(PIPE_SERVIDOR, 0666) == -1)
    { // Cria o FIFO do cliente
        if (errno == EEXIST)
        {
            error_exit("PIPE ja existe.\n");
        }

        error_exit("Erro ao criar pipe.\n");
    }

    printf("Aguardando utilizadores...\n");

    // 2º - abrir o pipe servidor para leitura
    fd_recebe = open(PIPE_SERVIDOR, O_RDWR);
    if (fd_recebe == -1)
    {
        error_exit("Erro ao abrir pipe global\n");
    }

    do
    {
        TDADOS buffer; // guardar mensagem recebida do feed

        // 4º - ler struct enviada pelo cliente
        int bytes_read = read(fd_recebe, &buffer, sizeof(TDADOS));

        if (bytes_read == -1)
        {
            // Nenhum dado disponível, aguardar...
            usleep(100000);
            continue;
        }
        else if (bytes_read > 0)
        {
            ///----------------------- ADICIONAR USER -----------------------

            if (buffer.tipo == 1)
            {
                pdados->sucesso = adicionar_utilizador(pdados, buffer.user);

                imprimir_utilizadores(pdados->array_users, pdados->num_utilizadores);

                fd_envia = open(buffer.user.pipe_cliente, O_WRONLY);

                if (fd_envia == -1)
                {
                    error_exit("Erro ao abrir pipe do cliente\n");
                }

                // 5º - enviar resposta ao cliente
                if (write(fd_envia, pdados, sizeof(TDADOS)) == -1)
                {
                    error_exit("Erro ao enviar mensagem1.\n");
                }
            }
            ///----------------------- ADICIONAR TOPICO -----------------------
            else if (buffer.tipo == 2)
            {
                printf("Pedido de topicos feito por: %s", buffer.user.username);
                fd_envia = open(buffer.user.pipe_cliente, O_WRONLY);

                if (fd_envia == -1)
                {
                    error_exit("Erro ao abrir pipe do cliente\n");
                }

                // 5º - enviar resposta ao cliente
                if (write(fd_envia, pdados, sizeof(pdados) == -1))
                {
                    error_exit("Erro ao enviar mensagem2.\n");
                }
            }
            ///----------------------- MENSAGEM -----------------------
            else if (buffer.tipo == 3)
            {
            }
            ///----------------------- SUBSCRIBE TOPICO -----------------------
            else if (buffer.tipo == 4)
            {
                printf("ENTREI NO TIPO 4 %s\n", buffer.user.pipe_cliente);
                fd_envia = open(buffer.user.pipe_cliente, O_WRONLY);
                pdados->sucesso = verificar_ou_cria_topico(pdados, buffer.topico, buffer.user);
                // return -1 se ultrapassar o maximo de topicos que podem existir ou se o nome do topico for demasiado grande.
                // return 0 se o topico já existir ou se este foi adicionado com sucesso.
                if (write(fd_envia, pdados, sizeof(pdados) == -1))
                {
                    error_exit("Erro ao enviar mensagem3.\n");
                }
                imprimir_topicos(pdados->array_topicos, pdados->num_topicos);
            }
            ///----------------------- UNSUBSCRIBE TOPICO -----------------------
            else if (buffer.tipo == 5)
            {

                pdados->sucesso = remover_subscricao_topico(pdados, buffer.topico.nome, buffer.user);
                //  sucesso = -1 -> Tópico não encontrado
                //  sucesso = 0 -> Sucesso ao remover o tópico (era o ultimo subscritor do topico)
                //  sucesso = 1 -> Subscrição removida, mas o tópico permanece
                //  sucesso = -2 -> Utilizador não está subscrito ao topico que quer dar unsubscribe
                if (write(fd_envia, pdados, sizeof(pdados) == -1))
                {
                    error_exit("Erro ao enviar mensagem4.\n");
                }
            }
            ///----------------------- EXIT -----------------------
            else if (buffer.tipo == 6)
            {
            }

            close(fd_envia);
        }
    } while (pdados->stop);

    // so vamos fechar o fd_recebe quando sair do loop
    close(fd_recebe);
    unlink(PIPE_SERVIDOR);
}

void *threadMensagens(void *dados)
{
}

int main()
{
    char buffer[10];

    pthread_t t[2];
    TDADOS d[2];
    d[0].stop = 1;
    // d[1].stop = 1;

    pthread_create(&t[0], NULL, &threadNP, &d[0]);
    pthread_create(&t[1], NULL, &threadMensagens, &d[1]);

    do
    {
        printf("\nIntroduzir close para fim \n");
        scanf("%s", buffer);

        // leComandos();
        if (strcmp(buffer, "close") == 0)
        {
            d[0].stop = 0;
            // write(... pipe servidor...);
            //  pthread_kill(t,SIGUSR1);
            //close(PIPE_SERVIDOR); //!!!!!!!!!WARNING!!!!!!!!!!!!!!!!
        }
        else
        {
            printf("\nComando Invalido\n");
        }
    } while (strcmp(buffer, "close") != 0);
    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
    printf("\nAdeus\n");

    return 0;
}

// modificar a funcao leComnados;
/// implementar na main o lecomandos();<----------------------------------------------
