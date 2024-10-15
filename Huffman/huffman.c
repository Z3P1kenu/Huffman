#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TAM 256

typedef struct arvore{
    void* rep;
    void* valor;
    struct arvore *esq, *dir;
}arvore;

typedef struct lista{
    void* tam;
    struct arvore *raiz;
    struct lista *prox, *ant;
}lista;

void swap(lista *a, lista *b);//Troca os ponteiros de lugar
arvore* criar_arvore(unsigned char v, arvore *esquerda, arvore *direita);//Cria uma arvore
lista* remover_inicio(lista **head);//remove o primeiro item da lista
void addlist(lista **head, unsigned char v);//Adiciona item na lista
void print_arvore(arvore *a, int altura);//printa a arvore (TBM ACHO QUE NAO VAI PRECISAR MAIS PRA FRENTE)
void print_lista(lista *l); //printa os itens da lista (ACHO QUE NAO VAI PRECISAR DISSO MAIS PRA FRENTE)
void compactar(const char *nomedoarquivo, const char *novoarquivo, lista **list);
void arvore_de_huffman(lista **head);//Controi a árvore de Huffman
int altura(arvore *raiz);//Retorna o caminho da raiz até a folha mais distante
int tamanho_arvore(arvore *raiz);//Retorna o tamanho da árvore
char** alocar_dicionario(int profundidade);//Aloca mémoria para fazer o dicionario
void criar_dicionario(char **dicionario, arvore *raiz, char *caminho, int profundidade);//Cria o dicionario
uint16_t swap_uint16(uint16_t val);//Resolve o edianess fazendo ele escrever o byte mais significativo primeiro
void escrever_metadados(FILE *arquivo, uint16_t arv_tam, uint8_t padding);//Escreve o cabeçalho com o trash e o tamanho da arvore
void escrever_arvore(FILE *arquivo, arvore *raiz);//Escreve a árvore em preordem no cabeçalho
void ler_metadados(FILE *arquivo); //AINDA EM PRODUÇÃO
lista* get_tail(lista *head); //retorna o ultimo item da lista
lista* partition(lista *low, lista *high);
void quick_sort_linked_list(lista *low, lista *high);

int main() {
    int op;
    char nomedoarquivo[TAM];
    char tipo[TAM/4];
    char novoarquivo[TAM];
    lista *list = NULL;

    do{
        printf("Digite 1 para compactar um arquivo e 2 para descompactar.\n");
        if (scanf("%d", &op) != 1) {
            printf("Entrada inválida. Por favor, digite 1 ou 2.\n");
            while (getchar() != '\n'); // Limpa o buffer de entrada
            continue;
        }
        if (op < 1 || op > 2) {
            printf("Número inválido.\n");
        }
    }while(op<1 || op>2);

    switch (op)
    {
    case 1:
        
        printf("Insira o nome do arquivo:");
        scanf("%s", nomedoarquivo);
        //printf("Tipo do novo arquivo");
        //scanf("%s", tipo);

        //strcat(nomedoarquivo, tipo);

        printf("Insira o nome do arquivo compactado:");
        scanf("%s", novoarquivo);
        strcat(novoarquivo, ".watts");

        compactar(nomedoarquivo, novoarquivo, &list);

        break;
    case 2:

        printf("Insira o nome do arquivo compactado:");
        scanf("%s", nomedoarquivo);
        strcat(novoarquivo, ".watts");

        printf("Insira o nome do arquivo:");
        scanf("%s", novoarquivo);
        printf("Tipo do novo arquivo");
        scanf("%s", tipo);

        strcat(novoarquivo, tipo);

        //descompactar(nomedoarquivo, novoarquivo, &list);

        break;
    default:
        break;
    }

    return 0;
}

void quick_sort_linked_list(lista *low, lista *high) {
    if (high != NULL && low != high && low != high->prox) {
        lista *pivot = partition(low, high);
        quick_sort_linked_list(low, pivot->ant);
        quick_sort_linked_list(pivot->prox, high);
    }
}

lista* partition(lista *low, lista *high) {
    int pivot = *(int*)(high->raiz->rep);
    lista *i = low->ant;
    for (lista *j = low; j != high; j = j->prox) {
        if (*(int*)(j->raiz->rep) <= pivot) {
            if (i == NULL) {
                i = low;
            } else {
                i = i->prox;
            }
            swap(i, j);
        }
    }
    if (i == NULL) {
        i = low;
    } else {
        i = i->prox;
    }
    swap(i, high);
    return i;
}

