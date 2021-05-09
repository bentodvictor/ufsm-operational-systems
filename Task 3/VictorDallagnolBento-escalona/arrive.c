#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sistema.h"

Filas* cria_arrive(void){								// cria fila dinamicamente alocada
	Filas* f = (Filas*) malloc(sizeof(Filas));      	// alocação dinamica
	if (f != NULL)										// se a alocação não der erro
		f->inicio = f->fim = NULL;
	return f;											// retorna a fila
}

void libera_arrive(Filas* f){							// para liberar a fila
	if (f != NULL){
		Fila_arrive* no	;						      // cria uma nó auxiliar para receber o inicio
		while(f->inicio != NULL){
			no = f->inicio;							// nó recebe inicio
			f->inicio = f->inicio->prox;			// inicio aponta para próximo
			free (no);								// libera nó, que era o antigo inicio, elemento é liberado
		}
	}
}

int tamanho_arrive(Filas* f){
	if(f == NULL)						// condição para fila inexistente
		return 0;
	int cont = 0;						// contador para tamanho
	Fila_arrive* no = f->inicio;		// cria nó inicial para percorrer fila
	while(no != NULL){					// enquanto nó não chegar no fim, percorre fila e incrementa contador
		cont++;
		no = no->prox;		// anda para o proximo elemento
	}
	return cont;
}


int arrive_cheia(Filas* f){
	return 0;					// fila nunca esta cheia
}

int arrive_vazia(Filas* f){
	if(f == NULL)
		return 1;			// se fila não foi criada é vazia
	if(f->inicio == NULL)	// se inicio for nulo fila é vazia
		return 1;
	return 0;				// fila não esta vazia
}

int insere_arrive(Filas* f, Processo process){
	if(f == NULL)			// verifica problema na criação da fila
		return 0;
	Fila_arrive* no = (Fila_arrive*)malloc(sizeof(Fila_arrive));	// aloca memoria para um novo elemento da fila
	if(no == NULL)		// verifica se alocação deu certo
		return 0;
	no->p = process;	// Dados do process é copiado para o campo de processos da fila
	no->p.ID = fl.j;								// ID do processo recebe a flag J
	no->p.timeExec = 1 + rand() % T;                // Atribui tempo de execução aleatório ao processo, no mínimo 1
  no->p.IOPrior =  1 + rand() % IO;               // Atribui prioridade aleatória ao processo, no míniimo 1
	no->p.solicitaIO =  1 + rand() % SIO;            // Atribui tempo que o processo vai solicitar I/O
	no->p.atendimentoIO =  3 + rand() % AIO;         // Atribui tempo de atendimento ao para a funçao I/O
	no->p.turnaroundEntrada = fl.TempoSimulado;					// Pega o tempo em que processo é criado
	no->p.waitingTime_arrive_chegada = fl.TempoSimulado;		//	Pega tempo simulado de cada processo para calcular tempo de espera na fila
	no->p.waitingTime_ready = 0;								// Zera variáveis que receberão o totao de espera por fila
	no->p.waitingTime_io = 0;
	no->p.waitingTime_arrive = 0;

	strcpy(no->p.ciclo, "NEW");							// Nova situação para o processo
	printf("{%d} P[%d] -> %s\n", fl.TempoSimulado, no->p.ID, no->p.ciclo);
	fprintf (trace, "{%d} P[%d] -> %s\n", fl.TempoSimulado, no->p.ID, no->p.ciclo);
	no->prox = NULL;
	if(f->fim == NULL)	// insere no final da fila
		f->inicio = no;
	else
		f->fim->prox = no;
	f->fim = no;
	fl.j++;									// Incremente ID
	return 1;
}

Processo remove_arrive(Filas* f){
		Fila_arrive* no = f->inicio;
	Processo aux = no->p;					// Cria um processo auxiliar que recebe dados do processo que foi removido
	f->inicio = f->inicio->prox;
	if(f->inicio == NULL)
		f->fim = NULL;
	return aux;						// retornar o processo ou só sua ID????
}

void print_arrive(Filas* f){
	Fila_arrive* fa;
	printf("(CHEGADA) : ");
	for(fa = f->inicio; fa != NULL; fa = fa->prox)
		printf("%d ", fa->p.ID);					// Printa a identidade do processo
		printf("\n");
}
