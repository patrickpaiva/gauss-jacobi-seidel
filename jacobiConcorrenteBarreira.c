#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double **aux, *bAux, *xAux, *xnAux;
int n, nthreads, *contJacobiAux, threadsFinalizadas = 0, fim = 0;

pthread_mutex_t mutex;
pthread_cond_t barreiraIndividual;
pthread_cond_t barreiraGlobal;

/** @brief Função que será executada pelas threads, a divisão foi feita de forma que cada thread
 *          calcule uma quantidade de vezes proporcional do nthreads.
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
  while(!fim){
    for (i = id; i < n; i += nthreads) {
      parcial = bAux[i];
      for (j = 0; j < n; j++) {
        if (j != i) {
          parcial -= (*(*(aux + i) + j)) * (xAux[j]);
        }
      }
      xnAux[i] = parcial / (*(*(aux + i) + i));
    }

    // aguardar autorizacao para realizar a proxima iteracao
		pthread_mutex_lock(&mutex);
    threadsFinalizadas++;
    if(threadsFinalizadas == nthreads){
      pthread_cond_signal(&barreiraGlobal);
    }
		pthread_cond_wait(&barreiraIndividual, &mutex);
		pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);

  return NULL;
}

/** @brief  Função responsável por orquestar o funcionamento das threads e da barreira,
 *          executando o método de Gauss-Jacobi.
 * 
 *  @return int.
 */

int execucaoJacobi() {
  pthread_t *tid;
  int flag = 1, i;
  double epsilon = 0.00000001; // erro aceito tolerável
  // aloca memória para threads
  tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
  if (tid == NULL) {
    fprintf(stderr, "ERRO--malloc\n");
    return 2;
  }

  // inicializa as variáveis de sincronização
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&barreiraIndividual, NULL);
  pthread_cond_init(&barreiraGlobal, NULL);

  // criar as threads
  for (long int i = 0; i < nthreads; i++) {
    if (pthread_create(tid + i, NULL, tarefaJacobiConc, (void *)i)) {
      fprintf(stderr, "ERRO--pthread_create\n");
      return 3;
    }
  }
  
  while(flag == 1) {
    (*contJacobiAux)++;

    pthread_mutex_lock(&mutex);
    while(threadsFinalizadas < nthreads){
      pthread_cond_wait(&barreiraGlobal, &mutex);
    }
    pthread_mutex_unlock(&mutex);

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
      threadsFinalizadas = 0;
      pthread_cond_broadcast(&barreiraIndividual);
    }

  };
  fim = 1;
  pthread_cond_broadcast(&barreiraIndividual); // libera as threads para encerrar

  //--espera todas as threads terminarem
  for (long int i = 0; i < nthreads; i++) {
    if (pthread_join(*(tid + i), NULL)) {
      printf("--ERRO: pthread_join() \n");
      exit(-1);
    }
  }

}

/** @brief  Função responsável chamar a execucaoJacobi() alocando todas as estruturas de dados necessárias.
 * 
 * 
 *  @param  tamanho             Tamanho n da matriz de coeficientes n x n
 *  @param  **a                 Matriz a de coeficientes
 *  @param  **b                 Matriz b valores a direita do sistema.
 *  @param  **x                 Matriz x de valores iniciais do sistema.
 *  @param  **xn                Matriz xn com os valores calculados na iteracao n.
 *  @param  *contJacobi         Contador do número de iterações do método.
 *  @param  numeroThreads       Quantidade de threads sendo utilizadas na execução.
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
