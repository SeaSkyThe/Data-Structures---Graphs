#include <stdio.h>
#include <stdlib.h>
#include "buscas.h"


int main(void){

    // //Lendo matriz de adj de um arquivo
    // int tipo, tamanho;
    // FILE *f = fopen("arquivos/1.txt", "r+");
    //
    // if(f == NULL){
    //     return 0;
    // }
    // int matriz_adj[TAM][TAM];
    // inicializar_matriz(matriz_adj);
    // le_matriz_arquivo(f, matriz_adj, &tipo, &tamanho);
    //
    // //Printando matriz
    // print_matriz(matriz_adj, tamanho);
    //
    // //Escrevendo a matriz num arquivo
    // FILE *escrita = fopen("arquivos/escrita_matriz.txt", "w+");
    // escreve_matriz_arquivo(escrita, matriz_adj, tipo, tamanho);


    //Testes de buscas.

    //busca_profundidade(0, "arquivos/2.txt", "arquivos/tabela_profundidade2.txt"); //primeiro parametro é raiz, segundo parametro é arquivo do grafo e o terceiro o arquivo de saida
    busca_largura(0, "arquivos/2.txt", "arquivos/tabela_largura2.txt");
    return 0;
}
