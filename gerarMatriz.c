#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double **aux;
int tam, nthreads;
double limite = 2;

/** @brief Função que será executada pelas threads, a divisão foi feita de forma que cada thread
 *          execute apenas uma linha da matriz.
 * 
 *
 *  Se o argumento da função não for válido, a função não funcionará
 *
 *  @param *arg, Recebe o id da thread que está executando no momento
 *  @return Void.
 */

void *tarefaMatriz(void *arg) {
  long int id = (long int)arg;  // identificador da thread
  srand(time(NULL));
  for (int i = id; i < tam; i += nthreads) {
    double somaLinha = 0;
    for (int j = 0; j < tam; j++) {
      if (i != j) {
        //double valor = ((rand() % tam) + 1) - ((rand() % 10 + 1)/11);
        double valor = ((double)rand()/(double)(RAND_MAX)) * limite;
        *(*(aux + i) + j) = valor;
        somaLinha += valor;
      }
    }
    *(*(aux + i) + i) = somaLinha/0.5;
  }
  pthread_exit(NULL);

  return NULL;
}

/** @brief  Função que será responsável por criar as threads e chamar a função *tarefaMatriz(),
 *          para cada uma delas, passando o id da thread atual como parâmetro.
 * 
 *  @return int.
 */

int execucao() {
  pthread_t *threadsId;

  // aloca memória para threads
  threadsId = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
  if (threadsId == NULL) {
    fprintf(stderr, "ERRO--malloc\n");
    return 2;
  }
  // execução das threads
  for (long int i = 0; i < nthreads; i++) {
    if (pthread_create(threadsId + i, NULL, tarefaMatriz, (void *)i)) {
      fprintf(stderr, "ERRO--pthread_create\n");
      return 3;
    }
  }
  // espera todas as threads terminarem
  for (long int i = 0; i < nthreads; i++) {
    if (pthread_join(*(threadsId + i), NULL)) {
      printf("--ERRO: pthread_join() \n");
      exit(-1);
    }
  }

  return 0;
}

/** @brief  Função responsável por gerar a matriz de forma concorrente.
 * 
 * 
 *  @param  tamanho             Tamanho que será gerada a matriz
 *  @param  **a                 Matriz a que deve ser preenchida com os valores que forem calculados pela função
 *  @param  numeroThreads       Quantidade de threads que serão utilizadas para executar a função
 *  @return Void.
 */

void gerarMatriz(int tamanho, double **a, int numeroThreads) {
  aux = a;
  tam = tamanho;
  nthreads = numeroThreads;
  execucao();
}
