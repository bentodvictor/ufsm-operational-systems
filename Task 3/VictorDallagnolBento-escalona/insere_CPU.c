#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sistema.h"

// FUNÇÃO INSERIR CPU

void insere_CPU(){
	fl.CPUStatus = 1;								// CPU esta ocupada
	temp2->timeExec--;								// Decrementa fatia de tempo do processo
	temp2->solicitaIO--;							// Decrementa Solicitação do I/O
	fl.auxiliar++;									// Auxiliar é comparado com fatia RR
	fl.TempoSimulado++;						
	fl.timeCPUUsada++;

	// Atualiza situação do processo
	strcpy(temp2->ciclo, "RUNNING");
	printf("{%d} P[%d] -> %s\n", fl.TempoSimulado, temp2->ID, temp2->ciclo);
	fprintf (trace, "{%d} P[%d] -> %s\n", fl.TempoSimulado, temp2->ID, temp2->ciclo);

	// Caso processo permaneça na CPU, flags são zeradas
	fl.B = 0;
	fl.C = 0;
	fl.A = 0;

//fluxo B Volta pra ready
	if(fl.auxiliar == entrada.FatiaTempo && temp2->timeExec > 0){			// Se tempo da fatia acabou mas o tempo do processo não 
		fl.B = 1;
		fl.C = 0;
		fl.A = 0;

//fluxo A Sai do sistema
	}else if(temp2->timeExec == 0){									// Se o tempo de execução acabou
		fl.B = 0;
		fl.C = 0;
		fl.A = 1;
		
//fluxo C Faz IO
	}else if(temp2->solicitaIO == 0){							// Tempo de solicitação do I/O atingido
		fl.B = 0;
		fl.C = 1;
		fl.A = 0;
	}
}