#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "logger.h"
#define TAM 100



void inicializar_matriz(int matriz[TAM][TAM]);
void print_matriz(int matriz[TAM][TAM], int tamanho);
void escreve_matriz_arquivo(FILE *f, int matriz[TAM][TAM], int tipo, int tamanho);
void le_matriz_arquivo(FILE *f, int matriz[TAM][TAM],int *copia_tipo, int *copia_tamanho);




void clearScreen(){
    char ch;
    printf("\nAperte ENTER para continuar... ");
    scanf("%c",&ch);
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

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
    printf("MATRIZ DE ADJACENCIA:\n\n");
    printf("  ");
    for(int i = 0; i < tamanho; i++)
        printf(" %d", i);
    printf("\n\n");

    for(int i = 0; i < tamanho; i++){
        printf("%d ", i);
        for(int j = 0; j < tamanho; j++){
            printf(" %d", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
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

    printf("Lendo o tipo e o tamanho do grafo!\n");
    printf("Tipo: %d | Tamanho: %d\n\n", tipo, tamanho);

    int vertice_saida, vertice_chegada, peso;
    printf("Lendo vertices de saida e chegada, e seu peso: \n");
    while(fscanf(f, "%d %d %d", &vertice_saida, &vertice_chegada, &peso) != EOF){
        printf("SAIDA: %d | CHEGADA: %d | PESO: %d\n\n", vertice_saida, vertice_chegada, peso);
        if(peso == 0){
            matriz[vertice_saida][vertice_chegada] += 1;
        }
        else{
            matriz[vertice_saida][vertice_chegada] += peso;
        }
        print_matriz(matriz, tamanho);
    }
    printf("\n");
    *copia_tipo = tipo;
    *copia_tamanho = tamanho;
}
