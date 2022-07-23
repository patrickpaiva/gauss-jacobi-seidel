#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>

#include "gerarMatriz.c"
#include "jacobiConcorrenteBarreira.c"
#include "jacobiSequencial.c"
#include "gaussSeidel.c"
#include "timer.h"

int nthreads, n; // numero de threads e tamanho da matriz nxn
int contadorJacobi = 0, contadorJacobiSeq = 0, contadorGaussSeidel = 0;  // contadores de iterações dos métodos analisados
double **a, *b, *x, *xn;

/** @brief Gera o vetor b do sistema linear, Ax = b
 * 
 *
 *  Se o argumento da função não for válido, a função não funcionará
 *
 *  @param tamanho    Tamanho que vamos gerar o vetor b
 *  @return Void.
 */
void gerarVetorB(int tamanho) {
  srand(time(NULL));
  for (int i = 0; i < tamanho; i++) {
    b[i] = (rand() % tamanho + 1) - ((rand() % 10 + 1)/10);
  }
}

/** @brief  Função main do código, responsável por alocar as estruturas de dados
 *          necessárias, bem como chamar os métodos analisados.
 * 
 *
 *  @param argc   Inteiro que recebe a quantidade de threads na execução.
 *  @param argv   Responsável por armazenar os dois argumentos passados pela linha de comando.
 *  @return int.
 */

int main(int argc, char *argv[]) {
  double ini, fim;  // tomada de tempo

  // Recebe o numero de threads e o numéro n de variáveis do sistema linear.
  if (argc < 3) {
    fprintf(stderr, "Digite: %s <numero threads> <numero de variaveis>\n",
            argv[0]);
    return 1;
  }
  nthreads = atoi(argv[1]); // número de threads
  n = atoi(argv[2]); // quantidade de variáveis no sistema linear

  // aloca espaço para as estruturas de dados
  a = (double **)malloc(n * sizeof(double *));
  for (int i = 0; i < n; i++) {
    *(a + i) = (double *)malloc(sizeof(double) * n);
  }
  b = (double *)malloc(n * sizeof(double));
  x = (double *)malloc(n * sizeof(double));
  xn = (double *)malloc(n * sizeof(double));

  if (a == NULL || b == NULL || x == NULL || xn == NULL) {
    fprintf(stderr, "ERRO--malloc\n");
    return 2;
  }
  GET_TIME(ini); // início da marcação de tempo criação da matriz A)
  gerarMatriz(n, a, nthreads); // Gerando matriz A (utilizando threads na geração)
  GET_TIME(fim); // fim da marcação de tempo
  printf("Tempo para gerar a matriz A:  %lf\n", fim - ini);
  
  printf("\n");
  GET_TIME(ini); // início da marcação de tempo para gerar b e x.

  gerarVetorB(n); // Gerando vetor do lado direito da igualdade
  for (int i = 0; i < n; i++) x[i] = 1;  // inicializa o vetor x com uns

  GET_TIME(fim); // fim da marcação de tempo
  printf("Tempo para gerar b e inicializar x:  %lf\n", fim - ini);

  printf("\n");
  GET_TIME(ini); // inicio da marcação de tempo da realização do método do gauss-jacobi concorrente
  jacobiConcorrente(n, a, b, x, xn, &contadorJacobi, nthreads);
  GET_TIME(fim); // fim da marcação de tempo

  printf("O Jacobi Concorrente foi finalizado com %d iteracoes.\n", contadorJacobi);
  printf("Tempo do metodo de Jacobi Concorrente:  %lf\n", fim - ini);

  for (int i = 0; i < n; i++) x[i] = 1;  // reinicializa o vetor x com uns
  printf("\n");
  GET_TIME(ini); // inicio da marcação de tempo da realização do método do gauss-jacobi sequencial
  jacobiSequencial(n, a, b, x, xn, &contadorJacobiSeq);
  GET_TIME(fim); // fim da marcação de tempo

  printf("O Jacobi Sequencial finalizado com %d iteracoes.\n", contadorJacobiSeq);
  printf("Tempo gasto no calculo de Jacobi Sequencial:  %lf\n", fim - ini);

  for (int i = 0; i < n; i++) x[i] = 1;  // reinicializa o vetor x com uns
  printf("\n");
  GET_TIME(ini); // inicio da marcação de tempo da realização do método de gauss-seidel
  gaussSeidel(n, a, b, x, xn, &contadorGaussSeidel);
  GET_TIME(fim); // fim da marcação de tempo

  printf("O Gauss Seidel finalizado com %d iteracoes.\n", contadorGaussSeidel);
  printf("Tempo gasto no calculo de Gauss Seidel:  %lf\n", fim - ini);

// libera memória alocada
  free(a);
  free(b);
  free(x);
  free(xn);
}