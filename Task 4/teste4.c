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

int MAX;                /* Para criar os tipos de impressora */

/* Arquivo para impressão */
int arquivo;

/* Print final  */
int TotalSolic;
int SolicOK;
int SolicERRO;


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
    int impressoes;
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
        dpt->quantidade++;               /* Incrementa quantidade de solicitações daquele setor, feito em cima para não começar em 0  */
        printf("Setor[%d] - realizando solicitacao %d...\n", dpt->ID, dpt->quantidade);
        
        sem_wait(&mutex_set);             /* Entra na região crítica */
        sol[count].id_solicitacao = arquivo;       /* salva o arquivo na estrutura Solicitações    */
        sol[count].id_setor = dpt->ID;             /*   salva ID do departamento na estrutura Solicitações    */
        arquivo++;                       /* Arquivo é incrementado   */
        count++;                         /* Contador para salvar dados na Estrutura Solicitações    */
        TotalSolic++;
        if(count > N*QTD && sol[0].atendido == 1){                 /* Quando buffer chega no seu limite, volta para o inicio */
            count = 0;
        }
        sem_post(&mutex_set);             /* Saída da região crítica */
        
        printf("Setor[%d] - solicita impressão do arquivo #%d.\n",  dpt->ID, sol[count-1].id_solicitacao);
        sleep(8 + rand()%10);
    }
    //for(count2 = 0; count2 < count; ++count2){
        //printf("\tSolicitações[%d] = arquivo #%d, setor #%d, atendido #%d\n", count2, sol[count2].id_solicitacao, sol[count2].id_setor, sol[count2].atendido);
    //}
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
    int indice;             /* Variavel para salvar o índice */
    while(1){
        for(count2 = 0; count2 < count; ++count2){          /* percorre todo buffer */
    sem_wait(&mutex_imp);                      /* entrada na secao critica  */
            if(sol[count2].atendido == 0){                  /* Se algum arquivo ainda não foi impresso  */
                if(rand()%99 < 14){                   /* Calculo para os 15% de erros     */
                    printf("<!> ERRO NA IMPRESSÃO - IMPRESSORA T1\n");
                    sol[count2].atendido = 0;   
                    SolicERRO++;

                }else{
                    sol[count2].id_impressora = print->ID;
                    sol[count2].atendido = 1;
                    sol[count2].impressoes++;
                    indice = count2;
                    SolicOK++;
                }

    sem_post(&mutex_imp);
    printf("#%d sendo impresso pela impressora %d(T1)\n",  sol[indice].id_solicitacao, sol[indice].id_impressora);    
    sleep(rand()%3);
    printf("#%d impresso com sucesso pela impressora %d(T1).\n\n", sol[indice].id_solicitacao, sol[indice].id_impressora);
                break;
            }
        }

    
    }
}





/*  CONSUMIDOR ~ simula impressora T2   */
void *simulaPrintT2(void *imp)
{   
    Impressoras* print = (Impressoras*)imp;         /*  Instancia da impressora */
    int indice;
    while(1){
        for(count2 = 0; count2 < count; ++count2){              /* precorre todo buffer */
    sem_wait(&mutex_imp);           /* entrada na secao critica  */
            if(sol[count2].atendido == 0){                  /* se algum arquivo anda não foi impresso   */
                sol[count2].id_impressora = print->ID;          /* Salva o indide da impressora no vetor */
                sol[count2].atendido = 1;           /* Arquivo vai ser atendido */
                sol[count2].impressoes++;
                indice = count2;            /* Salva o indice do arquivo que será impresso na variavel "indice " */
                SolicOK++;
                
            sem_post(&mutex_imp);
            printf("#%d sendo impresso pela impressora %d(T2)\n", sol[indice].id_solicitacao, sol[indice].id_impressora); 
            sleep(rand()%3*X);                      /* Impressora T2 é X vezes mais lenta que T1    */
            printf("#%d impresso com sucesso pela impressora %d(T2).\n\n", sol[indice].id_solicitacao, sol[indice].id_impressora);
                break;                      /* Sai do laço ao encontrar elemento que precisa ser atendido */
            }
        }

    }
}








/*  Principal   */
int main(int argc, char* argv[])
{    
    MAX = get_inputs(argc, argv);           /*  Função para pegar entrada   */
    
    printf("\t@@@  Numero de Setores: %d.\n", N);
    printf("\t@@@  Quantidade de soicitações por Setor: %d.\n", QTD);
    printf("\t@@@  T1 é %dx mais rápida que T2.\n", X);
    printf("\t@@@  Numero de Impressoras: %d.\n\n", MAX);
    
    /* Alimenta semente para aleatoriedade  */
    srand(time(NULL));

    int i;                  /* Contador */
    int distPrint;          /* Divide max entre impressoras T1 e T2 */
    pthread_t setor[N], t1[MAX], t2[MAX];          /*  Criação das threads de setores e de impressoras */

    /*  Inicialização dos arquivos, da quantidade de arquivos solicitadas pelo
        departamento e a instancia das estruturas das impessoras e setores    */
    Impressoras imp[MAX];
    Departamentos dep[N];
    arquivo = 0;
    count = 0;
    count2 = 0;
    TotalSolic = 0;
    SolicOK = 0;
    SolicERRO = 0;

    /*  inicialização dos semaforos */
    sem_init(&mutex_set, 0, 1);
    sem_init(&mutex_imp, 0 , 1);

    /*  Inicializa todos os processos como se ainda não foram atendidos.    */
    for(i = 0; i < N*QTD; ++i){
        sol[i].atendido = 0;
        sol[i].impressoes = 0;
    }

    /*  Thread para os Setores */
    for(i = 0; i < N; i++){ 
        dep[i].ID = i;                  // Atribui ID para departamento
        dep[i].quantidade = 0;	        // Zera quantidade de solicitações de cada departamento
        pthread_create(&setor[i], 0, simulaDocPrint, (void*)&dep[i]);
    }

    /*  Thread para impressoras T1 e T2    */
    for(i = 0; i < MAX; i++){
        imp[i].ID = i;                  // Seta ID das impressoras;
        distPrint = rand() % 2 + 1;         // Aleatoriedade para mandar para a impressora T1 ou T2
        if(distPrint == 1){
            pthread_create(&t1[i], 0, simulaPrintT1, (void*)&imp[i]);
        }else{
            pthread_create(&t2[i], 0, simulaPrintT2, (void*)&imp[i]);
        }
    } 

    /*  JOIN's */
    for(i = 0; i < N; i++){ 
        pthread_join(setor[i], NULL);
    }
    for(i = 0; i < MAX; i++){
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
    }
    exit(0);
}