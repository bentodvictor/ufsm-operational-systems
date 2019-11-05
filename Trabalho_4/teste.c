#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#include "queue.h"

#define N 5             /* Quantidade de Setores/Departamentos */
#define QTD 3           /* Quantidade total de solicitações de impressão para cada Setor */

sem_t  id_mutex;        /* semaforo binario para as threads criarem seu id */
sem_t  mutex;           /* semaforo binario utilizado p/ garantir exclusao mutua na regiao critica  */
sem_t  full;            /* semaforo utilizado pra controlar os sinais relativos as posicoes preenchidas no buffer */
sem_t  s[N];            /* semaforo para controlar a impressao dos arquivos do buffer */


/* Declaração da fila da impressora t1 */
Fila* t1;
Fila* t2;

int MAX;                        /* Variável usada para criar os tipos de impressora */
int arquivos[N];                /* Utilizado para armazenar os arquivos a serem impressos */

/* Variaveis para controle do Buffer "arquivos" */
int inicio;       
int final;        
int cont;        

int arquivo;                /* identificador dos arquivos(Threads) */

/* Estrutura para pegar a quantidade de solicitações de cada departamento */
typedef struct departamento{
    int id;
    int quantidade;
}Departamento;
Departamento dep[N];

/* prototipos */
void insere_impressao(int id_arquivo);
int imprime_item();
void realoca();


/************************************** Função para tratamento de entradas inválidas.******************************
int get_inputs(int argc, char* argv[])
{   
    if (argc < 2)           //  Condição caso nenhum argumento seja passado
    {
        printf("NENHUM ARGUMENTO ENCONTRADO!!!\n");
        exit(0);
        MAX = 0;
    }
    else
    {
        if(*argv[1] == '0')       // Se o valor do vetor na posição cont for o caractere '0'
        {
            printf("O ARGUMENTO <0> EH INVALIDO!!!\n");
            MAX = 0;
            exit(0);
        }
        else if(*argv[1] >= 49 && *argv[1] <= 57)		// Valor do vetor precisa estar entre 49(1) e 57(9) da tabela ASCII para ser valido
        {
            MAX = atoi(argv[1]);                   /* Salva a conversão da entrada para inteiros em MAX */                            
    /*    }
        else	// Se o valor do argumento não esta entre 49(1) e 57(9)
        {
            printf("O ARGUMENTO <%c> EH INVALIDO!!!\n", *argv[1]);
            MAX = 0;
            exit(0);
        }   
    }
    return MAX;
}   */

/******************************** Função para inserir no buffer ***************************************
void insere_impressao(int id_arquivo){
    if(cont < MAX && t1->inicio == NULL){               /* Verifica se buffer não encheu e se fila esta vazia */
   /*     arquivos[final] = id_arquivo;                   /* Adiciona arquivo no buffer */
   /*     final = (final + 1) % MAX;
        cont = cont + 1;
        printf("Arquivo[%d] colocado no buffer de impressão\n", id_arquivo);
        if(cont == MAX)
            printf("\n############## Buffer completo ##############\n");
    }else{                                  /* Se Buffer estver cheio, manda para a fila */
   /*     insere(t1, id_arquivo);
        printf("Arquivo[%d] inserido em fila de impressão\n", id_arquivo);
    }
}/*


/******************************** Simulação do arquivo impresso ***************************************/
void *setor(){
    int setor_solicita; 
    int id_thread;
    sem_wait(&id_mutex);            /* região crítica pela variável global "arquivo" */
    id_thread = arquivo;            /* identificador da thread */
    arquivo++;
    sem_post(&id_mutex);

    setor_solicita = rand() % MAX;              /* setor gera um arquivo para impressão */
    if(dep[setor_solicita].quantidade < QTD){
        printf("<!> Setor %d solicita impressão do arquivo[%d]\n",  setor_solicita, id_thread);
        dep[setor_solicita].quantidade++;
        while(1){
            printf("Arquivo[%d] pronto para impressao.\n", id_thread);

            sem_wait(&mutex);               /* entrada na seção crítica em funcao de operacoes no buffer */
            insere_impressao(id_thread);      /* insere o arquivo no buffer de impressão ou na fila */
            sem_post(&mutex);              /* saida da secao critica */

            sem_post(&full);
            sem_wait(&s[id_thread]);        /* bloqueia o processo até sua impressão */
            printf("Arquivo[%d] foi impresso com sucesso!\n", id_thread);
            sleep(rand()%4 + 5); //atraso na geração e atualização de novos arquivos. De 5 a 8 segundos
        }
    }
    else
    {
        printf("<!> Setor já atingiu sua quantidade de solicitações de impreção!\n");

    }
    
}

