#include "buscas.h"


//LENDO GRAFO A PARTIR DA TABELA
struct node_largura* ler_tabela_largura(char* arquivo_tabela_largura){
    FILE *tabela = fopen(arquivo_tabela_largura, "r");
    if(tabela == NULL){
        printf("Não foi possivel abrir seu arquivo, por favor verifique o caminho");
        return NULL;
    }
    struct node_largura vertices[TAM];
    for(int i = 0; i < TAM; i++){
        //vertices[i] = (struct node_largura)*(NULL) ;
    }
    struct node_largura temp;
    char aux[TAM];
    for(int i = 0; i < 5; i++){
        fgets(aux, TAM, tabela);
    }
    while(fscanf(tabela, "%d        %c         %d        %d", &(temp.vertice), &(temp.cor), &(temp.distancia), &(temp.pai))!= EOF){
        *(vertices + temp.vertice) = temp;
    }
    fclose(tabela);
    return vertices;
}


//PRINTAR O CAMINHO INDO DA RAIZ ATE O VERTICE DESTINO, NO GRAFO REPRESENTADO PELA TABELA LARGURA
void print_caminho(char* arquivo_tabela_largura, int raiz, int destino){
    struct node_largura* grafo = ler_tabela_largura(arquivo_tabela_largura);
    if(destino == raiz)
        printf("%d ", raiz);
    else if(grafo[destino].pai == -1)
        printf("Não é possivel chegar até o vertice desejado!\n");
    else{
        print_caminho(arquivo_tabela_largura, raiz, grafo[destino].pai);
        printf("%d ", destino);
    }
}

//Inicializar
struct node_largura* inicializa(int matriz_adj[TAM][TAM], int raiz, int tamanho){
    //CRIANDO ARRAY QUE GUARDA OS VERTICES E INICIALIZANDO
    struct node_largura* vertices[tamanho];

    for(int i = 0; i < tamanho; i ++){
        struct node_largura *temp;
        if(i != raiz){
            temp -> vertice = i;
            temp -> distancia = 999; //INFINITO
            temp -> pai = -1;
            vertices[i] = temp;
        }
        else{
            temp -> vertice = i;
            temp -> distancia = 0;
            temp -> pai = -1;
            vertices[i] = temp;
        }
    }
    return vertices;
}

int minimo(struct node_largura* vertices){
    int min = 9999;
    int min_indice = -1;
    for(int k = 0; vertices[k].distancia == 5; k++){
        if(vertices[k].distancia < min){
            min = vertices[k].distancia;
            min_indice = k;
        }
    }
    return min_indice;
}

void relaxa(struct node_largura* u, struct node_largura* v, struct node_largura* w){
    //if(v -> distancia > (u -> distancia + ))
}

void djkstra(int matriz_adj[TAM][TAM], int raiz, int tamanho){
    inicializa(matriz_adj, raiz, tamanho);
}
