#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sistema.h"

// FUNÇÃO INSERIR I/O

void insere_IO(Filas* i, Filas* r){			           // alocação dinamica do Temporario3
    if(fl.CPUStatus == -1){
        fl.timeCPUOciosa++;
    }
    switch(fl.fazerIO)                      // Confere a flag para fazer ou não I/O
    {
    case 1:
        if(fl.removerIO == 1){              // Se tiver que remover da fila do I/O
            temp3 = (Processo*) malloc(sizeof(Processo));
            *temp3 = remove_io(i);          // Remove processo da fila I/O e salva no temp3
            temp3->waitingTime_io_saida = fl.TempoSimulado;         // Salva tempo de saiva da fila de I/O do Processo
            temp3->waitingTime_io += temp3->waitingTime_io_saida - temp3->waitingTime_io_chegada;       // Faz calculo do tempo de espera do Processo na fila I/O
            fl.Time_io += temp3->waitingTime_io;        // Atribui tempo de espera da fila I/O
            fl.IOStatus = 1;                // I/O esta ocupado
            fl.TempoSimulado++;
            strcpy(temp3->ciclo, "BLOCKED-DISP(I/O)");                  // Atualiza situação do processo
	        printf("{%d} P[%d] -> %s\n", fl.TempoSimulado, temp3->ID, temp3->ciclo);
            fprintf (trace, "{%d} P[%d] -> %s\n", fl.TempoSimulado, temp3->ID, temp3->ciclo);   // salva no arquivo
            temp3->atendimentoIO--;
            fl.timeIO++;
            if(temp3->atendimentoIO == 0){          // Se atendimento do I/O acabar
                insere_ready(r, *temp3);            // Insere temp3 na READY
                fl.TReady += tamanho_ready(r);      // Pega tamanho da READY
                fl.PReady++;                        // Mais um processo inserido na READY
                free(temp3);
                fl.IOStatus = 0;                    // I/O fica desocupado
                fl.IO_completa++;
            }
        }else{                          // Se não tiver que remover da fila do I/O
            fl.IOStatus = 1;                // I/O esta ocupado
            fl.TempoSimulado++;
            temp3->atendimentoIO--;
            fl.timeIO++;
            strcpy(temp3->ciclo, "BLOCKED-DISP(I/O)");              // Atualiza situação do processo
	        printf("{%d} P[%d] -> %s\n", fl.TempoSimulado, temp3->ID, temp3->ciclo);
            fprintf (trace, "{%d} P[%d] -> %s\n", fl.TempoSimulado, temp3->ID, temp3->ciclo);
            if(temp3->atendimentoIO == 0){          // Se atendimento do I/O acabar
                insere_ready(r, *temp3);            // Insere temp3 na READY
                fl.TReady += tamanho_ready(r);      // Pega o tamanho da READY
                fl.PReady++;                        // Mais um processo inserido na READY
                free(temp3);
                fl.IOStatus = 0;                    // I/O fica desocupado
                fl.IO_completa++;
            }
        }
        break;
    case 0:
        break;
    }
}