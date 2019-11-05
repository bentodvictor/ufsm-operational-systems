#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define N 5             /* Quantidade de Setores/Departamentos */
#define QTD 3           /* Quantidade total de solicitações de impressão para cada Setor/Departamento */
#define X 3             /*  T1 é X vezes mais rápida que T2     */

sem_t mutex_set;       /* semaforo binario para exclusão mutua dos setores */
sem_t mutex_imp;       /* semaforo binario para exclusão mutua das impressoras  */
sem_t mutex_buffer;    /* semaforo binario para exclusão no acesso ao vetor de estruturas  */

int MAX;                /* Para criar os tipos de impressora */

/* Arquivo para impressão */
int arquivo;

/* Estrutura para os prints */
typedef struct printImpressoesDep{
    int impressoes;                 // numero de impressoes de cada setor
}PrintImpressoesDep;
PrintImpressoesDep pib[N];

/* Estrutura para armazenar a ID e a quantidade dde impressoras utilizadas  */
typedef struct impressoras{
    int ID;
    int SolicOK;
    int SolicERRO;
}Impressoras;

/* Estrutura para armazenar o departamento e a quantidade de solicitações já feitas */
typedef struct departamentos{
    int ID;
    int quantidade;
    int TotalSolic;
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
    while(dpt->quantidade < QTD)                        /* Verifica se quantidade de solicitações já foi atendida   */ 
    {
        dpt->quantidade++;                              /*  Incrementa quantidade de solicitações daquele setor, feito em cima para não começar em 0  */
        printf("Setor[%d] - realizando solicitacao %d...\n", dpt->ID, dpt->quantidade);
        
        sem_wait(&mutex_set);                           /* Entra na região crítica */
        sol[count].id_solicitacao = arquivo;            /* salva o arquivo na estrutura Solicitações    */
        sol[count].id_setor = dpt->ID;                  /*   salva ID do departamento na estrutura Solicitações    */
        arquivo++;                                      /* Arquivo é incrementado   */
        count++;                                        /* Contador para salvar dados na Estrutura Solicitações    */
        dpt->TotalSolic++;                              /* Total de solicitações do Departamento é incrementado */
        if(count > N*QTD && sol[0].atendido == 1){      /* Quando buffer chega no seu limite, volta para o inicio */
            count = 0;
        }
        sem_post(&mutex_set);             /* Saída da região crítica */
        
        printf("Setor[%d] - solicita impressão do arquivo #%d.\n",  dpt->ID, sol[count-1].id_solicitacao);
        sleep(4 + rand()%10);
    }
 /*   for(count2 = 0; count2 < count; ++count2){
        printf("\tSolicitações[%d] = arquivo #%d, setor #%d, atendido #%d\n", count2, sol[count2].id_solicitacao, sol[count2].id_setor, sol[count2].atendido);
    }*/
    if(dpt->quantidade >= QTD)              /*  Quando quantidade de solicitações já foi atendida   */
    {
        dpt->quantidade++;
        printf("<!> Setor[%d] já atingiu sua quantidade de solicitações de impressão!\n", dpt->ID);
    } 
}   



/*  CONSUMIDOR ~ simula impressora T1   */
void *simulaPrintT1(void *imp)
{       
    Impressoras* print = (Impressoras*)imp;         /* instancia a impressora   */
    int indice;                                     /* Variavel para salvar o índice */
    while(1){
        sem_wait(&mutex_buffer);                    /* entrada na secao critica  */
        if(count2 >= count){                        /* Condição de saída    */
            sem_post(&mutex_buffer);
            break;
        }
        indice = count2;                            /* Indice recebe contador antes do mesmo ser incrementado   */
        count2++;                           
        sem_post(&mutex_buffer);
        
        sem_wait(&mutex_imp);                       /* entrada na secao critica  */
        if(sol[indice].atendido == 0){              /* Se algum arquivo ainda não foi impresso  */
            if(rand()%99 < 14){                     /* Calculo para os 15% de erros     */
                printf("<!> ERRO NA IMPRESSÃO - IMPRESSORA T1\n");
                sol[indice].atendido = 0;           /* arquivo n foi atendido   */
                print->SolicERRO++;                 /* Solicitação com ERRO */
            }else{
                sol[indice].id_impressora = print->ID;      /* Salva ID da impressora na Estrutura  */
                sol[indice].atendido = 1;                   /* Arquivo foi impresso     */
                pib[indice].impressoes++;                   /* Numero de impressões do processo[indice] é incrementado  */
                print->SolicOK++;                           /*  Solicitação sem ERRO*/
            }                        
        }
        sem_post(&mutex_imp);
        printf("#%d sendo impresso pela impressora %d(T1)\n",  sol[indice].id_solicitacao, sol[indice].id_impressora);    
        sleep(rand()%3);
        printf("#%d impresso com sucesso pela impressora %d(T1).\n", sol[indice].id_solicitacao, sol[indice].id_impressora);                
    }        
}



