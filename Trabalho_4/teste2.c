#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#include "queue.h"

#define N 5             /* Quantidade de Setores/Departamentos */
#define QTD 3           /* Quantidade total de solicitações de impressão para cada Setor/Departamento */
#define PBSIZE 5        /* Tamanho do BUFFER de impressão */
#define X 3             /*  T1 é X vezes mais rápida que T2     */

sem_t  id_mutex;        /* semaforo binario para as threads criarem seu id */
sem_t  mutex;           /* semaforo binario utilizado p/ garantir exclusao mutua na regiao critica  */
sem_t  full;            /* semaforo utilizado pra controlar os sinais relativos as posicoes preenchidas no buffer */
sem_t  s[N];            /* semaforo para controlar a impressao dos arquivos do buffer */


int MAX;                /* Para criar os tipos de impressora */

/* Declaração da fila das impressoras */
Fila* filaImpressao;
/* Declaração do BUFFER e variáveis de controle para o mesmo */
int buffer[PBSIZE];
int inicio, final, cont;

/* Arquivo para impressão */
int arquivo;

/* Estrutura para armazenar a ID e a quantidade dde impressoras utilizadas  */
typedef struct impressoras{
    int ID;
    int quantidade;
}Impressoras;
Impressoras imp;

/* Estrutura para armazenar o departamento e a quantidade de solicitações já feitas */
typedef struct departamentos{
    int ID;
    int quantidade;
}Departamentos;
Departamentos dep;

/* Funções */
void insere_buffer(int x, int prioridade);
void realoca_buffer();
int print_item();

/*  Função para inserir o arquivo do departamento no BUFFER de impressão
    Caso o BUFFER estiver cheio, inserene fila da impressora específica */
void insere_buffer(int x, int prioridade){
        if(cont < PBSIZE && filaImpressao->inicio == NULL || prioridade == 1)    /* Verifica se BUFFER não encheu, fila vazia, e prioriade */
        {
            buffer[final] = x;            /* Adiciona arquivo no BUFFER   */
            final = (final + 1) % PBSIZE;
            cont++;
            printf("#%d no buffer de impressão.\n", x);
            if(cont == PBSIZE && prioridade != 1)
                printf("\n~~~~~~~~~~ Buffer completo ~~~~~~~~~~\n");
        }else{                                  /* Se Buffer estver cheio, manda para a fila */
            insere(filaImpressao, x);
            printf("#%d no fila de impressão.\n", x);
        }
}

/*  Função para simular o arquivo impresso  */
void *simulaDocPrint(){
    int id_arquivo;
    int indiceDep;
    int id_impressora;
    
    sem_wait(&id_mutex);
    id_arquivo = arquivo;
    arquivo++;
    id_impressora = impressora;
    impressora++;
    sem_post(&id_mutex);

    indiceDep = rand() % N;
    dep[indiceDep].ID = indiceDep; 


    if(dep[indiceDep].quantidade < QTD){
        printf("<!> Setor[%d] - #%d solicita impressão.\n",  dep[indiceDep].ID, id_arquivo);
        dep[indiceDep].quantidade++;
        while(1){
            printf("#%d pronto para impressao.\n", id_arquivo);
            
            sem_wait(&mutex);               /* entrada na seção crítica em funcao de operacoes no buffer */
            insere_buffer(id_arquivo, 0);      /* insere o arquivo no buffer de impressão ou na fila */
            sem_post(&mutex);              /* saida da secao critica */

            sem_post(&full);
            sem_wait(&s[id_arquivo]);        /* bloqueia o processo até sua impressão */
            
            sleep(rand()%4 + 5); //atraso na geração e atualização de novos arquivos. De 5 a 8 segundos
        }
    }
    else
    {
        printf("<!> Setor já atingiu sua quantidade de solicitações de impreção!\n");
    }
    
} 

