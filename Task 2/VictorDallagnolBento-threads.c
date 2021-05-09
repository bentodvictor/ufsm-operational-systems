/* ----------------------------------------------------------------------------------------------------------
   --------------------------- Trabalho Sistemas Operacionais - Threads--------------------------------------
   --------------------------------------Victor Dallagnol Bento----------------------------------------------
   ----------------------------------------------------------------------------------------------------------
   Compilar com a flag ... -pthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

typedef struct
{
   double somat;                                    // Soma do cálculo do PI para cada thread
   double limiteIn, limiteSu;                       // Limite superior e inerior de cada thread
} t_Args;

int main(int argc, char *argv[])
{
    int entrada;                                    // Variável para testar se entrada é valida.
    int i;                                          // Contador para for.
    double soma;                                    // Variável que recebe o numero passado em ARGV[1], a precisão do calculo
    double FuncaoA(double n);                       // Declaração da Função A - Sem Threads
    void FuncaoB(double n, int m);                  // Declaração da Função B - Com Threads
    int testa_entrada(int argc, char* argv[]);      // Declaração da função que testa se as entradas do terminal são válidas.
    double convf[2];                                // Vetor auxiliar para receber a conversão do que vem antes de "e" e depois de "e".
    double A_t;                                     // Variável para receber retorno da função A

    entrada = testa_entrada(argc, argv);            // Invoca função para testar se entradas são validas
    	if(entrada == 1)
    	{

        	char *c[2];                             // Função para pegar separar a entrada ARGV[1].
        	int j = 0;                              // Contador para separar entradas.

        	c[j] = strtok(argv[1], "e");            // Tokenize, separa o que vem antes de "e" e salva em c[0].

        	while(c[j] != NULL)                     // Percorre toda a String contida em ARGV[1].
        	{
        		++j;                                // Incrementa o contador para salvar no proximo endereço do vetor auxiliar.
        		c[j] = strtok(NULL, "e");           // Salva o que vem depois do caractere "e" em c[1].
        	}                                       // Fim da função.

            convf[0] = atof(c[0]);                  // Conversor do vetor auxiliar em ponto flutuante (float).
            convf[1] = atof(c[1]);
            soma = convf[0];                        // Soma recebe parte esquerda de "e".

            for(i = 0; i < convf[1]; ++i)           // For para multiplicação da entrada.
            {
                soma *= 10;                         // Soma é multiplicada por 10 até que o valor do expoente (convf[1]) seja atingido.
            }
        	A_t = FuncaoA(soma);                    // Chamada da função SEQUANCIAL, retorna o tempo gasto para efetuar o calculo.
        	printf("Tempo execucao calculo sequencial: %fs\n", A_t);        // Amostragem do tempo fasto para efetuar o calculo do PI.
          printf("---------------------------------------------------\n");
          FuncaoB(soma, atoi(argv[2]));           // Chama função B e passa como parametros a precisão e o numero de threads.
        }
        else
        {
        	exit(0);
        }
}


double FuncaoA(double n)	                        //	SEM THREADS.
{
	clock_t start_t, end_t;                         // Declaração dos tempos.
	double total_t;                                 // Declaração para pegar o tempo total.
	double equacao = 0;	            		        // Define equação.
	double i;					                    // Contador.

	start_t = clock();	        	               	// Começa contagem do CLOCK
	for(i = 0; i <= n; i++)                         // limitando é a soma, ARGV[1] convertido para flout e devidamente multiplicado
    {
        if ((int)i % 2 == 0)
        {
            equacao += 1/ (2*i + 1);               	// Se o contador for par sinal é positivo
        }
        else
        {
            equacao -= 1/ (2*i + 1);               	// Se o contador for impar sinal é negativo
        }
	}
    end_t = clock();                                    	        // Fim da contagem do CLOCK
	printf("\nSequencial -> PI = %f\n", 4 * equacao);               // Amostragem do calculo de PI * 4, como indica a formula
	total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;           // Conversão do CLOCK para segundos

return (total_t);                                    // Retorna o tempo total gasto para a execução do calculo.
}


void FuncaoB(double n, int m)       			    // COM THREADS
{
    clock_t start_t, end_t;                         // Variáveis para o tempo
    double CadaThread, total_t;                     // Variaveis para p limite de cada thread e o total do tempo gasto
    void *calcula(void *arg);                       // Declaração da função
    double SomaThreads = 0;                         // Variavel que vai receber a soma de todas os resultados das equqações das threads
    CadaThread = n / (double)m;                     // Parcela que cada Thread deve executar
    pthread_t tid[m];                               // Declaração do vetor de Threads
    int t;                                          // Contador
    double a[2*m];                                  // Vetor para armazenar o limite inferior e superior de cada Thread
    t_Args *total;                                  // Apontador par estrutura

    start_t = clock();                   		    // Começa contagem do CLOCK
    for(t = 0; t < m; t++)                          // Criação das Threads
    {
        total = malloc(sizeof(t_Args));             // Alocação de memória para cada thread
        if (total == NULL)
        {
            printf("erro ao alocar memoria\n");     // Erro na alocação de memória.
            exit(-1);
        }

        a[0] = 0;                                   // Iicialização dos limites para a primeira thread
        a[1] = CadaThread;
        total->limiteIn = a[t];                     // limite inferior de cada thread é colocado na estrutura
        total->limiteSu = a[t+1];                   // limite superior de cada thread é colocado na estrutura

        if (pthread_create(&tid[t], NULL, calcula, (void *)total))      // criação as threads
        {
            printf("erro ao criar thread\n");                           // erro na criação
            exit(-1);
        }
        a[t+2] = CadaThread + a[t+1];                 // Faz o somatório para o limite superior da proxima thread
    }
    end_t = clock();                                                 // Fim da contagem do CLOCK
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;            // Conversão do CLOCK para segundos
    for(t = 0; t < m; t++)                                           // For para garantir  o término das Threads
    {
        pthread_join(tid[t],(void *) &total);                       // Retorna o edereço da struct de cada thread
        SomaThreads += total->somat;                                // Soma os resultados obtidos nas equações das threads
    }

    printf("Somatório threads -> PI = %f\n", 4 * SomaThreads);            // Amostragem do resultado do cálculo do PI
    printf("Tempo execucao calculo com threads: %fs\n", total_t);        // Amostragem do tempo fasto para efetuar o calculo do PI.

}


void *calcula(void *arg)
{
    t_Args *args = (t_Args *) arg;                               // Variável ARGS vai ter o mesmo endereço de ARG, contendo o endereço da estrutura t_Args
    double equacao = 0;                                          // Variável para receber o resultado da equação

	while (args->limiteIn <= args->limiteSu)                     // Equanto o limite inferior for menor ou igual ao limite superior, executa cálculo
    {
        if ((int)args->limiteIn % 2 == 0)                        // Se o contador for par, sinal é positivo
        {
            equacao += 1/ (2*args->limiteIn + 1);                // Se o contador for par sinal é positivo
        }
        else
        {
            equacao -= 1/ (2*args->limiteIn + 1);                // Se o contador for impar sinal é negativo
        }
        (args->limiteIn)++;                                      // Incremento do limite inferior.
	}
	args->somat = equacao;                       // somat de cada estrutura recebe o resultado do calculo de cada Thread.
}


int testa_entrada(int argc, char* argv[])
{
    int ehValido = 1;       // Variavel para validar ou não a entrada
    int cont;               // Contador para o for

    if(argc != 3)           //  Condição caso o numero de argumentos seja diferente de 3 (programa, precisão e numero de threads).
    {
        printf("< NUMERO DE ARGUMENTOS INVALIDOS!!! >\n");
        ehValido = 0;
    }
    else
    {
        for(cont = 1; cont < argc; ++cont)
        {
            if(*argv[cont] == '0')                              // Se o valor do vetor na posição cont for o caractere '0'
            {
                ehValido = 0;
                printf("O ARGUMENTO <0> EH INVALIDO!!!\n");
                exit(0);
            }
            else if(*argv[cont] >= 49 && *argv[cont] <= 57)		// Valor do vetor precisa estar entre 49(1) e 57(9) da tabela ASCII para ser valido
            {
                ehValido = 1;
            }
            else	                                            // Se o valor do argumento não esta entre 49(1) e 57(9)
            {
                ehValido = 0;
                printf("O ARGUMENTO <%c> EH INVALIDO!!!\n", *argv[cont]);
                exit(0);
            }
        }
    }
    return ehValido;
}
