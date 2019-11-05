#define T 16				// Escolhe o tempo randômico que o processo precisa para executar
#define IO 4				// Escolher a prioridade randômica para I/O
#define SIO 3				// Tempo para solicitar I/O
#define AIO 10				// Tempo para atendimento I/O

// Estrutura para entradas passadas pelo .txt
typedef struct entradas{
	int TempoSimulacao;
	int ML;
	int FatiaTempo;
}Entradas;

// Estrutura para as variáveis e constantes globais
typedef struct flags{
	int ProcessoCriado;				// Processos criados
	int TamanhoArrive;				// Tamanho da fila ARRIVE
	int ProcessosCompletos;			// Processo que já foram completos
	int ProcessosNoSistema;			// Processos que estão no sistema
	int TempoSimulado;				// Tempo simulado, comparado com entrada
	int auxiliar; 					// Controla o tempo na CPU
	int IO_completa;				// Quantas operação de I/O foram completas 
	int CPUStatus;					// Verifica se CPU está sendo usada ou não
	int IOStatus;					// Verifica se I/O está usado ou não
	int j;							// Valor da ID de cada processo, começa em 0 e sempre aumenta
	int i;							// Indice da fila estática
	int A, B, C;					// Flags para setar qual fluxo apartir da CPU o processo deve seguir 
	int fazerCPU;					// Flag para determinar se o Processo pode ou não usar a CPU
	int fazerIO;					// Flag para determinar se o Processo pode ou não usar o dispositivo I/O
	int removerChegada;				// Flag que indica que um processo pode ser removido da lista de CHEGADA
	int inserirReady;				// Flag que indica que um processo precisa ser inserido na lista de PRONTOS
	int removerIO;					// Flag para saber se devemos ou não remover Processo da fila I/O
	float timeCPUUsada;				// Tempo da cpu gasta na execução de processos
	float timeCPUOciosa;			// Tempo que CPU fica ociosa
	float timeIO;					// Tempo total gasto em operações de I/O
	float somaTimeExecProcess;		// Variavel para somar o tempo de execução de cada processo;	
	float Time_arrive;				// Tempo de espera da fila ARRIVE
	float Time_ready;				// Tempo de espera da fila READY
	float Time_io;					// Tempo de espera da fila READY
	float PReady;					// Processos inseridos na READY
	float PIO;						// Processos inseridos na I/O
	float TArrive;					// Tamanho da fila ARRIVE
	float TReady;					// Tamanho da fila READY
	float TIO;						// Tamanho da fila I/O
}Flags;

// Estrutura para Processos
typedef struct process{
    int ID;     		            // ID do processo
    int timeExec;           		// Tempo de execução (aleatório)
    int IOPrior;            		// Prioridade do IO (aleatório)
	int solicitaIO;					// Tempo que o processo vai solicitar IO
	int atendimentoIO;				// Tempo de atendiamento para I/O
	char ciclo[20]; 				// new, ready, running, blocked and terminatted
	int turnaroundEntrada;			// tempo de turnaround
	int turnaroundSaida;			// tempo de turnaround
	int waitingTime_arrive;			// tempo de espera por processo
	int waitingTime_io;				// tempo de espera por processo
	int waitingTime_ready;			// tempo de espera por processo
	int waitingTime_arrive_chegada;			// tempo de espera por processo
	int waitingTime_io_chegada;				// tempo de espera por processo
	int waitingTime_ready_chegada;			// tempo de espera por processo
	int waitingTime_arrive_saida;			// tempo de espera por processo
	int waitingTime_io_saida;				// tempo de espera por processo
	int waitingTime_ready_saida;			// tempo de espera por processo
}Processo;

// Estrutura para fila de CHEGADA
typedef struct fila_arrive{
	Processo p;
	struct fila_arrive *prox;
}Fila_arrive;

// Estrutura para fila de PRONTOS
typedef struct fila_ready{
	Processo p;
	struct fila_ready *prox;
}Fila_ready;

// Estrutura para fila de I/O
typedef struct fila_io{						
	Processo p;							// Fila I/O vai ter um tamanho fixo 
	struct fila_io *prox, *ant;			
}Fila_io;

// Estrutura para as Filas
typedef struct filas{
	Fila_arrive *inicio, *fim;		// ponteiro para inicio e final da fila ARRIVE
	Fila_ready *begin, *end;		// Instancia da fila READY
	Fila_io *fila_io;				// Instancia de ponteiro para fila I/O
	int tamanho;
}Filas;

//Atribuições 
Entradas entrada;				// Intancia de ENTRADAS para ter acesso a elas em todo código
Flags fl;						// Instancia de FLAGS para ter acesso a elas em todo código
Processo *temp1;					// Ponteiro para processos entre troca de fila 
Processo *temp2;					// Ponteiro para processos que estão na CPU
Processo *temp3;					// Ponteiro para processos ques estão no I/O



// Funções
void PegaEntradas();
void CriarProcessos(Filas* fila);
void confere_CPU(Filas* r);
void insere_CPU();
void organizaFilas(Filas* c, Filas* p);			
void fluxos(Filas* p, Filas* i);
void confere_IO(Filas *i);
void insere_IO(Filas* i, Filas* r);


//  Filas ready - estática
Filas* cria_ready(void);
void libera_ready(Filas* f);
int tamanho_ready(Filas* f);
int ready_cheia(Filas* f);
int ready_vazia(Filas* f);
int insere_ready(Filas* f, Processo process);
Processo remove_ready(Filas* f);
void print_ready(Filas* f);

// Filas arrive	- dinâmica
Filas* cria_arrive(void);
void libera_arrive(Filas* f);
int tamanho_arrive(Filas* f);
int arrive_cheia(Filas* f);
int arrive_vazia(Filas* f);
int insere_arrive(Filas* f, Processo process);
Processo remove_arrive(Filas* f);
void print_arrive(Filas* f);


// Filas IO	- dinâmica duplamente encadeada
Filas* cria_io();
int insere_io(Filas* f, Processo process);
Processo remove_io(Filas* f);
void print_io(Filas* f);
int io_vazia(Filas* f);
int tamanho_io(Filas* f);

// Criação dos arquivos
FILE *outputs;				// Arquivo para as saídas
FILE *trace;				// Arquivo para o trace de execução