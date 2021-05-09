#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define N 10             /* Quantidade de Setores/Departamentos */
#define QTD 3           /* Quantidade total de solicitações de impressão para cada Setor/Departamento */
#define PBSIZE 5        /* Tamanho do BUFFER de impressão */
#define X 3             /*  T1 é X vezes mais rápida que T2     */

sem_t mutex_set;       /* semaforo binario para exclusão mutua dos setores */
sem_t mutex_imp;       /* semaforo binario para exclusãi mutua das impressoras  */

int MAX;                /* Para criar os tipos de impressora */

/* Arquivo para impressão */
int arquivo;

/* Estrutura para armazenar a ID e a quantidade dde impressoras utilizadas  */
typedef struct impressoras{
    int ID;
}Impressoras;

/* Estrutura para armazenar o departamento e a quantidade de solicitações já feitas */
typedef struct departamentos{
    int ID;
    int quantidade;
}Departamentos;

/* Estrutura para solicitaçẽs   */
typedef struct solicitacoes{
    int atendido;
    int id_setor;
    int id_solicitacao;
    int id_impressora;
}Solicitacoes;
Solicitacoes sol[N*QTD];

/* Contador para acessar e percorrer solicitações   */
int count;
int count2;

/*  Funções */
int get_inputs(int argc, char* argv[]);
void *simulaDocPrint(void *dep);
void *simulaPrintT2(void *imp);
void *simulaPrintT2(void *imp);


/*  PRODUTOR ~ Função para simular os arquivos impressos  */
void *simulaDocPrint(void *dep){
    
   
    Departamentos* dpt = (Departamentos*)dep;           /* Instancia de Departamento    */
 
    while(dpt->quantidade < QTD)           /* Verifica se quantidade de solicitações já foi atendida   */ 
    {
        printf("Setor[%d] - produzindo arquivo %d..\n", dpt->ID, arquivo);
        sem_wait(&mutex_set);             /* Entra na região crítica */
        sol[count].id_solicitacao = arquivo;       /* salva o arquivo na estrutura Solicitações    */
        sol[count].id_setor = dpt->ID;             /*   salva ID di departamento na estrutura Solicitações    */
        arquivo++;                       /* Arquivo é incrementado   */
        count++;                         /* Contador para salvar dados na Estrutura Solicitações    */
        if(count > 30){                 /* Quando buffer chega no seu limite, volta para o inicio */
            count = 0;
        }
        dpt->quantidade++;               /* Incrementa quantidade de solicitações daquele setor  */
        sem_post(&mutex_set);             /* Saída da região crítica */
        printf("Setor[%d] - solicita impressão do arquivo #%d.\n",  dpt->ID, sol[count-1].id_solicitacao);
        sleep(2 + rand()%10);
    }
    if(dpt->quantidade >= QTD)              /*  Quando quantidade de solicitações já foi atendida   */
    {
        dpt->quantidade++;
        printf("<!> Setor[%d] já atingiu sua quantidade de solicitações de impreção!\n", dpt->ID);
    }
}   


/*  CONSUMIDOR ~ simula impressora T1   */
void *simulaPrintT1(void *imp)
{       
    Impressoras* print = (Impressoras*)imp;         /* instancia a impressora   */
    sem_wait(&mutex_imp);           /* entrada na secao critica  */
    for(count2 = 0; count2 < count; ++count2){          /* percorre todo buffer */
        if(sol[count2].atendido == 0){                  /* se algum arquivo anda não foi impresso   */
            
                sol[count2].id_impressora = rand() % MAX;
                sol[count2].atendido = 1;
            sem_post(&mutex_imp);
        }
            printf("#%d sendo impresso pela impressora %d(T1)\n",  sol[count2].id_solicitacao, sol[count2].id_impressora);
    }    
        sleep(rand()%5);
        printf("#%d impresso com sucesso pela impressora %d(T1).\n\n", sol[count2].id_solicitacao, sol[count2].id_impressora);
}


/*  CONSUMIDOR ~ simula impressora T2   */
void *simulaPrintT2(void *imp)
{   
    Impressoras* print = (Impressoras*)imp;         /*  Instancia da impressora */
    sem_wait(&mutex_imp);           /* entrada na secao critica  */
    for(count2 = 0; count2 < count; ++count2){              /* precorre todo buffer */
        if(sol[count2].atendido == 0){                  /* Se algum arquivo ainda não foi impresso  */
            if(rand()%100 <= 15){                   /* Calculo para os 15% de erros     */
                
                sol[count2].id_impressora = rand() % MAX;
                sol[count2].atendido = 1;
                sem_post(&mutex_imp);
            }else{
                printf("<!> ERRO NA IMPRESSÃO - IMPRESSORA T2\n");
            }
        }
            printf("#%d sendo impresso pela impressora %d(T2)\n", sol[count2].id_solicitacao, sol[count2].id_impressora); 
    }    
        sleep(rand()%5 + X);                      /* Impressora T2 é X vezes mais lenta que T1    */
        printf("#%d impresso com sucesso pela impressora %d(T2).\n\n", sol[count2].id_solicitacao, sol[count2].id_impressora);
}

/*  Principal   */
int main(int argc, char* argv[])
{    
    MAX = get_inputs(argc, argv);           /*  Função para pegar entrada   */
    
    /* Alimenta semente para aleatoriedade  */
    srand(time(NULL));

    int i;                  /* Contador */
    int distPrint;          /* Divide max entre impressoras T1 e T2 */
    pthread_t setor[N], t1[MAX], t2[MAX];          /*  Criação das threads de setores e de impressoras */

    /*  Inicialização dos arquivos, da quantidade de arquivos solicitadas pelo
        departamento e a instancia das estruturas das impessoras e setores    */
    Impressoras *imp;
    Departamentos dep[N];
    arquivo = 0;
    count = 0;
    count2 = 0;

    /*  inicialização dos semaforos */
    sem_init(&mutex_set, 0, 1);
    sem_init(&mutex_imp, 0 , 1);

    /*  Inicializa todos os processos como se ainda não foram atendidos.    */
    for(i = 0; i < N*QTD; ++i)
        sol[i].atendido = 0;

    /*  Thread para os Setores */
    for(i = 0; i < N; i++){ 
        dep[i].ID = i;
        dep[i].quantidade = 0;	
        pthread_create(&setor[i], 0, simulaDocPrint, (void*)&dep[i]);
    }

    /*  Thread para impressoras T1zz    */
    for(i = 0; i < MAX; i++){
        imp = malloc(sizeof(Impressoras));
        distPrint = rand() % 2 + 1;
        if(distPrint == 1){
            pthread_create(&t1[i], 0, simulaPrintT1, (void*)imp);
        }else{
            pthread_create(&t2[i], 0, simulaPrintT2, (void*)imp);
        }
    } 


    /*  JOIN */
    for(i = 0; i < N; i++){ 
        pthread_join(setor[i], NULL);
    }
    for(i = 0; i < MAX; i++){
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
    }
    exit(0);
}