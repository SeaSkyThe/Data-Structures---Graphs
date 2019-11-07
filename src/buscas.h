#include <stdio.h>
#include <stdlib.h>
#include "matrizAdjacencia.h"

int logger = 1;


//ASSINATURAS
void gerar_tabela_profundidade(int matriz[TAM][TAM]);
void gerar_tabela_largura(int matriz[TAM][TAM]);

int busca_profundidade(int raiz, char *arquivo_grafo, char *arquivo_destino);

//int busca_largura(int matriz[TAM][TAM],int tamanho, int raiz);
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
        printf("\nTempo de descoberta de %d: %d\n", node -> vertice, node -> descoberta);
    }
    //para cada vertice adjacente
    for(int i = 0; i < tamanho; i++){
        if(matriz[node -> vertice][i] != 0){ //SE FOR ADJACENTE AO VERTICE I
            if(vertices[i] -> cor == 'B'){ // E SE O VERTICE AO QUAL É ADJACENTE FOR BRANCO
                if(logger)
                    printf("\n\nA partir de %d visitando: %d\n", node->vertice, i);
                visita_profundidade(vertices[i], vertices, matriz, tamanho, tempo);
            }
        }
    }
    node -> cor = 'P';
    *tempo = *tempo + 1;
    node -> finalizacao = *tempo;
    if(logger)
        printf("\nTempo de finalizacao de %d: %d\n", node -> vertice, node -> finalizacao);


}
int busca_profundidade(int raiz, char *arquivo_grafo, char *arquivo_destino){
    //Lendo o grafo de um arquivo e gerando a matriz de ajd
    int tipo, tamanho;
    FILE *grafo = fopen(arquivo_grafo, "r+");

    if(grafo == NULL){
        printf("Não foi possivel abrir seu arquivo de grafo.\n");
        return 0;
    }

    int matriz_adj[TAM][TAM];
    inicializar_matriz(matriz_adj);
    le_matriz_arquivo(grafo, matriz_adj, &tipo, &tamanho);

    //Printando matriz
    print_matriz(matriz_adj, tamanho);


    //AQUI ACABA O TRATAMENTO DA TABELA
    if(logger == 1)
        printf("\n\n| Inicializando Busca em Profundidade |\n\n");

    //Inicializando vertices
    struct node_profundidade* vertices[tamanho];

    if(logger == 1){
        printf("Vertice Raiz Inicializado!\n");
        printf("Inicializando restante dos vértices!\n\n");
    }

    for(int j = 0; j < tamanho; j++){ //Criando as estruturas e Inicializando o resto dos vertices
        if(j != raiz){
            struct node_profundidade *temp = (struct node_profundidade*)malloc(sizeof(struct node_profundidade)); //variavel que irá armazenar temporariamente os vertices
            temp -> vertice = j;
            temp -> cor = 'B';
            vertices[j] = temp;
            if(logger == 1) //LOG DE PROCESSAMENTO
                printf("Vertice Inicializado: %d \n", temp -> vertice);
        }
        else{
            struct node_profundidade* node_raiz = (struct node_profundidade*)malloc(sizeof(struct node_profundidade));//variavel que ira armazenar a raiz
            node_raiz -> vertice = raiz; //criando o nó da raiz
            node_raiz -> cor = 'B';
            vertices[j] = node_raiz; //colocando a raiz no vetor
        }
    }
    //Fim da inicializacao de vertices

    int tempo = 0;
    visita_profundidade(vertices[raiz], vertices ,matriz_adj, tamanho, &tempo);
    for(int i = 0; i < tamanho; i++){
        if(vertices[i] -> cor == 'B' && i != raiz){ //Se a cor do vertice for branca, iremos chamar a funcao de visita a partir dele
            if(logger == 1){
                printf("\n\n\nVertice que sera visitado: %d\n", vertices[i] -> vertice);
            }
            visita_profundidade(vertices[i], vertices ,matriz_adj, tamanho, &tempo);
        }
    }

    //Gravando resultados num arquivo texto
    FILE *f = fopen(arquivo_destino, "w+");
    if(f == NULL){
        printf("Não foi possivel realizar a gravação da sua tabela de resultados, por favor tente novamente!\n");
        return 0;
    }
    fprintf(f, "Arquivo do grafo: %s\n", arquivo_grafo);
    fprintf(f, "RAIZ: %d\n", raiz);
    fprintf(f, "\n| VERTICE | COR | DESCOBERTA | FINALIZAÇÃO |\n\n");
    for(int k = 0; k < tamanho; k++){
        fprintf(f, "%6d %7c %8d %14d\n", vertices[k] -> vertice, vertices[k] -> cor, vertices[k] -> descoberta, vertices[k] -> finalizacao);
    }
    return 1;
}

//----------------------------------------------------------------------------------------------------------------------//
//FIM DA IMPLEMENTACAO DA BUSCA EM PROFUNDIDADE, INICIO DA BUSCA EM LARGURA

struct node_largura{
    int vertice;
    char cor;  // B = BRANCO; C = CINZA; P = PRETO
    int distancia;
    int pai;
};

//Implementando fila
struct node_fila{
    struct node_largura* conteudo;
    struct node_fila* prox;
};

