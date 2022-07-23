#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double **aux, *bAux, *xAux, *xnAux;
int n, *contGaussSeidelAux;

/** @brief  Função responsável por executar os cálculos do método de Gauss-Seidel.
 *          
 * 
 *  @return int.
 */

void execucaoGaussSeidel() {
  int flag, i;
  double epsilon = 0.000001; // erro aceito tolerável

  do {
    (*contGaussSeidelAux)++;

    double parcial;
    for (int i = 0; i < n; i ++) {
      parcial = bAux[i];
      for (int j = 0; j < n; j++) {
        if(j < i){
          parcial -= (*(*(aux + i) + j)) * (xnAux[j]);
        } else if(j > i) {
          parcial -= (*(*(aux + i) + j)) * (xAux[j]);
        }
      }
      xnAux[i] = parcial / (*(*(aux + i) + i));
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

/** @brief  Função responsável por alocar as estruturas de dados necessárias e chamar a execucaoGaussSeidel().
 * 
 * 
 *  @param  tamanho             Tamanho n da matriz de coeficientes n x n
 *  @param  **a                 Matriz a de coeficientes
 *  @param  **b                 Matriz b valores a direita do sistema.
 *  @param  **x                 Matriz x de valores iniciais do sistema.
 *  @param  **xn                Matriz xn com os valores calculados na iteracao n.
 *  @param  *contGaussSeidel      Contador do número de iterações do método.
 *  @return Void.
 */

void gaussSeidel(int tamanho, double **a, double *b, double *x, double *xn, int *contGaussSeidel) {
  aux = a;
  bAux = b;
  xAux = x;
  xnAux = xn;
  n = tamanho;
  contGaussSeidelAux = contGaussSeidel;
  execucaoGaussSeidel();
}