lista* get_tail(lista *head) { //Retorna o ultimo item da lista
    lista *aux = head;
    while (aux->prox != NULL) {
        aux = aux->prox;
    }
    return aux;
}

void swap(lista *a, lista *b){ //Troca os ponteiros de lugar
    arvore *aux = a->raiz;
    a->raiz = b->raiz;
    b->raiz = aux;
}

void insertion_sort_list(lista *head) { //insert_sort para listas encadeadas
    if (head == NULL) return; // Verifica se a lista está vazia
    lista *i, *j;
    for (i = head; i->prox != NULL; i = i->prox) {
        for (j = i->prox; j != NULL; j = j->prox) {
            if (*((int*)i->raiz->rep) > *((int*)j->raiz->rep)) {
                swap(i, j); // Troca os valores
            }
        }
    }
}

arvore* criar_arvore(unsigned char v, arvore *esquerda, arvore *direita){ //Cria uma arvore
    arvore *new = (arvore*) malloc(sizeof(arvore));

    new->dir = direita;
    new->esq = esquerda;

    new->valor = malloc(sizeof(unsigned char));
    new->rep = malloc(sizeof(int));

    *(unsigned char*)new->valor = v;
    *(int*)new->rep = 1;

    if(esquerda != NULL && direita != NULL){ //(ACHO QUE DÁ PRA FAZER ISSO MELHOR MAS N TENHO CERTEZA)
        *((int*)new->rep) = *((int*)esquerda->rep) + *((int*)direita->rep);
    }
    else if(esquerda != NULL){
        *((int*)new->rep) = *((int*)esquerda->rep);
    }
    else if(direita != NULL){
        *((int*)new->rep) = *((int*)direita->rep);
    }
    return new;
}

lista* remover_inicio(lista **head){ //remove o primeiro item da lista
    if(*head == NULL){
        return NULL; //caso não tenha item ele retorna NULL
    }

    lista *aux = *head;
    *head = (*head)->prox;
    //(*head)->ant = NULL;
    aux->prox = NULL; //aponta o proximo pra NULL pra não dar problema de acesso de memoria

    if(*head != NULL){
        (*head)->tam = aux->tam - 1;
    }

    return aux;
}

void compactar(const char *nomedoarquivo, const char *novoarquivo, lista **list){ //Ainda vamos mudar isso
    FILE *file;
    FILE *new_file;
    size_t current;
    int profundidade;
    char **dicionario;
    lista *tail;
    char *codificado;
    uint16_t tamanho;
    uint8_t trash;
    int bit_no_buffer = 0;
    unsigned buffer_saida = 0;

    //Leitura do arquivo

    file = fopen(nomedoarquivo, "rb"); //Abre o arquivo que vai ser compactado
    if(file == NULL){ //Conferir se foi aberto corretamente
        perror("Erro ao ler o arquivo: ");
        return;
    }

    fseek(file, 0, SEEK_END); //Salta o ponteiro para o final do arquivo
    long filesize = ftell(file); //Quarda a posição atual do arquivo que no caso é o final logo quarda o tamanho do arquivo
    rewind(file); //Retorna para o começo do arquivo

    unsigned char *buffer = (unsigned char*) malloc(filesize); //Aloca mémoria do buffer
    if(buffer == NULL){ //Tratamento de erro
        perror("Erro ao alocar mémoria: ");
        free(buffer);
        fclose(file);
        return;
    }

    current = fread(buffer, 1, filesize, file); //Lê o conteudo do arquivo e salva no buffer
    if(current != filesize){ //Tratamento de erro
        perror("Erro ao ler o arquivo: ");
        free(buffer);
        fclose(file);
        return;
    }

    for(long i=0; i < filesize; i++){ //Adiciona o conteudo do arquivo na lista
        addlist(list, buffer[i]);
    }

    //----------------------------
    //Compactação

    if(*list == NULL){
        perror("Erro ao encotrar a lista");
        free(buffer);
    }

    tail = get_tail(*list);
    insertion_sort_list(*list);

    arvore_de_huffman(list);

    profundidade = altura((*list)->raiz);

    dicionario = alocar_dicionario(profundidade);
    criar_dicionario(dicionario, (*list)->raiz, "", profundidade);

    codificado = calloc(filesize * 8 + 1, sizeof(unsigned char));
    for(long i = 0; i < filesize; i++){
        strcat(codificado, dicionario[buffer[i]]);
    }

    tamanho = tamanho_arvore((*list)->raiz) + 1;
    trash = 8 - (strlen(codificado) % 8);

    printf("%d %d", trash, tamanho);

    //----------------------------
    //Escrevendo o arquivo

    new_file = fopen(novoarquivo, "wb"); //Abre o novo arquivo em modo de escrida
    if(new_file == NULL){ //Tratamento de erro
        perror("Erro ao escrever o arquivo: ");
        free(buffer);
        fclose(new_file);
        free(codificado);
        for (int i = 0; i < TAM; i++) {
            free(dicionario[i]);
        }
        free(dicionario);
        return;
    }

    escrever_metadados(new_file, tamanho, trash); //Escreve o cabeçalho

    escrever_arvore(new_file, (*list)->raiz); //Escreve a árvore

    for(long i = 0; i < strlen(codificado); i++){ //Escreve o código novo
        buffer_saida = (buffer_saida << 1) | (codificado[i] - '0'); //Desloca o buffer para a esquerda e seta o novo bit
        bit_no_buffer++;

        if(bit_no_buffer == 8){ //Quando tem um byte escreve no arquivo novo
            fwrite(&buffer_saida, sizeof(unsigned char), 1, new_file);
            buffer_saida = 0;
            bit_no_buffer = 0;
        }
    }

    if(bit_no_buffer > 0){ //Escreve os bits que não conseguiram formar um byte com as casas que faltaram 0
        buffer_saida <<= (8 - bit_no_buffer);
        fwrite(&buffer_saida, sizeof(unsigned char), 1, new_file);
    }

    free(buffer);
    fclose(new_file);
    fclose(file);
    free(codificado);
    for (int i = 0; i < TAM; i++) {
        free(dicionario[i]);
    }
    free(dicionario);
    
}

