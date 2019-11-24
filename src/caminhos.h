#include "buscas.h"

struct aresta{
    int saida, destino, peso;
};  //struct para arestas



//PRINTAR O CAMINHO ENCONTRADO PELOS ALGORITMOS
void print_caminho(struct node_largura* vertices[], int raiz, int destino, FILE *f){
    if(raiz == destino){
        printf("%d ", destino);
        fprintf(f, "%d ", destino);
    }
    else if(vertices[destino] -> pai == -1){
        printf("Não existe trajeto para  %d\n", destino);
        fprintf(f, "Não existe trajeto para  %d\n", destino);
    }
    else{
        print_caminho(vertices, raiz, vertices[destino] -> pai, f);
        printf("%d ", destino);
        fprintf(f, "%d ", destino);
    }
}


int distanciaMin(struct node_largura* vertices[], int S[], int tamanho){  //S é o array que controla os vertices que ja foram incluidos no caminho
    int min = 9999;
    int min_indice = -1;
    for(int k = 0; k < tamanho; k++){
        if(S[k] == 0 && vertices[k] -> distancia <= min){
            min = vertices[k] -> distancia;
            min_indice = k;
        }
    }
    return min_indice;
}

int dijkstra(int matriz_adj[TAM][TAM], int raiz, int tamanho, char *arquivo_destino){

    struct node_largura* vertices[tamanho];  //array que guarda os vertices

    int S[tamanho]; //array que ira ser "true" caso vertice ja tenha sido usado no caminho e "false" caso não
    for(int i = 0; i < tamanho; i ++){
        //inicializando vertices
        struct node_largura *temp = (struct node_largura*)malloc(sizeof(struct node_largura));
        if(i != raiz){
            temp -> vertice = i;
            temp -> distancia = 999;  //inicializando todos com distancia infinito, exceto a raiz
            temp -> pai = -1;         //pai como '-1' sinalizando o 'NULL'
            vertices[i] = temp;
        }
        else{
            temp -> vertice = i;
            temp -> distancia = 0;  //raiz iniciamos com 0
            temp -> pai = -1;       //pai como '-1' sinalizando o 'NULL'
            vertices[i] = temp;
        }

        //inicializando array S[]
        S[i] = 0;
    }
    for(int k = 0; k < tamanho; k++){
        int u = distanciaMin(vertices, S, tamanho);  //pega o vertice de menor distancia do array de vertices não processados
        S[u] = 1; //depois de ser pego, marcamos como utilizado no array S[]
        //relaxa
        for(int v = 0; v < tamanho; v++){
            //atualiza a distancia do vertice v, se existe uma aresta entre 'u' e 'v', e se o peso total
            //do caminho da raiz para o v for menor que a distancia atual de v
            if(matriz_adj[u][v] && (vertices[u] -> distancia + matriz_adj[u][v]) < vertices[v] -> distancia){
                vertices[v] -> distancia = (vertices[u] -> distancia) + matriz_adj[u][v];
                vertices[v] -> pai = u;
            }
        }
    }




    //Gravando resultados num arquivo texto
    FILE *f = fopen(arquivo_destino, "w+");
    if(f == NULL){
        printf("Não foi possivel realizar a gravação da sua tabela de resultados, por favor tente novamente!\n");
        log_print(arquivo_log_buscas, "[caminhos.h - dijkstra() - Linha 81]: ERRO DE GRAVAÇÃO", "Não foi possível gravar a sua tabela de resultados, por favor, tente novamente.\n");
        return 0;
    }
    fprintf(f, "RAIZ: %d\n", raiz);
    fprintf(f, "\n| VERTICE | DISTANCIA | PAI |\n\n");
    for(int k = 0; k < tamanho; k++){
        fprintf(f, "%5d %10d %10d\n", vertices[k] -> vertice, vertices[k] -> distancia, vertices[k] -> pai);
    }

    //printando caminhos
    fprintf(f, "\nCAMINHOS: \n");
    for(int j = 0; j < tamanho; j++){
        printf("\n\nCaminho ate %d: ", j);
        fprintf(f, "\nCaminho ate %d: ", j);
        print_caminho(vertices, raiz, j, f);
    }
    printf("\n\n");

    fclose(f);
    return 1;
}


