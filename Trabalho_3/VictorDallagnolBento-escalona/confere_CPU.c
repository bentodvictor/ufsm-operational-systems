#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sistema.h"

// FUNÇÃO PARA CONFERIR CPU

void confere_CPU(Filas* r){							// Recebe fila READY para saber se esta vazia

	if (fl.CPUStatus == -1)				// CPU ociosa
	{
		fl.removerChegada = 1;	// Processo deve sair da fila de chegada
		fl.inserirReady = 1;		// Processo removido da fila chegada deve ir p fila de prontos
		fl.fazerCPU = 1;				// Primeiro processo da fila de prontos deve ir para CPU
	}
	else								// CPU ocupada
	{
		fl.removerChegada = 1;			// Processo deve sair da fila de chegada
		fl.inserirReady = 1;			// Processo removido da fila chegada deve ir p fila de prontos
		fl.fazerCPU = 0;				// flag em 0, indica que o processo nao pode acessar a cpu
	}
}
