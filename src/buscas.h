#include <stdio.h>
#include <stdlib.h>
#include "matrizAdjacencia.h"

int logger = 1;
char arquivo_log[100] = "arquivos/log_profundidade.log";

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
        log_print(arquivo_log, "buscas.h - visita_profundidade - Linha 32","\n  [buscas.h - visita_profundidade() -  Linha 32]:  DESCOBERTA DE %d: %d", node -> vertice, node -> descoberta);
        printf("\nDESCOBERTA DE %d: %d\n", node -> vertice, node -> descoberta);
        //clearScreen();
    }
    //para cada vertice adjacente
    for(int i = 0; i < tamanho; i++){
        if(matriz[node -> vertice][i] != 0){ //SE FOR ADJACENTE AO VERTICE I
            if(vertices[i] -> cor == 'B'){ // E SE O VERTICE AO QUAL É ADJACENTE FOR BRANCO
                if(logger){
                    log_print(arquivo_log, "buscas.h - visita_profundidade - Linha 41" ,"\n\n  [buscas.h - visita_profundidade() -  Linha 41]:  A partir de %d visitando: %d\n\n", node->vertice, i);
                    printf("\nA partir de %d visitando: %d\n", node->vertice, i);
                }
                visita_profundidade(vertices[i], vertices, matriz, tamanho, tempo);
            }
        }
    }
    node -> cor = 'P';
    *tempo = *tempo + 1;
    node -> finalizacao = *tempo;
    if(logger){
        log_print(arquivo_log, "buscas.h - visita_profundidade -  Linha 52", "\n\n  [buscas.h - visita_profundidade() -  Linha 52]:  FINALIZAÇÃO DE %d: %d\n", node -> vertice, node -> finalizacao);
        printf("\nFINALIZAÇÃO DE %d: %d\n", node -> vertice, node -> finalizacao);
    }



}
int busca_profundidade(int raiz, char *arquivo_grafo, char *arquivo_destino){
    //Lendo o grafo de um arquivo e gerando a matriz de ajd
    int tipo, tamanho;
    FILE *grafo = fopen(arquivo_grafo, "r+");

    if(grafo == NULL){
        if(logger)
            log_print(arquivo_log, arquivo_grafo, "\n [buscas.h - busca_profundidade() -  Linha 64]:  Não foi possivel abrir seu arquivo de grafo.\n");
        printf("Não foi possivel abrir seu arquivo de grafo.\n");
        return 0;
    }

    int matriz_adj[TAM][TAM];
    inicializar_matriz(matriz_adj);
    le_matriz_arquivo(grafo, matriz_adj, &tipo, &tamanho);

    //Printando matriz
    print_matriz(matriz_adj, tamanho);
    fclose(grafo);

    if(logger){
        //printando matriz no arquivo de log, e inicializacao da busca em profundidade
        log_print(arquivo_log, arquivo_grafo, "\n\n [matrizAdjacencia.h - le_matriz_arquivo() -  Linha 72] [matrizAdjacencia.h - print_matriz() -  Linha 36]: \n\n MATRIZ ADJACÊNCIA\n");
        for(int i = 0; i < tamanho; i++){
            for(int j = 0; j < tamanho; j++){
                log_print(arquivo_log, "buscas.h - Linha 84", " %d", matriz_adj[i][j]);

            }
            log_print(arquivo_log, "buscas.h - Linha 35", "\n");
        }

        printf("\n\n| Inicializando Busca em Profundidade |\n\n");
        log_print(arquivo_log, "buscas.h - Linha 35", "\n | INICIALIZANDO BUSCA EM PROFUNDIDADE |\n");
    }


    //Inicializando vertices
    struct node_profundidade* vertices[tamanho];

    if(logger == 1){
        printf("Vertice Raiz Inicializado!\n");
        printf("Inicializando restante dos vertices!\n\n");
        log_print(arquivo_log, "buscas.h - Linha 35", "\n  [buscas.h - busca_profundidade() -  Linha 104]:  Vértice Raiz: %d Inicializado \n  Inicializando restante dos vértices\n\n", raiz);
    }

    for(int j = 0; j < tamanho; j++){ //Criando as estruturas e Inicializando o resto dos vertices
        if(j != raiz){
            struct node_profundidade *temp = (struct node_profundidade*)malloc(sizeof(struct node_profundidade)); //variavel que irá armazenar temporariamente os vertices
            temp -> vertice = j;
            temp -> cor = 'B';
            vertices[j] = temp;
            if(logger == 1){
                log_print(arquivo_log, "buscas.h - Linha 35", "  Vértice Inicializado: %d \n", temp -> vertice);
                printf("Vertice Inicializado: %d \n", temp -> vertice);
            }

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
            visita_profundidade(vertices[i], vertices ,matriz_adj, tamanho, &tempo);
        }
    }

    //Gravando resultados num arquivo texto
    FILE *f = fopen(arquivo_destino, "w+");
    if(f == NULL){
        printf("Não foi possivel realizar a gravação da sua tabela de resultados, por favor tente novamente!\n");
        if(logger)
            log_print(arquivo_log, "busca.h - Linha 35", "\n\nTabela gerada não pôde ser gravada, tente novamente\n");
        return 0;
    }
    fprintf(f, "Arquivo do grafo: %s\n", arquivo_grafo);
    fprintf(f, "RAIZ: %d\n", raiz);
    fprintf(f, "\n| VERTICE | COR | DESCOBERTA | FINALIZAÇÃO |\n\n");
    for(int k = 0; k < tamanho; k++){
        fprintf(f, "%6d %7c %8d %14d\n", vertices[k] -> vertice, vertices[k] -> cor, vertices[k] -> descoberta, vertices[k] -> finalizacao);
    }

    fclose(f);
    if(logger){
        log_print(arquivo_log, "busca.h - Linha 35", "\n\n\n [buscas.h - busca_profundidade() -  Linha 144]: Tabela gerada gravada com sucesso no arquivo:  %s\n", arquivo_destino);
        log_print(arquivo_log, "busca.h - Linha 35", "------------------------------------------------------------------------------------------------------------------------------------");
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
    //free(temp);
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
        if(logger)
            log_print(arquivo_log, arquivo_grafo, "\n [buscas.h - busca_largura() -  Linha 222]:  Não foi possivel abrir seu arquivo de grafo.\n");
        printf("Não foi possivel abrir seu arquivo de grafo.\n");
        return 0;
    }

    int matriz_adj[TAM][TAM];
    inicializar_matriz(matriz_adj);
    le_matriz_arquivo(grafo, matriz_adj, &tipo, &tamanho);

    //Printando matriz
    print_matriz(matriz_adj, tamanho);
    //fechando arquivo de grafo
    fclose(grafo);
    //INICIANDO BUSCA
    if(logger){
        //printando matriz no arquivo de log, e inicializacao da busca em profundidade
        log_print(arquivo_log, arquivo_grafo, "\n\n [matrizAdjacencia.h - le_matriz_arquivo() -  Linha 72] [matrizAdjacencia.h - print_matriz() -  Linha 36]: \n\n MATRIZ ADJACÊNCIA \n");
        for(int i = 0; i < tamanho; i++){
            for(int j = 0; j < tamanho; j++){
                log_print(arquivo_log, "buscas.h - Linha 234", " %d", matriz_adj[i][j]);

            }
            log_print(arquivo_log, "buscas.h - Linha 234", "\n");
        }

        printf("\n\n| Inicializando Busca em Largura |\n\n");
        log_print(arquivo_log, "buscas.h - Linha 237", "\n | INICIALIZANDO BUSCA EM LARGURA |\n");
    }


    //Inicializando vertices
    struct node_largura* vertices[tamanho];
    struct node_largura* node_raiz = (struct node_largura*)malloc(sizeof(struct node_largura));//variavel que ira armazenar a raiz
    if(logger == 1){
        printf("Vertice Raiz Inicializado!\n");
        printf("Inicializando restante dos vértices!\n\n");
        log_print(arquivo_log, "buscas.h - Linha 262", "\n  [buscas.h - busca_largura() -  Linha 262]:  Vértice Raiz: %d Inicializado \n  Inicializando restante dos vértices\n\n", raiz);
    }

    for(int j = 0; j < tamanho; j++){ //Criando as estruturas e Inicializando o resto dos vertices
        struct node_largura *temp = (struct node_largura*)malloc(sizeof(struct node_largura)); //variavel que irá armazenar temporariamente os vertices
        if(j != raiz){
            temp -> vertice = j;
            temp -> cor = 'B';
            temp -> distancia = 999; //INFINITO
            temp -> pai = -1;
            vertices[j] = temp;
            if(logger == 1){
                printf("Vertice Inicializado: %d \n", temp -> vertice);
                log_print(arquivo_log, "buscas.h - Linha 262", "  [buscas.h - busca_largura() -  Linha 262]: Vértice Inicializado: %d \n", temp -> vertice);
            }
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

    if(logger){
        printf("\n\nEnfileirando a raiz %d\n", raiz);
        log_print(arquivo_log, "buscas.h - Linha 287", "\n\n  [buscas.h - busca_largura() -  Linha 287]: Enfileirando a raiz %d \n", raiz);
    }

    struct node_fila* atual = (struct node_fila*)malloc(sizeof(struct node_fila));

    while(queue -> primeiro != NULL){ //enquanto a fila nao estiver vazia
        atual = desenfileirar(queue);
        struct node_largura* vertice_atual = atual -> conteudo;

        if(logger){
            printf("\nDesenfileirando vértice: %d\n", vertice_atual -> vertice);
            log_print(arquivo_log, "buscas.h - Linha 287", "\n  [buscas.h - busca_largura() -  Linha 297]: Desenfileirando vértice: %d \n", vertice_atual -> vertice);
        }
        //para cada vertice adjacente

        if(logger){
            printf("\nVerificando os vertices adjacentes brancos ao: %d\n", vertice_atual -> vertice);
            log_print(arquivo_log, "buscas.h - Linha 304", "\n  [buscas.h - busca_largura() -  Linha 310]: Verificando os vértices adjacentes brancos à: %d\n", vertice_atual -> vertice);
        }
        for(int i = 0; i < tamanho; i++){
            if(matriz_adj[vertice_atual -> vertice][i] != 0){ //SE FOR ADJACENTE AO VERTICE I
                if(vertices[i] -> cor == 'B'){ // E SE O VERTICE AO QUAL É ADJACENTE FOR BRANCO
                    //ATUALIZANDO VERTICES VISITADOS
                    vertices[i] -> cor = 'C';
                    vertices[i] -> distancia = (vertice_atual -> distancia) + 1;
                    vertices[i] -> pai = vertice_atual -> vertice;
                    enfileirar(queue, vertices[i]);

                    if(logger){
                        printf("\nAtualizando vertice adjacente branco encontrado: %d\n", vertices[i] -> vertice);
                        log_print(arquivo_log, "buscas.h - Linha 287", "\n\n  Atualizando vértice adjacente encontrado: %d\n", vertices[i] -> vertice);
                        printf("Cor: %c \nDistancia: %d\nPai: %d\n", vertices[i] -> cor, vertices[i] -> distancia, vertices[i] -> pai);
                        log_print(arquivo_log, "buscas.h - Linha 287", "    [buscas.h - busca_largura() -  Linha 314]: Cor: %c \n", vertices[i] -> cor);
                        log_print(arquivo_log, "buscas.h - Linha 287", "    [buscas.h - busca_largura() -  Linha 315]: Distancia: %d \n", vertices[i] -> distancia);
                        log_print(arquivo_log, "buscas.h - Linha 287", "    [buscas.h - busca_largura() -  Linha 316]: Pai: %d \n", vertices[i] -> pai);
                    }
                }
            }
            //printf("DISTANCIA DO VERTICE %d: %d\n", vertices[i] -> vertice, vertices[i] -> distancia);
        }
        vertice_atual -> cor = 'P';

        if(logger){
            printf("Finalizando vertice atual: %d - Cor: %c\n", vertice_atual -> vertice, vertice_atual -> cor);
            log_print(arquivo_log, "buscas.h - Linha 287", "\n  [buscas.h - busca_largura() -  Linha 329]: Finalizando vertice atual : %d - Cor: P \n", vertice_atual -> vertice);
        }

    }


    //Gravando resultados num arquivo texto
    FILE *f = fopen(arquivo_destino, "w+");
    if(f == NULL){
        printf("Não foi possivel realizar a gravação da sua tabela de resultados, por favor tente novamente!\n");
        log_print(arquivo_log, "[buscas.h - busca_largura() -  Linha 342]: ERRO DE GRAVAÇÃO", "Não foi possível gravar a sua tabela de resultados, por favor, tente novamente.\n");
        return 0;
    }
    fprintf(f, "Arquivo do grafo: %s\n", arquivo_grafo);
    fprintf(f, "RAIZ: %d\n", raiz);
    fprintf(f, "\n| VERTICE | COR | DISTANCIA | PAI |\n\n");
    for(int k = 0; k < tamanho; k++){
        fprintf(f, "%5d %8c %8d %8d\n", vertices[k] -> vertice, vertices[k] -> cor, vertices[k] -> distancia, vertices[k] -> pai);
    }
    fclose(f);
    if(logger){
        log_print(arquivo_log, "busca.h - Linha 217", "\n\n\n [buscas.h - busca_largura() -  Linha 217]: Tabela gerada gravada com sucesso no arquivo:  %s\n", arquivo_destino);
        log_print(arquivo_log, "busca.h - Linha 217", "------------------------------------------------------------------------------------------------------------------------------------");
    }
    return 1;

}
