#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sistema.h"

// FUNÇÃO FLUXO A, B e C

void fluxos(Filas* p, Filas* i){

	if (fl.A)
	{
		fl.TempoSimulado++;
		strcpy(temp2->ciclo, "TERMINATED");					// Atualiza situação do processo
		printf("{%d} P[%d] -> %s\n", fl.TempoSimulado, temp2->ID, temp2->ciclo);			
		fprintf (trace, "{%d} P[%d] -> %s\n", fl.TempoSimulado, temp2->ID, temp2->ciclo);	// salva no arquivo
		temp2->turnaroundSaida = fl.TempoSimulado;					// Salva tempo em que processo terminou 
		printf("*** TEMPO DE ESPERA DO PROCESSO[%d] = %d\n", temp2->ID, (temp2->waitingTime_arrive + temp2->waitingTime_ready + temp2->waitingTime_io));
		printf("*** PROCESSO: %d - TURNAROUND: %d\n", temp2->ID, temp2->turnaroundSaida - temp2->turnaroundEntrada);
		fprintf (outputs, "TEMPO DE ESPERA DO PROCESSO[%d] = %d\n", temp2->ID, (temp2->waitingTime_arrive + temp2->waitingTime_ready + temp2->waitingTime_io));
		fprintf (outputs, "PROCESSO: %d - TURNAROUND: %d\n", temp2->ID, temp2->turnaroundSaida - temp2->turnaroundEntrada);
		fl.somaTimeExecProcess += (temp2->waitingTime_arrive + temp2->waitingTime_ready + temp2->waitingTime_io);		//  Tempo total de espera do Processo
		free(temp2);						// Libera temporário
		fl.ProcessosCompletos++;
		fl.ProcessosNoSistema--;
		fl.CPUStatus = -1;						// CPU volta a ficar ociosa pois o processo acaba
		fl.timeCPUOciosa++;
		print_ready(p);
	}
	else if(fl.B)
	{
		fl.TempoSimulado++;
		insere_ready(p, *temp2);				// Temporario2 é inserido na fila READY
		fl.TReady += tamanho_ready(p);			// Pega tamanho da READY toda vez que um processo é inserido 
		fl.PReady++;							// Processo na READY aumenta
		print_ready(p);
		free(temp2);
		fl.CPUStatus = -1;					// CPU fica ociosa pois o processo vai para READY
		fl.timeCPUOciosa++;
	}
	else if (fl.C)
	{
		fl.TempoSimulado++;
		insere_io(i, *temp2);				// Temporario2 é inserido na fila do I/O
		fl.TIO += tamanho_io(i);			// Pega tamanho da I/O toda vez que um processo é inserido 
		fl.PIO++;							// Processo na I/O aumenta
		print_io(i);
		free(temp2);
		fl.CPUStatus = -1;					// CPU fica ociosa pois processo vai para fila do I/O
		fl.timeCPUOciosa++;
	}
}