void arvore_de_huffman(lista **head){ //Pega os dois primeiros itens da lista e cria uma uma lista binaria com eles
    lista *primeiro, *segundo, *tail;
    while((*((int*)(*head)->tam)) > 1){
        primeiro = remover_inicio(head); //Pega o primeiro item da lista
        //Como o primeiro item foi removido da lista o segundo vira o primeiro
        segundo = remover_inicio(head); //Pega o primeiro item da lista que era o segundo

        lista *new = (lista*) malloc(sizeof(lista)); //Aloca mémoria para o novo item da lista


        new->raiz = criar_arvore('*', primeiro->raiz, segundo->raiz); //Cria uma arvore que tem o primeiro e segundo item como folhas

        if (*head != NULL) {
            *((int*)new->tam) = *((int*)(*head)->tam) + 1; 
        } 
        else {
            *((int*)new->tam) = 1;
        }//Caso a lista não esteja vazia o novo tamanho é igual ao da cabeça + 1, caso contrario o tamanho é 1

        new->prox = *head;
        *head = new;

        free(primeiro); //Free para evitar memory leak
        free(segundo);

        tail = get_tail(*head);
        insertion_sort_list(*head); //insere a arvore na lista e ordena ela
    }
}

void addlist(lista **head, unsigned char v){ //Adiciona item na lista
    lista *aux, *new = (lista*) malloc(sizeof(lista));
    if(new == NULL){
        perror("Erro ao alocar memoria para lista");
        return;
    }

    new->raiz = criar_arvore(v, NULL, NULL);
    
    new->tam = malloc(sizeof(int));
    if(new->tam == NULL){
        perror("Erro ao alocar memoria para tamanho");
        free(new->raiz);
        free(new);
        return;
    }

    int tamanho = *((int*)new->tam);

    if(*head == NULL){ //Se nao tem item na lista ele cria um
        new->prox = NULL;
        //new->ant = NULL;
        *head = new;
        tamanho = 1;
    }
    else{
        aux = *head;
        while(aux->prox != NULL && *((unsigned char*)aux->raiz->valor) != v){ //Caminha a lista até o final ou até encontrar um item igual
            aux = aux->prox;
        }
        if(*((unsigned char*)aux->raiz->valor) == v){ //caso encontre um item igual ele incrementa a repetição
            (*(int*)aux->raiz->rep)++;
            free(new->raiz->valor);
            free(new->raiz->rep);
            free(new->tam);
            free(new->raiz); //como o item já existe não precisamos mais do new ent free pra nao dar problema
            free(new);
        }
        else if(aux->prox == NULL){
            aux->prox = new;
            new->prox = NULL;
            //new->ant = aux;
            tamanho++;
        }
    }
}

void print_arvore(arvore *a, int altura){
    if(a != NULL){
        //printf(" P:%p", a);
        putchar('(');
        printf("I:%C R:%d A:%d", *((unsigned char*)a->valor), *((int*)a->rep), altura);
        altura++;
        print_arvore(a->esq, altura);
        print_arvore(a->dir, altura);
        putchar(')');
    }
    else{
        printf("()");
    }
}

