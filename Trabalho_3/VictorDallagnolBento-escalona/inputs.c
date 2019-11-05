#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sistema.h"

void PegaEntradas(){
    char* inputs[50];           // Recebe entradas do arquivo
    char line[50];              // Linha do arquivo 
    int i = 0;                  // Contador

    FILE *file;                             // Criação do poteiro que faz a ligação com o arquivo.
    file = fopen("inputs.txt", "r");        // Abre o arquivo e pega as entradas 
    
    if (file == NULL)                       // Erro ao ler arquivo.
    {
        printf ("EXIT_FAILURE");
    }  
    
    while(fgets(line, sizeof line, file) != NULL)       // Equanto arquivo não for nulo.
    {
        inputs[i] = strdup(line);                       // Salva valores em um vetor auxiliar.
        i++;
    }
    entrada.TempoSimulacao = atoi(inputs[0]);
    entrada.ML = atoi(inputs[1]);
    entrada.FatiaTempo = atoi(inputs[2]);                                 // Define o tamanho máximo da fila READY e I/O
    printf("\t\t\t===========================================\n");
    printf("\t\t\t| Tempo total da simulacao: %d             \n\t\t\t| Grau de Multiprogramação: %d             \n\t\t\t| Fatia de tempo para escalonamento RR: %d \n", entrada.TempoSimulacao, entrada.ML, entrada.FatiaTempo);
    printf("\t\t\t===========================================\n\n");
}