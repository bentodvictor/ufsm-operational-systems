#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "sistema.h"

Filas* cria_io(){
    Filas* f = (Filas*) malloc(sizeof(Filas));      	// alocação dinamica
    if(f != NULL){
        f->fila_io = NULL;
    }
    return f;
}

int tamanho_io(Filas* f){
    if(f == NULL)
        return 0;           // não tenho lista
    int cont = 0;
    Fila_io* no = f->fila_io;          // auxiliar recebe primeiro nó da lista, seu conteudo é o primeiro elemento
    while(no != NULL){          // enquanto não chegar no final da lista
        cont++;                 // conta mais um até chgar no final da fila
        no = no-> prox;     
    }
    return cont;
}

int io_cheia(Fila_io* li){
    return 0;
}

int io_vazia(Filas* f){
    if(f->fila_io == NULL)
        return 1;                   // Lista vazia, não tenho lista
    return 0;
}


int insere_io(Filas* f, Processo process){
	Fila_io *no = f->fila_io;
	Fila_io* ant = NULL;
	
    process.waitingTime_io_saida = 0;                           // sera tempo de saida de espera do Processo na fila I/O
    process.waitingTime_io_chegada = fl.TempoSimulado;          // em um certo tempo simulado, Processo chega na fila I/O 
	while(no != NULL && no->p.IOPrior <= process.IOPrior){
        ant = no;
		no = no->prox;
	}
	
	Fila_io* novo = (Fila_io*) malloc(sizeof(Fila_io));
	    novo->p = process;
	strcpy(process.ciclo, "BLOCKED-FILA(I/O)");                 // Atualiza situação do Processo
	printf("{%d} P[%d] -> %s\n", fl.TempoSimulado, process.ID, process.ciclo);
    fprintf (trace, "{%d} P[%d] -> %s\n", fl.TempoSimulado, process.ID, process.ciclo);
	
	if(ant == NULL){ //insere no inicio
		novo->prox = no;
		f->fila_io = novo;
	}
	else{ //insere meio ou fim
		novo->prox = ant->prox;
		ant->prox = novo;
	}
		
	return 1;
}

Processo remove_io(Filas* f){
    Fila_io* no = f->fila_io;               
	Processo aux = no->p;					// Cria um processo auxiliar que recebe dados do processo que foi removido
	no = no->ant;
    f->fila_io = f->fila_io->prox;
	return aux;             // retorna processo
}


void print_io(Filas* f){
    Fila_io* t;
    printf("(I/O) : ");
    for(t = f->fila_io; t != NULL; t = t->prox){
		printf("%d ", t->p.ID);
    }
    printf("\n");
}