#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "sistema.h"


int main(int arg, char* argv[])
{
	fl.TempoSimulado = 0;						// variável para controlar o tempo de simulação
	fl.TamanhoArrive = 0;						// variável para controlar o tamanho da fila de chegada
	fl.j = 0;									// define a ID dos processos
	fl.CPUStatus = -1;							// CPU Ociosa
	fl.ProcessosNoSistema = 0;					// Nenhum processo no sistema
	fl.IOStatus = 0;							// I/O ocioso
	fl.IO_completa = 0;
	fl.timeCPUUsada = 0;
	fl.timeIO = 0;
	fl.somaTimeExecProcess = 0;
	fl.PReady = 0;
	fl.PIO = 0;
	fl.TArrive = 0;
	fl.TReady = 0;
	fl.TIO = 0;

	PegaEntradas();								    // chama função para pegar entradas

	Filas* chegada = cria_arrive();						// cria fila de CHEGADA
	Filas* prontos = cria_ready();						// cria fila de PRONTOS
	Filas* io = cria_io();								// cria fila de I/O


	outputs = fopen("outputs.txt", "wt");					// Criação do Arquivo para saídas
	if (outputs == NULL)
	{
	    printf("Problemas na CRIACAO do arquivo\n");
	    return 0;
	}

	trace = fopen("trace.txt", "wt");					// Criação do Arquivo para Trade de execução dos Processos
	if (trace == NULL)
	{
	    printf("Problemas na CRIACAO do arquivo\n");
	    return 0;
	}

	while (fl.TempoSimulado < entrada.TempoSimulacao)			// while para simulação
	{
		CriarProcessos(chegada);								// Cria processo na ARRIVE
		if(fl.TempoSimulado == entrada.TempoSimulacao)			// Testa se tempo de simulação já foi atingido
			break;
		print_arrive(chegada);
		if(fl.TempoSimulado == entrada.TempoSimulacao)
			break;
		confere_CPU(prontos);									// Seta as flags para saber situação atual da CPU
		if(fl.TempoSimulado == entrada.TempoSimulacao)
			break;
		organizaFilas(chegada, prontos);						// Organiza as filas conforme a situação da CPU
		if(fl.TempoSimulado == entrada.TempoSimulacao)
			break;
		insere_CPU();											// Coloca/Mantém processos na CPU
		if(fl.TempoSimulado == entrada.TempoSimulacao)
			break;
		fluxos(prontos, io);									// Função para os fluxo que o processo pode seguir
		if(fl.TempoSimulado == entrada.TempoSimulacao)
			break;
		confere_IO(io);											// Seta as flags para saber a situação atual do I/O
		if(fl.TempoSimulado == entrada.TempoSimulacao)
			break;
		insere_IO(io, prontos);									// Coloca processos da fila de I/O para o dispositivo I/O
		if(fl.TempoSimulado == entrada.TempoSimulacao)
			break;
	}
	// Prints para ver saidas
	printf("\nPROCESSOS TERMINADOS: %d\n", fl.ProcessosCompletos);
	printf("\nOPERAÇÕES DE I/O COMPLETAS: %d\n", fl.IO_completa);
	printf("\nTEMPO DE CPU USADA: %.2f, %.2f(PORCENTO)\n", fl.timeCPUUsada, 100*(fl.timeCPUUsada/(fl.timeCPUUsada+fl.timeCPUOciosa)));
	printf("\nTEMPO DE CPU OCIOSA: %.2f, %.2f(PORCENTO)\n", fl.timeCPUOciosa, 100*(fl.timeCPUOciosa/(fl.timeCPUUsada+fl.timeCPUOciosa)));
	printf("\nTEMPO DE I/O USADO: %.2f, %.2f(PORCENTO)\n", fl.timeIO, 100*(float)(fl.timeIO/(fl.TempoSimulado)));
	printf("\nTEMPO MÉDIO DE ESPERA ENTRE PROCESSOS: %.2f\n", (fl.somaTimeExecProcess/fl.ProcessosCompletos));
	printf("\nTEMPO MÉDIO NA FILA ARRIVE: %.2f\n", (fl.Time_arrive/fl.ProcessoCriado));
	printf("\nTEMPO MÉDIO NA FILA READY: %.2f\n", (fl.Time_ready/fl.PReady));
	printf("\nTEMPO MÉDIO NA FILA IO: %.2f\n", (fl.Time_io/fl.PIO));
	printf("\nTAMANHO MÉDIO NA FILA ARRIVE: %.2f\n", (fl.TArrive/fl.ProcessoCriado));
	printf("\nTAMANHO MÉDIO NA FILA READY: %.2f\n", (fl.TReady/fl.PReady));
	printf("\nTAMANHO MÉDIO NA FILA IO: %.2f\n", (fl.TIO/fl.PIO));

	// Atribuição das saídas para arquivo
	fprintf (outputs, "PROCESSOS TERMINADOS: %d\n", fl.ProcessosCompletos);
	fprintf (outputs, "OPERAÇÕES DE I/O COMPLETAS: %d\n", fl.IO_completa);
	fprintf (outputs, "TEMPO DE CPU USADA: %.2f, %.2f(PORCENTO)\n", fl.timeCPUUsada, 100*(fl.timeCPUUsada/(fl.timeCPUUsada+fl.timeCPUOciosa)));
	fprintf (outputs, "TEMPO DE CPU OCIOSA: %.2f, %.2f(PORCENTO)\n", fl.timeCPUOciosa, 100*(fl.timeCPUOciosa/(fl.timeCPUUsada+fl.timeCPUOciosa)));
	fprintf (outputs, "TEMPO DE I/O USADO: %.2f, %.2f(PORCENTO)\n", fl.timeIO, 100*(float)(fl.timeIO/(fl.TempoSimulado)));
	fprintf (outputs, "TEMPO MÉDIO DE ESPERA ENTRE PROCESSOS: %.2f\n", (fl.somaTimeExecProcess/fl.ProcessosCompletos));
	fprintf (outputs, "TEMPO MÉDIO NA FILA ARRIVE: %.2f\n", (fl.Time_arrive/fl.ProcessoCriado));
	fprintf (outputs, "TEMPO MÉDIO NA FILA READY: %.2f\n", (fl.Time_ready/fl.PReady));
	fprintf (outputs, "TEMPO MÉDIO NA FILA IO: %.2f\n", (fl.Time_io/fl.PIO));
	fprintf (outputs, "TAMANHO MÉDIO NA FILA ARRIVE: %.2f\n", (fl.TArrive/fl.ProcessoCriado));
	fprintf (outputs, "TAMANHO MÉDIO NA FILA READY: %.2f\n", (fl.TReady/fl.PReady));
	fprintf (outputs, "TAMANHO MÉDIO NA FILA IO: %.2f\n", (fl.TIO/fl.PIO));

	fclose(outputs);
	fclose(trace);

	return 0;
}
