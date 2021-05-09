#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int testa_entrada(int argc, char* argv[]);      // Declaração da função que testa se as entradas do terminal (filho por nível) são válidas.
    pid_t idPai;	            // Processo pai
    int cont;                   // Contador para o for
    int n[argc];                // Vetor inteiro para receber conversão de argv
    int nivel = 1;              // Nivel da arvore de processos
    int estado;                 // Estado para função wait
    clock_t start_t, end_t;     // Variáveis para contagem do clock.
    double total_t;             // Variável para mostrar o tempo em segundos.
    int entrada;                // Testa se entrada é valida.

    entrada = testa_entrada(argc, argv);            // Invoca função para testar se entradas são validas

    if(entrada == 1)
    {
        for (cont = 0; cont < argc; cont++)         // Conversão dos caracteres passados atraves do terminal
        {
            n[cont] = atoi(argv[cont]);
        }

        start_t = clock();                              // Contagem do clock inicial.

        for(cont = 1; cont <= n[nivel]; cont++)         // Cotador começa em 1 por que o primeiro elemento do vetor é o próprio programa.
        {
            idPai = fork();                             // Chamada da função fork

            switch (idPai)                              // Avalia o retorno da função fork()
            {
                case -1:                                // Erro no FORK.
                    exit (1);

                case 0:                     // FILHO.
                    cont = 0;		            // Contador é zerado para o filho recomeçar o for, com sua devida parada (n[nivel])
                    if (nivel < argc-1)     // condição para testar se o nivel máximo da arvore já foi atingido
                    {
                        nivel++;            // Nivel da arvore de processos é incrementado para executar o filho
                    }
                    else
                    {
                       cont = n[nivel];    // Caso nivel máximo for atingido, cont recebe o valor de n[nivel] e encerra o laço
                    }
                    break;

                default:                // PAI
                    printf("PAI: %d , FILHO: %d\n", getpid(), idPai);
                    wait(&estado);      // Pai espara filhos terminarem primeiro para evitar processos orfãos.
                    break;
           }
    	    if (cont == n[nivel])                                     // Condição para testar se o numero de filhos já foi atingido
    	    {
    	    	printf("PROCESSO %d FINALIZADO!\n", getpid());        // Quando número de filhos é atingido, processo é terminado.
    	    }

    	    if (cont == n[1])                    // Verifica se o numero de filhos do primeiro processo (PAI) já foi atingido
    	    {
                        end_t = clock();        // Contagem do clock final
                        total_t = (double)(end_t - start_t);
                        // Divisão  para mostrar tempo em segundos [CLOCK/(CLOCK/S)]
			if(total_t > 0)		// Como somente o PAI tem tempo positivo, essa condição pega apenas o tempo final do pai
			{
                       		printf("\nTempo total usado pela CPU: %f SEGUNDOS...\n", ((total_t)/CLOCKS_PER_SEC));
			}
            }
        }   // Fim do laço FOR
        exit(0);
    }
    else                // Caso entrada não seja valida
    {
        exit(0);
    }                   // Fim do IF que testa as entradas válidas
}




//  Função para tratamento de entradas inválidas.
int testa_entrada(int argc, char* argv[])
{
    int ehValido = 1;       // Variavel para validar ou não a entrada
    int cont;               // Contador para o for

    if (argc < 2)           //  Condição caso nenhum argumento seja passado
    {
        printf("NENHUM ARGUMENTO ENCONTRADO!!!\n");
        ehValido = 0;
    }
    else
    {
        for(cont = 1; cont < argc; ++cont)
        {
            if(*argv[cont] == '0')       // Se o valor do vetor na posição cont for o caractere '0'
            {
                ehValido = 0;
                printf("O ARGUMENTO <0> EH INVALIDO!!!\n");
                exit(0);
            }
            else if(*argv[cont] >= 49 && *argv[cont] <= 57)		// Valor do vetor precisa estar entre 49(1) e 57(9) da tabela ASCII para ser valido
            {
                ehValido = 1;
            }
            else	// Se o valor do argumento não esta entre 49(1) e 57(9)
            {
                ehValido = 0;
                printf("O ARGUMENTO <%c> EH INVALIDO!!!\n", *argv[cont]);
                exit(0);
            }
        }
    }
    return ehValido;
}
