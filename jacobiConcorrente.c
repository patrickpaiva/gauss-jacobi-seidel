#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double **aux, *bAux, *xAux, *xnAux;
int n, nthreads, *contJacobiAux;

/** @brief Função que será executada pelas threads, a divisão foi feita de forma que cada thread
 *          execute apenas uma linha da matriz.
 * 
 *
 *  Se o argumento da função não for válido, a função não funcionará
 *
 *  @param *arg, Recebe o id da thread que está executando no momento
 *  @return Void.
 */

void *tarefaJacobiConc(void *arg) {
  long int id = (long int)arg;  // identificador da thread
  double parcial;
  int i, j;
  for (i = id; i < n; i += nthreads) {
    parcial = bAux[i];
    for (j = 0; j < n; j++) {
      if (j != i) {
        parcial -= (*(*(aux + i) + j)) * (xAux[j]);
      }
    }
    xnAux[i] = parcial / (*(*(aux + i) + i));
  }
  pthread_exit(NULL);

  return NULL;
}

/** @brief  Função que será responsável por criar as threads e chamar a função *tarefaMatriz(),
 *          para cada uma delas, passando o id da thread atual como parâmetro.
 * 
 *  @return int.
 */

int execucaoJacobi() {
  pthread_t *tid;
  int flag, i;
  double epsilon = 0.000001; // erro aceito tolerável
  // aloca memória para threads
  tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
  if (tid == NULL) {
    fprintf(stderr, "ERRO--malloc\n");
    return 2;
  }
  do {
    (*contJacobiAux)++;
    // criar as threads
    for (long int i = 0; i < nthreads; i++) {
      if (pthread_create(tid + i, NULL, tarefaJacobiConc, (void *)i)) {
        fprintf(stderr, "ERRO--pthread_create\n");
        return 3;
      }
    }
    //--espera todas as threads terminarem
    for (long int i = 0; i < nthreads; i++) {
      if (pthread_join(*(tid + i), NULL)) {
        printf("--ERRO: pthread_join() \n");
        exit(-1);
      }
    }

    flag = 1; // sinalizador para saber quando parar a execução

    // verifica condicao de parada |x[i]-xn[i]|<epsilon para todo i
    for (i = 0; i < n; i++) {
      if (fabs(xAux[i] - xnAux[i]) < epsilon) {
        flag = 0;
      }
    }
    if (flag == 1) {
      for (i = 0; i < n; i++) {
        xAux[i] = xnAux[i];  // atualiza x[i]	para a proxima iteracao
      }
    }
  } while (flag == 1);
}

/** @brief  Função responsável por gerar a matriz de forma concorrente.
 * 
 * 
 *  @param  tamanho             Tamanho que será gerada a matriz
 *  @param  **a                 Matriz a que deve ser preenchida com os valores que forem calculados pela função
 *  @param  numeroThreads       Quantidade de threads que serão utilizadas para executar a função
 *  @return Void.
 */

void jacobiConcorrente(int tamanho, double **a, double *b, double *x, double *xn, int *contJacobi, int numeroThreads) {
  aux = a;
  bAux = b;
  xAux = x;
  xnAux = xn;
  n = tamanho;
  nthreads = numeroThreads;
  contJacobiAux = contJacobi;
  execucaoJacobi();
}
