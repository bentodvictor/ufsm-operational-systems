#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sistema.h"

void organizaFilas(Filas* c, Filas* p){                          
	if(fl.CPUStatus == -1){
        fl.timeCPUOciosa++;
    }
    temp1 = (Processo*) malloc(sizeof(Processo));			     // alocação do processo

    if(fl.removerChegada == 1 && fl.inserirReady == 1 && fl.fazerCPU == 0)          // CPU esta ocupada e processo precisa ser removido da ARRIVE para READY
    {
        if (fl.ProcessosNoSistema < entrada.ML)                                     // Verifica se numero de processos no sistema foi atingido 
        {
            *temp1 = remove_arrive(c);           // temporário recebe processo removido da fila de ARRIVE 
            fl.TamanhoArrive--;
            fl.TempoSimulado++;
            temp1->waitingTime_arrive_saida = fl.TempoSimulado;                 // Tempo de saida da fila do Processo é atualizada
            temp1->waitingTime_arrive += temp1->waitingTime_arrive_saida - temp1->waitingTime_arrive_chegada;      // Tempo de espera do processo na fila ARRIVE é atualizado
            fl.Time_arrive += temp1->waitingTime_arrive;        // Tempo de espera da fila ARRIVE é atualizado
            insere_ready(p, *temp1);            // temporario é inserido na fila READY
            fl.TReady += tamanho_ready(p);      // Pega tamanho da READY
            fl.PReady++;                        // Mais um processo na READY
            fl.ProcessosNoSistema++;             // Manutenção do grau de multiprogramação (ML)
           }
        print_ready(p);
    }
    else           // CPU desocupada
    {
        if (fl.ProcessosNoSistema < entrada.ML)         // Verifica se numero de processos no sistema foi atingido 
        {  
            *temp1 = remove_arrive(c);                  // temporario recebe processo removido da ARRIVE
            fl.TamanhoArrive--;
            fl.ProcessosNoSistema++;                    // Manutenção do grau de multiprogramação (ML)
            fl.TempoSimulado++;
            temp1->waitingTime_arrive_saida = fl.TempoSimulado;                 // Tempo de saida da fila do Processo é atualizada
            temp1->waitingTime_arrive += temp1->waitingTime_arrive_saida - temp1->waitingTime_arrive_chegada;      // Tempo de espera do processo na fila ARRIVE é atualizado
            fl.Time_arrive += temp1->waitingTime_arrive;        // Tempo de espera da fila ARRIVE é atualizado
            insere_ready(p, *temp1);                            // temporario é inserido na READY
            fl.TReady += tamanho_ready(p);      // Pega tamanho da READY
            fl.PReady++;                        // Mais um processo na READY
        }
            temp2 = (Processo*) malloc(sizeof(Processo));			// alocação dinamica do temporário2
            print_ready(p);                             
            *temp2 = remove_ready(p);                               // Temporario2 Recebe o processo retirado da READY pois o mesmo ira para CPU
            temp2->waitingTime_ready_saida = fl.TempoSimulado;      // Tempo de saida da fila do Processo é atualizada
            temp2->waitingTime_ready += temp2->waitingTime_ready_saida - temp2->waitingTime_ready_chegada;      // Tempo de espera do processo na fila READY é atualizado
            fl.Time_ready =+ temp2->waitingTime_ready;        // Tempo de espera da fila READY é atualizado
            fl.auxiliar = 0;
        }
}
