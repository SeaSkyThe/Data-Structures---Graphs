#include <stdio.h>
#include <stdlib.h>
#include "matrizAdjacencia.h"

int logger = 1;

//ASSINATURAS
void gerar_tabela_profundidade(int matriz[TAM][TAM]);
void gerar_tabela_largura(int matriz[TAM][TAM]);
void busca_profundidade(int matriz[TAM][TAM], int tamanho, int raiz, char *nome_arquivo);
void busca_largura(int matriz[TAM][TAM],int tamanho, int raiz);
//void visita_profundidade(struct node_profundidade* node, struct node_profundidade* vertices[] ,int matriz[TAM][TAM], int tamanho, int *tempo);


//IMPLEMENTANDO BUSCA EM PROFUNDIDADE ------------------------------------------------------------------------------------------------------------

struct node_profundidade{  //Estrutura que será utilizada para facilitar a gravação em arquivo
    int vertice; //conteudo do vertice
    char cor; // B = BRANCO; C = CINZA; P = PRETO
    int descoberta;
    int finalizacao;
};

void visita_profundidade(struct node_profundidade* node, struct node_profundidade* vertices[] ,int matriz[TAM][TAM], int tamanho, int *tempo){
    node -> cor = 'C';  //atualizando cor do vertice visitado
    *tempo = *tempo + 1; //atualizando o tempo
    node -> descoberta = *tempo; //atualizando o tempo de descoberta
    if(logger){
        printf("Tempo de descoberta de %d: %d\n", node -> vertice, node -> descoberta);
    }
    //para cada vertice adjacente
    for(int i = 0; i < tamanho; i++){
        if(matriz[node -> vertice][i] != 0){ //SE FOR ADJACENTE AO VERTICE I
            if(vertices[i] -> cor == 'B'){ // E SE O VERTICE AO QUAL É ADJACENTE FOR BRANCO
                if(logger)
                    printf("A partir de %d visitando: %d\n", node->vertice, i);
                visita_profundidade(vertices[i], vertices, matriz, tamanho, tempo);
            }
        }
    }
    node -> cor = 'P';
    *tempo = *tempo + 1;
    node -> finalizacao = *tempo;
    printf("\nTempo de finalizacao de %d: %d\n", node -> vertice, node -> finalizacao);


}
void busca_profundidade(int matriz[TAM][TAM], int tamanho, int raiz, char *nome_arquivo){
    if(logger == 1)
        printf("\n\n| Inicializando Busca em Profundidade |\n\n");
    struct node_profundidade* vertices[tamanho];
    struct node_profundidade *node_raiz = (struct node_profundidade*)malloc(sizeof(struct node_profundidade));//variavel que ira armazenar a raiz
    node_raiz -> vertice = raiz; //criando o nó de raiz priemiro
    node_raiz -> cor = 'B';
    vertices[0] = node_raiz; //colocando a raiz na primeira pos do vetor
    for(int j = 1; j < tamanho; j++){ //Criando as estruturas e Inicializando todos os vertices
        if(j != raiz){
            struct node_profundidade *temp = (struct node_profundidade*)malloc(sizeof(struct node_profundidade)); //variavel que irá armazenar temporariamente os vertices
            temp -> vertice = j;
            temp -> cor = 'B';
            vertices[j] = temp;
            if(logger == 1) //LOG DE PROCESSAMENTO
                printf("Vertice Inicializado: %d \n", temp -> vertice);
        }
    }
    int tempo = 0;
    for(int i = 0; i < tamanho; i++){
        if(vertices[i] -> cor == 'B'){ //Se a cor do vertice for branca, iremos chamar a funcao de visita a partir dele
            if(logger == 1){
                printf("\nVertice que sera visitado primeiro: %d\n", vertices[i] -> vertice);
            }
            visita_profundidade(vertices[i], vertices ,matriz, tamanho, &tempo);
        }
    }

    //Gravando resultados num arquivo texto
    FILE *f = fopen(nome_arquivo, "r+");
    fprintf(f, "%d\n", raiz);
    for(int k = 0; k < tamanho; k++){
        fprintf(f, "%d %c %d %d\n", vertices[k] -> vertice, vertices[k] -> cor, vertices[k] -> descoberta, vertices[k] -> finalizacao);
    }
}

//----------------------------------------------------------------------------------------------------------------------//
//FIM DA IMPLEMENTACAO DA BUSCA EM PROFUNDIDADE, INICIO DA BUSCA EM LARGURA
struct node_largura{
    int vertice;
    char cor;  // B = BRANCO; C = CINZA; P = PRETO
    int distância;
    struct node_largura* pai;
};