/*  Função para remover arquivo do BUFFER e colocar em uma das FILAS   */
int print_item(){
    int id_arquivo_impresso;
    id_arquivo_impresso = buffer[inicio];
    inicio = (inicio + 1) % PBSIZE;
    cont--;
    printf("#%d retirado do Buffer de impressão.\n", id_arquivo_impresso);
    return id_arquivo_impresso;
}

/* Quando item for removio do BUFFER, esta função retira da fila e coloca no BUFFER */
void realoca_buffer(){
    int aux;
    
    if(filaImpressao->inicio != NULL){               /*  Vreifica se fila esta vazia */
        aux = remov(filaImpressao);                  /* Remove arquivo da fila */
        insere_buffer(aux, 1);                 
    }
}

/*  Thread que simula a impressora T1 */
void *simulaPrintT1(){
    int id_arquivo_impresso;

 //   sleep(rand()% 5);
    while(1){
        sem_wait(&full);            /*  bloqueia se buffer vazio    */
        sem_wait(&mutex);           /* entrada na secao critica por manipulacoes no buffer e fila */
            id_arquivo_impresso = print_item();
        if(filaImpressao->inicio != NULL){           /* Se fila não estive vazia realoca BUFFER      */
            realoca_buffer();
        }
        sem_post(&mutex);
        sem_post(&s[id_arquivo_impresso]); // desbloqueia o arquivo que foi impresso
        sleep(rand()% 3 + 2); //tempo gasto na impressao: de 2 a 4 segundos
        printf("#%d impresso com sucesso pela impressora %d.\n\n", id_arquivo_impresso, impressora);
    }
}


/*  Thread que simula a impressora T2 */
void *simulaPrintT2(){
    int id_arquivo_impresso;

//    sleep(rand()% 5*X);                         /* Impressora T2 é X vezes mais lenta que T1        */
    while(1){
        sem_wait(&full);                        /*  bloqueia se buffer vazio    */
        sem_wait(&mutex);                       /* entrada na secao critica por manipulacoes no buffer e fila  */
            id_arquivo_impresso = print_item();
        if(filaImpressao->inicio != NULL){          /* verifica se a fila está vazia    */
            realoca_buffer();
        }
        sem_post(&mutex);
        sem_post(&s[id_arquivo_impresso]);      /* desbloqueia o arquivo que foi impresso   */
        sleep(rand()% 3 + 2);                   /* tempo gasto na impressao: de 2 a 4 segundos */
        printf("#%d impresso com sucesso pela impressora %d.\n\n", id_arquivo_impresso, impressora);
    }
}


int main(int argc, char* argv[]){
    
    MAX = get_inputs(argc, argv);
    
    int i;
    pthread_t thread[N], t1[MAX/2], t2[MAX/2];
    
    //Seta a semente da funcao geradora de numeros aleatorios
    srand(time(NULL));

    //inicialização do buffer como vazio
    inicio = 0;
    final = 0;
    cont = 0;

    arquivo = 0;
    impressora = 0;

    //inicializacao das filas como vazias
    filaImpressao = cria();

    //inicialização dos semaforos
    sem_init(&id_mutex, 0, 1);
    sem_init(&mutex, 0 , 1);
    sem_init(&full, 0, 0);
    for(i = 0; i < N; i++){
        sem_init(&s[i], 0, 0);
    }

    /*  Thread para impressoras T1    */
    for(i = 0; i < MAX/2; i++){
        if(pthread_create(&t1[i], 0, simulaPrintT1, NULL)){
    		printf("Erro na criacao de Threads\n");
    	    exit(-1);
        }
    } 
    
    /*  Thread para impressoras T2   */
    for(i = 0; i < MAX/2; i++){
        if(pthread_create(&t2[i], 0, simulaPrintT2, NULL)){
    		printf("Erro na criacao de Threads\n");
    	    exit(-1);
        }
    } 

    /*  criação dos Setores */
    for(i = 0; i < N; i++){
        if(pthread_create(&thread[i], 0, simulaDocPrint, NULL)){
        	printf("Erro na criacao de Threads\n");
        	exit(-1);
        }
    }
    pthread_exit(NULL);
    exit(0);
}
