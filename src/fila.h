
//nó da busca em largura
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
