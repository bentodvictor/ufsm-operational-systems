#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sistema.h"

Filas* cria_ready(void){								// cria fila dinamicamente alocada
	Filas* f = (Filas*) malloc(sizeof(Filas));      	// alocação dinamica
	if (f != NULL)										// se a alocação não der erro
		f->begin = f->end = NULL;
	return f;											// retorna a fila 
}

void libera_ready(Filas* f){							// para liberar a fila 
	if (f != NULL){
		Fila_ready* no	;						      // cria uma nó auxiliar para receber o begin
		while(f->begin != NULL){
			no = f->begin;							// nó recebe begin
			f->begin = f->begin->prox;			// begin aponta para próximo
			free (no);								// libera nó, que era o antigo begin, elemento é liberado
		}
	}
}

int tamanho_ready(Filas* f){
	if(f == NULL)						// condição para fila inexistente 
		return 0;
	int cont = 0;						// contador para tamanho
	Fila_ready* no = f->begin;		// cria nó inicial para percorrer fila
	while(no != NULL){					// enquanto nó não chegar no end, percorre fila e incrementa contador
		cont++;			
		no = no->prox;		// anda para o proximo elemento
	}
	return cont;
}


int ready_cheia(Filas* f){			
	return 0;					// fila nunca esta cheia
}

int ready_vazia(Filas* f){
	if(f == NULL)
		return 1;			// se fila não foi criada é vazia
	if(f->begin == NULL)	// se begin for nulo fila é vazia
		return 1;
	return 0;				// fila não esta vazia
}

int insere_ready(Filas* f, Processo process){
	if(f == NULL)			// verifica problema na criação da fila
		return 0;
	Fila_ready* no = (Fila_ready*)malloc(sizeof(Fila_ready));	// aloca memoria para um novo elemento da fila
	if(no == NULL)		// verifica se alocação deu certo
		return 0;
	no->p = process;	// Dados do process é copiado para o campo de processos da fila
	no->p.waitingTime_ready_chegada = fl.TempoSimulado;			// Tempo de chegada do Processo na fila READY é atualizado
	no->prox = NULL;
	strcpy(no->p.ciclo, "READY");				// Atualiza situação do processo
	printf("{%d} P[%d] -> %s\n", fl.TempoSimulado, no->p.ID, no->p.ciclo);
	fprintf (trace, "{%d} P[%d] -> %s\n", fl.TempoSimulado, no->p.ID, no->p.ciclo);		
	if(f->end == NULL)	// insere no final da fila
		f->begin = no;
	else
		f->end->prox = no;
	f->end = no;
	return 1;
}

Processo remove_ready(Filas* f){
	Fila_ready* no = f->begin;
	Processo aux = no->p;					// Cria um processo auxiliar que recebe dados do processo que foi removido
	f->begin = f->begin->prox;
	if(f->begin == NULL)
		f->end = NULL;
	//free(no);
	return aux;						// retornar o processo ou só sua ID????
}

void print_ready(Filas* f){
	Fila_ready* fa;
	printf("(PRONTOS) : ");
	for(fa = f->begin; fa != NULL; fa = fa->prox)
		printf("%d ", fa->p.ID);					// Printa a identidade do processo
		printf("\n");
}