void print_lista(lista *l){
    int i = 0;
    while(l != NULL){
        print_arvore(l->raiz, i);
        putchar('\n');
        l = l->prox;
    }
}

void criar_dicionario(char **dicionario, arvore *raiz, char *caminho, int profundidade){ //Cria o novo dicionario a parti da árvore de Huffman
    char esquerda[profundidade], direita[profundidade];
    
    if(raiz->dir == NULL && raiz->esq == NULL){ //Caso o nó seja uma folha ele adicona ao dicionario o novo valor para o valor da folha Ex: A = 01 ao inves de A = 0100 0001
        strcpy(dicionario[*(unsigned char*)raiz->valor], caminho);
    }
    else{
        strcpy(esquerda, caminho); //Copia a string do caminho para a string da esquerda
        strcpy(direita, caminho); //Copia a string do caminho para a string da direita

        strcat(esquerda, "0"); //Caso a siga para a esquerda adiciona 0
        strcat(direita, "1"); //Caso direita adiciona 1

        criar_dicionario(dicionario, raiz->esq, esquerda, profundidade); //Chamada recursiva
        criar_dicionario(dicionario, raiz->dir, direita, profundidade);
    }
}

char** alocar_dicionario(int profundidade){ //Aloca mémoria para o dicionario baseado na profundidade da árvore já que é isso que diz quantos bits o novo valor vai ter
    char **dicionario;
    int i;
    dicionario = malloc(sizeof(char*) * TAM);

    for(i = 0; i < TAM; i++){
        dicionario[i] = calloc(profundidade, sizeof(char));
    }

    return dicionario;
}

int altura(arvore *raiz){ //Retorna a altura da ávore
    int altura_da_esquerda, altura_da_direita;
    if(raiz == NULL){
        return -1;
    }
    else{
        altura_da_esquerda = altura(raiz->esq) + 1;
        altura_da_direita = altura(raiz->dir) + 1;
    }
    if(altura_da_direita > altura_da_esquerda){ //Decide qual é a maior altura e a retorna
        return altura_da_direita;
    }
    else{
        return altura_da_esquerda;
    }
}

int tamanho_arvore(arvore *raiz){ //Retorna o tamanho da árvore
    int tamanho_esq, tamanho_dir;
    if(raiz == NULL){
        return -1;
    }
    else{
        tamanho_esq = tamanho_arvore(raiz->esq) + 1;
        tamanho_dir = tamanho_arvore(raiz->dir) + 1;
    }
    return tamanho_dir + tamanho_esq;
}

uint16_t swap_uint16(uint16_t val) {
    return (val << 8) | (val >> 8); //Desloca um byte pra esquerda e um pra direita para trazer o byte mais significativo para frente
}

void escrever_metadados(FILE *arquivo, uint16_t arv_tam, uint8_t padding){
    uint16_t metadados = ((padding << 13) | arv_tam); //Desloca os bits do padding para o inicio do byte e junta com o tamanho da árvore
    metadados = swap_uint16(metadados); //Troca o byte de maior significado para frente
    fwrite(&metadados, sizeof(uint16_t), 1, arquivo); //Escreve o cabeçalho no arquivo
}

void escrever_arvore(FILE *arquivo, arvore *raiz){
    unsigned char folha; //Variavel para saber se o nó é uma folha ou não

    fwrite(&(raiz->valor), sizeof(unsigned char), 1, arquivo); //Escreve o valor da raiz no cabeçalho

    if(raiz->dir == NULL && raiz->esq == NULL){ //Caso o nó seja uma folha ele escreve um byte 1 após o valor (Não deve ser o jeito mais eficiente já que adiciona um byte inteiro oq custa mais mémoria)
        folha = 1;
        fwrite(&folha, sizeof(unsigned char), 1, arquivo);
    }
    else{ //Caso não ele escreve um byte 0
        folha = 0;
        fwrite(&folha, sizeof(unsigned char), 1, arquivo);

        escrever_arvore(arquivo, raiz->esq); //Chama a função recursivamente. Chama o filho da esquerda primeiro para ser escrido em preordem
        escrever_arvore(arquivo, raiz->dir);
    }
}

void ler_metadados(FILE *arquivo){
    uint16_t metadados;
    fread(&metadados, sizeof(uint16_t), 1, arquivo);
    uint8_t metapa = metadados >> 13;
    uint16_t metar = metadados & 0x1FFF;
}