// Metodo que realiza a retirada do dado do buffer
int imprime_item(){
    int id_arquivo_impresso;
    id_arquivo_impresso = arquivos[inicio];                 /* Pega primeiro item do Buffer para retirar */
    inicio = (inicio + 1) % MAX;
    cont = cont - 1;
    printf("Arquivo[%d] retirado do Buffer de impressão.\n", id_arquivo_impresso);
    return id_arquivo_impresso;
}

//quando um item eh removido, essa funcao eh chamada para retirar um arquivo das filas e por ele no buffer
void realoca(){
    int aux;
    if(t1->inicio != NULL){             // Verifica se fila nao esta vazia.
        aux = remov(t1); //remove o item da fila
        insere_impressao(aux); //insere no buffer
    }
}

//Thread que simula a impressora
void *imprime(){
    int id_arquivo_impresso;
    sleep(rand()% 5 + 3); //tempo inicial de impressao de 3 a 7 segundos
    while(1){
        sem_wait(&full); //bloqueia se buffer vazio
        sem_wait(&mutex); //entrada na secao critica por manipulacoes no buffer e fila
        id_arquivo_impresso = imprime_item();
        if(t1->inicio != NULL ){//verifica se as filas nao estao vazias
            realoca();
        }
        sem_post(&mutex);
        sem_post(&s[id_arquivo_impresso]); // desbloqueia o arquivo que foi impresso
        sleep(rand()% 3 + 2); //tempo gasto na impressao: de 2 a 4 segundos
    }
}

int main(int argc, char* argv[]){

    int entrada;                    /* Variável para dizer se entrada é ou não valida */

    entrada = get_inputs(argc, argv);            /* Chama função que verifica entrada */

    if (entrada == 0)
    {
        return 0;                   /* Se entrada não for válida, termina o programa */
    }
    else
    {
        
        int i;
        pthread_t thread[N], imp;               /* Cria as threads para setores e as threads para impressão */
    
        //Seta a semente da funcao geradora de numeros aleatorios
        srand(time(NULL));

        //inicialização do buffer como vazio
        inicio = 0;
        final = 0;
        cont = 0;

        arquivo = 0;

        //criacao e inicializacao da fila de impressão
        t1 = cria();

        //inicialização dos semaforos
        sem_init(&id_mutex, 0, 1);
        sem_init(&mutex, 0 , 1);
        sem_init(&full, 0, 0);
        for(i = 0; i < entrada; i++){
            sem_init(&s[i], 0, 0);
        }

        // criacao da thread de impressao
        if(pthread_create(&imp, 0, imprime, NULL)){
    		printf("Erro na criacao de Threads\n");
    	    exit(-1);
        }
        
        // Inicializa estrutura dos setores
        for(i = 0; i < N; i++){
            dep[i].id = 0;
            dep[i].quantidade = 0;
        }

        //criacao dos setores
        for(i = 0; i < N; i++){
            if(pthread_create(&thread[i], 0, setor, NULL)){
        	    printf("Erro na criacao de Threads\n");
        	    exit(-1);
            }
            sleep(rand()%5);            //tempo entre a geração de impressão para os departamentos
        }
        pthread_exit(NULL);
        exit(0);

    }




}







