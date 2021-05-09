#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sistema.h"

void CriarProcessos(Filas* fila){
	if(fl.CPUStatus == -1){
        fl.timeCPUOciosa++;
  }
	while(fl.TamanhoArrive < entrada.ML){      			// Define um tamanho máximo para fila ARRIVE
		Processo teste; 								// Cria um processo NULL
		if (insere_arrive(fila, teste)){				// Insere e seta os valores do processo na ARRIVE
			fl.ProcessoCriado++;						// Nº de processos criados aumenta
		}else{
			break;										// Limite máximo da fila atingido
		}
		fl.TamanhoArrive++;								// Aumenta o tamanho da rive por processo criado
		fl.TArrive += tamanho_arrive(fila);
		}
}
