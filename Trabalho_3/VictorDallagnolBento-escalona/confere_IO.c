#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sistema.h"

// FUNÇÃO PARA CONFERIR I/O

void confere_IO(Filas *i){							// Recebe fila I/O para saber se esta vazia
    if (fl.IOStatus == 0 && io_vazia(i) == 1)					// I/O Desocupado e Fila vazia
	{
		fl.removerIO = 0;				// Não remove da fila I/O
		fl.fazerIO = 0;					// Não faz I/O
	}
	else if (fl.IOStatus == 0 && io_vazia(i) == 0)		        // I/O desocupado e a fila não está vazia
	{
		fl.removerIO = 1;               // Processo precisa ser removido da fila de I/O 
		fl.fazerIO = 1;					// Faz I/O
    }
	else							// I/O está ocupado
	{
		fl.removerIO = 0;           // Não remove da fila
		fl.fazerIO = 1;				// Continua fazendo I/O
	}
}