void *simulaPrintT2(void *imp)
{   
    Impressoras* print = (Impressoras*)imp;         /*  Instancia da impressora */
    int indice;
    while(1){
        sem_wait(&mutex_buffer);                /* entrada na secao critica  */
        if(count2 >= count){                    /* Condição de saída    */
            sem_post(&mutex_buffer);
            break;
        }
        indice = count2;            /* indice recebe contador   */
        count2++;               
        sem_post(&mutex_buffer);

        sem_wait(&mutex_imp);
        if(sol[indice].atendido == 0){                  /* se algum arquivo anda não foi impresso   */
            sol[indice].id_impressora = print->ID;      /* Salva o indide da impressora no vetor */
            sol[indice].atendido = 1;                   /* Arquivo vai ser atendido */
            pib[indice].impressoes++;                   /* Numero de impressões do processo[indice] é incrementado  */
            print->SolicOK++;
        }
        sem_post(&mutex_imp);
        printf("#%d sendo impresso pela impressora %d(T2)\n", sol[indice].id_solicitacao, sol[indice].id_impressora); 
        sleep(rand()%3*X);                      /* Impressora T2 é X vezes mais lenta que T1    */
        printf("#%d impresso com sucesso pela impressora %d(T2).\n", sol[indice].id_solicitacao, sol[indice].id_impressora);
    }
}



/*  Principal   */
int main(int argc, char* argv[])
{    
    MAX = get_inputs(argc, argv);           /*  Função para pegar entrada   */
    
    printf("@@@  Numero de Setores: %d.\n", N);
    printf("@@@  Quantidade de soicitações por Setor: %d.\n", QTD);
    printf("@@@  T1 é %dx mais rápida que T2.\n", X);
    printf("@@@  Numero de Impressoras: %d.\n\n", MAX);
    
    /* Alimenta semente para aleatoriedade  */
    srand(time(NULL));

    int i;                  /* Contador */
    pthread_t setor[N], t1[MAX/2], t2[MAX/2];          /*  Criação das threads de setores e de impressoras */

    /*  Inicialização das variáveis   */
    Impressoras imp[MAX];
    Departamentos dep[N];
    arquivo = 0;
    count = 0;
    count2 = 0;

    /*  inicialização dos semaforos */
    sem_init(&mutex_set, 0, 1);
    sem_init(&mutex_imp, 0 , 1);
    sem_init(&mutex_buffer, 0 , 1);         /* não sei se precisa ser usado */

    /*  Inicializa todos os processos como se ainda não foram atendidos.    */
    for(i = 0; i < N*QTD; ++i){
        sol[i].atendido = 0;
    }

    /* Zera solicitações das impressoras  */
    for(i = 0; i < MAX; ++i){
        imp[i].SolicOK = 0;
        imp[i].SolicERRO = 0;
    }

    /*  Thread para impressoras T1 e T2    */
    for(i = 0; i < MAX/2; i++){
        imp[i].ID = i;                  // Seta ID das impressoras;
        pthread_create(&t1[i], 0, simulaPrintT1, (void*)&imp[i]);
    }
    for(i = MAX/2; i < MAX; ++i){
        imp[i].ID = i;                  // Seta ID das impressoras;
        pthread_create(&t2[i], 0, simulaPrintT2, (void*)&imp[i]); 
    }

    /*  Thread para os Setores */
    for(i = 0; i < N; i++){ 
        dep[i].ID = i;                  // Atribui ID para departamento
        dep[i].quantidade = 0;	        // Zera quantidade de solicitações de cada departamento
        pib[i].impressoes = 0;
        dep[i].TotalSolic = 0;
        pthread_create(&setor[i], 0, simulaDocPrint, (void*)&dep[i]);
    }


    /*  JOIN's */
    for(i = 0; i < N; i++){ 
        pthread_join(setor[i], NULL);
    }
    for(i = 0; i < MAX/2; i++){
        pthread_join(t1[i], NULL);
    }    
    for(i = MAX/2; i < MAX; i++){
        pthread_join(t2[i], NULL);
    }
    

    /* Print Final  */
    printf("\n\n=======================================\n");
    for(int i = 0; i <N; i++){
        printf("Setor[%d] - Impressoes[%d] - Solicitacoes[%d]\n", dep[i].ID, pib[i].impressoes, dep[i].TotalSolic);
    }
    printf("\n\n=======================================\n");
    for(int i = 0; i <MAX/2; i++){
        printf("Impressora T1[%d] - Sucesso[%d] - Falha[%d]\n", imp[i].ID, imp[i].SolicOK, imp[i].SolicERRO);
    }
    printf("\n\n=======================================\n");
    for(int i = MAX/2; i <MAX; i++){
        printf("Impressora T2[%d] - Sucesso[%d] - Falha[%d]\n", imp[i].ID, imp[i].SolicOK, imp[i].SolicERRO);
    }
    exit(0);
}