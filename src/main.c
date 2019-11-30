#include <stdio.h>
#include <stdlib.h>
#include "caminhos.h"

void clearScreen(char* mensagem){
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";

    char ch;
    printf("%s", mensagem);
    scanf("%c",&ch);
    getchar();

    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

int fileexists(const char * filename){
    /* try to open file to read */
    FILE *file;
    if ((file = fopen(filename, "r")) != NULL){
        fclose(file);
        return 1;
    }
    return 0;
}

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
    //busca_largura(0, "arquivos/2.txt", "arquivos/tabela_largura2.txt");
    //print_caminho("arquivos/tabela_largura2.txt", 0, 9);

    //OP 1 e 2
    char caminho[2*TAM];
    int matriz_adj[TAM][TAM];
    int tipo, tamanho;
    FILE *f;

    //MENU
    int option;
    char ch;
    char *mensagem;
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);

    inicio:
    printf("\n      | MENU |\n\n");
    printf(" 1 - Carregar Grafo \n");
    printf(" 2 - Salvar Grafo \n");
    printf(" 3 - Mostrar Matriz de Adjacencia do grafo carregado\n");
    printf(" 4 - Modificar Grafo \n");
    printf(" 5 - Busca em Profundidade \n");
    printf(" 6 - Busca em Largura \n");
    printf(" 7 - Aplicar Dijkstra \n");
    printf(" 8 - Aplicar Bellman-Ford \n");
    printf(" 0 - Sair \n");


    printf("\nPor favor escolha uma das opções acima: ");
    scanf(" %d", &option);
    printf("\n");
    switch (option) {
        case 0:
            break;


        case 1:
        {
            printf("\n\nDigite o caminho do arquivo do seu grafo: ");
            scanf("%s", caminho);
            //strcpy(caminho, "arquivos/8.grafo");
            printf("\n");

            f = fopen(caminho, "r");

            if(f != NULL){
                inicializar_matriz(matriz_adj);
                le_matriz_arquivo(f, matriz_adj, &tipo, &tamanho);

                mensagem = "Parabens, grafo carregado com sucesso, pressione ENTER para retornar ao menu...";
                fclose(f);
                clearScreen(mensagem);
                goto inicio;

            }

            mensagem = "\nNão foi possivel encontrar esse arquivo, por favor, pressione ENTER e tente novamente...\n";
            clearScreen(mensagem);

            goto inicio;

        }

        case 2:
        {
            print_matriz(matriz_adj, tamanho);
            printf("\nO GRAFO QUE SERÁ SALVO, É O ULTIMO CARREGADO PELO USUÁRIO, CASO NENHUM TENHA SIDO CARREGADO\nO RESULTADO SERÁ UM ARQUIVO EM BRANCO\n");
            printf("\n\nDigite o caminho do arquivo para salvar o seu grafo: ");
            scanf("%s", caminho);
            if(fileexists(caminho)){
                int flag;
                printf("Esse arquivo já existe, deseja sobreescrever? \n");
                printf("1 - Sim\n");
                printf("2 - Nao\n");
                scanf("%d", &flag);
                switch (flag) {
                    case 1:
                    {
                        f = fopen(caminho, "w+");
                        escreve_matriz_arquivo(f, matriz_adj, tipo, tamanho);
                        fclose(f);
                        mensagem = "Arquivo sobrescrito com sucesso!\n";
                        clearScreen(mensagem);
                        goto inicio;
                    }

                    case 2:
                    {
                        mensagem = "Por favor, escolha entao outro nome de arquivo... Pressione ENTER para continuar...\n";
                        clearScreen(mensagem);
                    }

                    default:
                        goto inicio;
                }

            }
            f = fopen(caminho, "w+");
            escreve_matriz_arquivo(f, matriz_adj, tipo, tamanho);
            fclose(f);
            mensagem = "Arquivo criado com sucesso! Pressione ENTER para continuar...\n";
            clearScreen(mensagem);
            goto inicio;
        }

        case 3:
        {
            printf("MATRIZ DO GRAFO CARREGADO:\n\n");
            print_matriz(matriz_adj, tamanho);
            mensagem = "Pressione ENTER para voltar ao MENU...";
            clearScreen(mensagem);
            goto inicio;
        }
        case 4:
        {
            int linha, coluna, peso;
            print_matriz(matriz_adj, tamanho);
            printf("Digite a linha e a coluna que deseja modificar na matriz, por favor, ambos tem que ser < %d\n", tamanho);
            printf("LINHA: ");
            scanf("%d", &linha);
            printf("COLUNA: ");
            scanf("%d", &coluna);
            printf("Digite agora o peso dessa aresta que sera criada: ");
            scanf("%d", &peso);

            matriz_adj[linha][coluna] = peso;
            printf("Modificação feita com sucesso!\n\n");
            print_matriz(matriz_adj, tamanho);

            mensagem = "\nPressione ENTER para voltar ao MENU...";
            clearScreen(mensagem);
            goto inicio;
        }

        case 5:
        {
            int flag;
            char entrada[2*TAM];
            char saida[2*TAM];
            int raiz;
            printf("Por favor digite o caminho do arquivo de grafo no qual sera feita a busca em profundidade: ");
            scanf("%s", entrada);
            printf("\nPor favor, agora digite o caminho do arquivo onde será salvo o resultado da busca: ");
            scanf("%s", saida);
            if(fileexists(saida)){
                int flag;
                printf("O arquivo de saida já existe, deseja sobreescrever? \n");
                printf("1 - Sim\n");
                printf("2 - Nao\n");
                scanf("%d", &flag);
                switch (flag) {
                    case 1:
                    {
                        printf("\nDigite a raiz para essa busca: ");
                        scanf("%d", &raiz);
                        busca_profundidade(raiz, entrada, saida);
                        mensagem = "Busca feita com sucesso!\n";
                        clearScreen(mensagem);
                        goto inicio;
                    }

                    case 2:
                    {
                        mensagem = "\nPor favor, escolha entao outro nome de arquivo de saida... Pressione ENTER para voltar ao MENU...\n";
                        clearScreen(mensagem);
                    }

                    default:
                        goto inicio;
                }

            }
            printf("\nDigite a raiz para essa busca: ");
            scanf("%d", &raiz);
            busca_profundidade(raiz, entrada, saida);
            mensagem = "Busca feita com sucesso!\n";
            clearScreen(mensagem);
            goto inicio;

        }

        case 6:
        {
            int flag;
            char entrada[2*TAM];
            char saida[2*TAM];
            int raiz;
            printf("Por favor digite o caminho do arquivo de grafo no qual sera feita a busca em largura: ");
            scanf("%s", entrada);
            printf("\nPor favor, agora digite o caminho do arquivo onde será salvo o resultado da busca: ");
            scanf("%s", saida);
            if(fileexists(saida)){
                int flag;
                printf("O arquivo de saida já existe, deseja sobreescrever? \n");
                printf("1 - Sim\n");
                printf("2 - Nao\n");
                scanf("%d", &flag);
                switch (flag) {
                    case 1:
                    {
                        printf("\nDigite a raiz para essa busca: ");
                        scanf("%d", &raiz);
                        busca_largura(raiz, entrada, saida);
                        mensagem = "Busca feita com sucesso!\n";
                        clearScreen(mensagem);
                        goto inicio;
                    }

                    case 2:
                    {
                        mensagem = "\nPor favor, escolha entao outro nome de arquivo de saida... Pressione ENTER para voltar ao MENU...\n";
                        clearScreen(mensagem);
                    }

                    default:
                        goto inicio;
                }

            }
            printf("\nDigite a raiz para essa busca: ");
            scanf("%d", &raiz);
            busca_largura(raiz, entrada, saida);
            mensagem = "Busca feita com sucesso!\n";
            clearScreen(mensagem);
            goto inicio;
        }


        case 7:
        {
            printf("O algoritmo de Dijkstra sera aplicado no grafo previamente carregado pela opção '1' do MENU\n Caso nenhum grafo tenha sido carregado, o resultado pode ser indesejado.\n");
            printf("\nAqui está o grafo carregado: \n\n");
            print_matriz(matriz_adj, tamanho);

            int op_dij;
            printf("Deseja aplicar o algoritmo nesse grafo? \n");
            printf("1 - SIM\n");
            printf("2 - NAO\n");
            printf("Escolha: ");
            scanf("%d", &op_dij);
            printf("\n");
            switch (op_dij) {
                case 1:
                {
                    int raiz_dijkstra;

                    printf("Escolha uma raiz para iniciarmos o algoritmo: ");
                    scanf("%d", &raiz_dijkstra);

                    char destino_dijkstra[2*TAM];
                    printf("Digite o caminho do arquivo para salvar os resultados: ");
                    scanf("%s", destino_dijkstra);


                    dijkstra(matriz_adj, raiz_dijkstra, tamanho, destino_dijkstra);

                    mensagem = "\nPressione ENTER para voltar ao MENU...";
                    clearScreen(mensagem);
                    goto inicio;
                }

                case 2:
                {
                    mensagem = "\nPor favor, carregue entao um outro grafo antes de tentar novamente... Pressione ENTER para voltar ao MENU...\n";
                    clearScreen(mensagem);
                }

                default:
                    goto inicio;
            }


        }

        case 8:
        {
            printf("O algoritmo de Bellman-Ford sera aplicado no grafo previamente carregado pela opção '1' do MENU\n Caso nenhum grafo tenha sido carregado, o resultado pode ser indesejado.\n");
            printf("\nAqui está o grafo carregado: \n\n");
            print_matriz(matriz_adj, tamanho);

            int op_bel;
            printf("Deseja aplicar o algoritmo nesse grafo? \n");
            printf("1 - SIM\n");
            printf("2 - NAO\n");
            printf("Escolha: ");
            scanf("%d", &op_bel);
            printf("\n");
            switch (op_bel) {
                case 1:
                {
                    int raiz_bellman;
                    int ciclo_peso_neg;
                    printf("Escolha uma raiz para iniciarmos o algoritmo: ");
                    scanf("%d", &raiz_bellman);

                    char destino_bellman[2*TAM];
                    printf("Digite o caminho do arquivo para salvar os resultados: ");
                    scanf("%s", destino_bellman);
                    //strcpy(destino_bellman, "arquivos/8.bellman");
                    ciclo_peso_neg = bellman_ford(matriz_adj, raiz_bellman, tamanho, tipo, destino_bellman);

                    if(ciclo_peso_neg){
                        printf("\nO grafo inserido tem um ciclo de peso negativo\n");
                    }
                    mensagem = "\nPressione ENTER para voltar ao MENU...";
                    clearScreen(mensagem);
                    goto inicio;
                }

                case 2:
                {
                    mensagem = "\nPor favor, carregue entao um outro grafo antes de tentar novamente... Pressione ENTER para voltar ao MENU...\n";
                    clearScreen(mensagem);
                }

                default:
                    goto inicio;
            }


        }

        default:
        {
            mensagem = "\nPor favor, escolha uma opcao valida...\n";
            clearScreen(mensagem);
            goto inicio;
        }
    }
    return 0;
}