//-----------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------BELLMAN-FORD------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------//


int bellman_ford(int matriz_adj[TAM][TAM], int raiz, int tamanho, char *arquivo_destino){
    int num_v = tamanho; //numero de vertices
    int num_a = numArestas(matriz_adj, tamanho); //num de arestas
    struct node_largura* vertices[tamanho];  //array que guarda os vertices
    for(int i = 0; i < tamanho; i ++){
        //inicializando vertices
        struct node_largura *temp = (struct node_largura*)malloc(sizeof(struct node_largura));
        if(i != raiz){
            temp -> vertice = i;
            temp -> distancia = 999;  //inicializando todos com distancia infinito, exceto a raiz
            temp -> pai = -1;         //pai como '-1' sinalizando o 'NULL'
            vertices[i] = temp;
        }
        else{
            temp -> vertice = i;
            temp -> distancia = 0;  //raiz iniciamos com 0
            temp -> pai = -1;       //pai como '-1' sinalizando o 'NULL'
            vertices[i] = temp;
        }
    }
    //guardando arestas num array
    struct aresta* arestas[num_a];
    int count = 0;
    for(int i = 0; i < tamanho; i++){
        for(int j = 0; j < tamanho; j++){
            struct aresta* temp = (struct aresta*)malloc(sizeof(struct aresta));
            if(matriz_adj[i][j] != 0){
                temp -> saida = i;
                temp -> destino = j;
                temp -> peso = matriz_adj[i][j];
                arestas[count] = temp;
                count++;
            }
        }
    }
    printf("ARESTAS:\n");
    for(int k = 0; k < num_a; k++){
        printf("SAIDA: %d  CHEGADA: %d  PESO: %d\n", arestas[k] -> saida, arestas[k] -> destino, arestas[k] -> peso);
    }
    //relaxa todas as arestas 'num_v - 1' vezes
    for(int i = 0; i <= num_v - 1; i++){
        for(int j = 0; j < num_a; j++){
            int u = arestas[j] -> saida;
            int v = arestas[j] -> destino;
            int peso = arestas[j] -> peso;
            if(vertices[u] -> distancia + peso < vertices[v] -> distancia){
                vertices[v] -> distancia = vertices[u] -> distancia + peso;
                vertices[v] -> pai = u;
            }
        }
    }

    //refaz para verificar se existem ciclos de peso negativo
    for(int i = 1; i <= num_a; i++){
        int u = arestas[i] -> saida;
        int v = arestas[i] -> destino;
        int peso = arestas[i] -> peso;
        if(vertices[u] -> distancia != 999 && vertices[u] -> distancia + peso < vertices[v] -> distancia){
            return 0;
        }
    }


    //Gravando resultados num arquivo texto
    FILE *f = fopen(arquivo_destino, "w+");
    if(f == NULL){
        printf("Não foi possivel realizar a gravação da sua tabela de resultados, por favor tente novamente!\n");
        log_print(arquivo_log_buscas, "[caminhos.h - bellman_ford() - Linha 179]: ERRO DE GRAVAÇÃO", "Não foi possível gravar a sua tabela de resultados, por favor, tente novamente.\n");
        return 0;
    }
    fprintf(f, "RAIZ: %d\n", raiz);
    fprintf(f, "\n| VERTICE | DISTANCIA | PAI |\n\n");
    for(int k = 0; k < tamanho; k++){
        fprintf(f, "%5d %10d %10d\n", vertices[k] -> vertice, vertices[k] -> distancia, vertices[k] -> pai);
    }

    //printando caminhos
    fprintf(f, "\nCAMINHOS: \n");
    for(int j = 0; j < tamanho; j++){
        printf("\n\nCaminho ate %d: ", j);
        fprintf(f, "\nCaminho ate %d: ", j);
        print_caminho(vertices, raiz, j, f);
    }
    printf("\n\n");

    fclose(f);
    return 1;

}
