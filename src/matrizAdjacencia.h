#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "logger.h"
#define TAM 100
char *arquivo_log_matrizes = "arquivos/log_matrizes.log";
int logger = 1;


// void inicializar_matriz(int matriz[TAM][TAM]);
// void print_matriz(int matriz[TAM][TAM], int tamanho);
// void escreve_matriz_arquivo(FILE *f, int matriz[TAM][TAM], int tipo, int tamanho);
// void le_matriz_arquivo(FILE *f, int matriz[TAM][TAM],int *copia_tipo, int *copia_tamanho);






void inicializar_matriz(int matriz[TAM][TAM]){
    //Inicializando matriz
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            matriz[i][j] = 0;
        }
    }
}
void print_matriz(int matriz[TAM][TAM], int tamanho){
    //Printando matriz
    //printf("MATRIZ DE ADJACENCIA:\n\n");
    printf("  ");
    for(int i = 0; i < tamanho; i++)
        printf("  %2d", i);
    printf("\n\n");

    for(int i = 0; i < tamanho; i++){
        printf("%d ", i);
        for(int j = 0; j < tamanho; j++){
            printf(" %3d", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    if(logger){
        log_print(arquivo_log_matrizes, "matrizAdjacencia.h", "\n\n [matrizAdjacencia.h - print_matriz() -  Linha 31]  \n\n MATRIZ ADJACÊNCIA \n");
        for(int i = 0; i < tamanho; i++){
            for(int j = 0; j < tamanho; j++){
                log_print(arquivo_log_matrizes, "matrizAdjacencia.h", " %d", matriz[i][j]);

            }
            log_print(arquivo_log_matrizes, "matrizAdjacencia.h", "\n");
        }
        log_print(arquivo_log_matrizes, "matrizAdjacencia.h", "                 Matriz Lida com Sucesso!!\n --------------------------------------------------------------------------------------------");
    }
}

void escreve_matriz_arquivo(FILE *f ,int matriz[TAM][TAM], int tipo, int tamanho){
    //Escrevendo em arquivo
    fprintf(f, "%d\n", tipo);
    fprintf(f, "%d\n", tamanho);
    for(int i = 0; i < tamanho; i++){
        printf("Varrendo a linha %d da matriz: \n", i);
        for(int j = 0; j < tamanho; j++){
            if(matriz[i][j] != 0){
                printf("Escrevendo a linha %d coluna %d da matriz no arquivo...\n", i, j);
                if(matriz[i][j] == 1)
                    fprintf(f, "%d %d 0\n", i, j);
                else
                    fprintf(f, "%d %d %d\n", i, j, matriz[i][j]);
            }
        }
        printf("\n");
    }
}
void le_matriz_arquivo(FILE *f, int matriz[TAM][TAM],int *copia_tipo, int *copia_tamanho){
    //Lendo arquivo
    int tipo, tamanho;

    fscanf(f, "%d", &tipo);
    fscanf(f, "%d", &tamanho);

    if(logger){
        printf("Lendo o tipo e o tamanho do grafo!\n");
        printf("Tipo: %d | Tamanho: %d\n\n", tipo, tamanho);
        log_print(arquivo_log_matrizes, "matrizAdjacencia.h - linha 79", "\n\n[matrizAdjacencia.h - le_matriz_arquivo() - Linha 79]: \n    Lendo o tipo e o tamanho do grafo!\n");
        log_print(arquivo_log_matrizes, "matrizAdjacencia.h - linha 79", "    TIPO: %d | ", tipo);
        log_print(arquivo_log_matrizes, "matrizAdjacencia.h - linha 79", "TAMANHO: %d \n\n", tamanho);

        printf("Lendo vertices de saida e chegada, e seu peso: \n");
        log_print(arquivo_log_matrizes, "matrizAdjacencia.h - linha 79", "\n\n[matrizAdjacencia.h - le_matriz_arquivo() - Linha 79]: Lendo vértices de saída e chegada, e seu peso: \n");
    }


    int vertice_saida, vertice_chegada, peso;


    while(fscanf(f, "%d %d %d", &vertice_saida, &vertice_chegada, &peso) != EOF){
        if(logger){
            printf("SAIDA: %d | CHEGADA: %d | PESO: %d\n\n", vertice_saida, vertice_chegada, peso);
            log_print(arquivo_log_matrizes, "matrizAdjacencia.h - linha 79", "\n  SAIDA: %d |", vertice_saida);
            log_print(arquivo_log_matrizes, "matrizAdjacencia.h - linha 79", " CHEGADA: %d |", vertice_chegada);
            log_print(arquivo_log_matrizes, "matrizAdjacencia.h - linha 79", " PESO: %d \n\n", peso);
        }

        if(peso == 0){
            matriz[vertice_saida][vertice_chegada] += 1;
            if(tipo == 0){
                matriz[vertice_chegada][vertice_saida] += 1;
            }
        }
        else{
            matriz[vertice_saida][vertice_chegada] += peso;
            if(tipo == 0){
                matriz[vertice_chegada][vertice_saida] += peso;
            }
        }
    }
    print_matriz(matriz, tamanho);
    printf("\n");
    *copia_tipo = tipo;
    *copia_tamanho = tamanho;
}


int numArestas(int matriz_adj[TAM][TAM], int tamanho, int tipo){
    int numArestas = 0;
    for(int j = 0; j < tamanho; j++){
        for(int i = 0; i < tamanho; i++){
            if(matriz_adj[j][i] != 0){
                numArestas++;
            }
        }
    }
    if(tipo == 0)
        return (numArestas/2);
    return numArestas;
}
