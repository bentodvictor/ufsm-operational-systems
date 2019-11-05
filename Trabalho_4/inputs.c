#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

/************************************* Função para tratamento de entradas inválidas.*******************************/
int get_inputs(int argc, char* argv[])
{   
    int X;
    if (argc < 2)           //  Condição caso nenhum argumento seja passado
    {
        printf("NENHUM ARGUMENTO ENCONTRADO!!!\n");
        X = 0;
        exit(0);
    }
    else
    {
        if(*argv[1] == '0')       // Se o valor do vetor na posição cont for o caractere '0'
        {
            printf("O ARGUMENTO <0> EH INVALIDO!!!\n");
            X = 0;
            exit(0);
        }
        else if(*argv[1] >= 49 && *argv[1] <= 57)		// Valor do vetor precisa estar entre 49(1) e 57(9) da tabela ASCII para ser valido
        {
            X = atoi(argv[1]);                   /* Salva a conversão da entrada para inteiros em MAX */                            
        }
        else	// Se o valor do argumento não esta entre 49(1) e 57(9)
        {
            printf("O ARGUMENTO <%c> EH INVALIDO!!!\n", *argv[1]);
            X = 0;
            exit(0);
        }   
    }
    return X;
}