struct fila{
    struct node_fila *primeiro;
    struct node_fila *ultimo;
};

struct node_fila* new_Node(struct node_largura* conteudo){
    struct node_fila* temp = (struct node_fila*)malloc(sizeof(struct node_fila));
    temp -> conteudo = conteudo;
    temp -> prox = NULL;
    return temp;
}

struct fila* criar_fila(){
    struct fila* fila = (struct fila*)malloc(sizeof(struct fila));
    fila -> primeiro = fila -> ultimo = NULL;
    return fila;
}

void enfileirar(struct fila *fila, struct node_largura* conteudo){
    struct node_fila* novo = new_Node(conteudo);

    if(fila -> primeiro == NULL){
        fila -> primeiro  = fila -> ultimo = novo;
        return;
    }

    fila -> ultimo -> prox = novo;
    fila -> ultimo = novo;
}

struct node_fila* desenfileirar(struct fila *fila){
    if(fila -> primeiro == NULL)
        return NULL;

    struct node_fila* temp = fila -> primeiro;
    free(temp);
    fila -> primeiro = fila -> primeiro -> prox;

    if(fila -> primeiro == NULL)
        fila -> ultimo = NULL;

    return temp;
}

//Iniciando implementacao da busca
int busca_largura(int raiz, char *arquivo_grafo, char *arquivo_destino){
    //Lendo o grafo de um arquivo e gerando a matriz de ajd
    int tipo, tamanho;
    FILE *grafo = fopen(arquivo_grafo, "r+");

    if(grafo == NULL){
        printf("Não foi possivel abrir seu arquivo de grafo.\n");
        return 0;
    }

    int matriz_adj[TAM][TAM];
    inicializar_matriz(matriz_adj);
    le_matriz_arquivo(grafo, matriz_adj, &tipo, &tamanho);

    //Printando matriz
    print_matriz(matriz_adj, tamanho);

    //INICIANDO BUSCA
    if(logger == 1)
        printf("\n\n| Inicializando Busca em Largura |\n\n");

    //Inicializando vertices
    struct node_largura* vertices[tamanho];
    struct node_largura* node_raiz = (struct node_largura*)malloc(sizeof(struct node_largura));//variavel que ira armazenar a raiz
    if(logger == 1){
        printf("Vertice Raiz Inicializado!\n");
        printf("Inicializando restante dos vértices!\n\n");
    }

    for(int j = 0; j < tamanho; j++){ //Criando as estruturas e Inicializando o resto dos vertices
        if(j != raiz){
            struct node_largura *temp = (struct node_largura*)malloc(sizeof(struct node_largura)); //variavel que irá armazenar temporariamente os vertices
            temp -> vertice = j;
            temp -> cor = 'B';
            temp -> distancia = INFINITY; //INFINITO
            temp -> pai = -1;
            vertices[j] = temp;
            if(logger == 1) //LOG DE PROCESSAMENTO
                printf("Vertice Inicializado: %d \n", temp -> vertice);
        }
        else{
            node_raiz -> vertice = raiz; //criando o nó de raiz
            node_raiz -> cor = 'C';
            node_raiz -> distancia = 0;
            node_raiz -> pai = -1;
            vertices[raiz] = node_raiz; //colocando a raiz no vetor
        }
    }
    //Fim da inicializacao

    //Criando fila auxiliar
    struct fila *queue = criar_fila();

    enfileirar(queue, vertices[raiz]);

    while(queue -> primeiro != NULL){ //enquanto a fila nao estiver vazia
        struct node_fila* atual = (struct node_fila*)malloc(sizeof(struct node_fila));
        atual = desenfileirar(queue);
        struct node_largura* vertice_atual = atual -> conteudo;

        //para cada vertice adjacente
        for(int i = 0; i < tamanho; i++){
            if(matriz_adj[vertice_atual -> vertice][i] != 0){ //SE FOR ADJACENTE AO VERTICE I
                if(vertices[i] -> cor == 'B'){ // E SE O VERTICE AO QUAL É ADJACENTE FOR BRANCO
                    vertices[i] -> cor = 'C';
                    vertices[i] -> distancia = vertice_atual -> distancia + 1;
                    vertices[i] -> pai = vertice_atual -> vertice;
                    enfileirar(queue, vertices[i]);
                }
            }
        }
        vertice_atual -> cor = 'P';
    }


    //Gravando resultados num arquivo texto
    FILE *f = fopen(arquivo_destino, "w+");
    if(f == NULL){
        printf("Não foi possivel realizar a gravação da sua tabela de resultados, por favor tente novamente!\n");
        return 0;
    }
    fprintf(f, "Arquivo do grafo: %s\n", arquivo_grafo);
    fprintf(f, "RAIZ: %d\n", raiz);
    fprintf(f, "\n| VERTICE | COR | DISTANCIA | PAI |\n\n");
    for(int k = 0; k < tamanho; k++){
        fprintf(f, "%5d %8c %8d %8d\n", vertices[k] -> vertice, vertices[k] -> cor, vertices[k] -> distancia, vertices[k] -> pai);
    }
    return 1;

}
