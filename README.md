# Gauss-Jacobi Concorrente x Gauss-Seidel

## Implementação Concorrente do Método Gauss-Jacobi

Esta é uma implementação feita em C do método de Gauss-Jacobi para resolução de sistemas lineares aplicando concorrência através da utilização da biblioteca pthread.  
 
O programa principal "gauss-jacobi-seidel" quando executado gera uma matriz A nxn diagonalmente dominante e um vetor b de tamanho n compondo assim um sistema linear da forma Ax = b. Após essa geração, o programa calcula a solução do sistema utilizando os métodos de Gauss-Jacobi na forma concorrente, na forma sequencial e também o Gauss-Seidel.

As três execuções acontecem utilizando exatamente o mesmo sistema, para que seja possível comparar o tempo de processamento. A príncipio o método de Gauss-Seidel é mais rápido por precisar de menos iterações que o Jacobi. No entanto, com a possibilidade de uso da concorrência, temos uma execução do Gauss-Jacobi até 6x mais rápida para um mesmo sistema.

## Exemplos de execução e saída
Matriz 10000 x 10000 com 12 threads:

```
Tempo para gerar a matriz A:  0.403452

Tempo para gerar b e inicializar x:  0.000309

O Jacobi Concorrente foi finalizado com 4 iteracoes.
Tempo do metodo de Jacobi Concorrente:  0.153479

O Jacobi Sequencial finalizado com 4 iteracoes.
Tempo gasto no calculo de Jacobi Sequencial:  1.124676

O Gauss Seidel finalizado com 3 iteracoes.
Tempo gasto no calculo de Gauss Seidel:  0.850920
```

Matriz 40000 x 40000 com 12 threads:
```
Tempo para gerar a matriz A:  9.817114

Tempo para gerar b e inicializar x:  0.002185

O Jacobi Concorrente foi finalizado com 4 iteracoes.
Tempo do metodo de Jacobi Concorrente:  2.262700

O Jacobi Sequencial finalizado com 4 iteracoes.
Tempo gasto no calculo de Jacobi Sequencial:  18.141780

O Gauss Seidel finalizado com 2 iteracoes.
Tempo gasto no calculo de Gauss Seidel:  9.187405
```

## Compilando e rodando:

Compilando o programa:
```
gcc gauss-jacobi-seidel.c -Wall -o gauss-jacobi-seidel -lpthread
```
Rode na forma:
./gauss-jacobi-seidel.exe <numero threads> <numero de variaveis>
```
.\gauss-jacobi-seidel.exe 12 10000
```

## Desenvolvimento
Este programa foi desenvolvido durante os cursos de Computação Concorrente (Prof. Silvana Rossetto) e Cálculo Numérico (Prof. Marcello Teixeira) no período 2022.1 na UFRJ.
