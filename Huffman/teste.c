#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TAM 256

typedef struct arvore{
    int rep;
    unsigned char valor;
    struct arvore *esq, *dir;
}arvore;

typedef struct lista{
    int tam;
    struct arvore *raiz;
    struct lista *prox;
}lista;

void swap(lista *a, lista *b){
    arvore *aux = a->raiz;
    a->raiz = b->raiz;
    b->raiz = aux;
}

void insertion_sort_list(lista *head) { //insert_sort para listas encadeadas
    if (head == NULL) return; // Verifica se a lista está vazia
    lista *i, *j;
    for (i = head; i->prox != NULL; i = i->prox) {
        for (j = i->prox; j != NULL; j = j->prox) {
            if (i->raiz->rep > j->raiz->rep) {
                swap(i, j); // Troca os valores
            }
        }
    }
}

arvore* criar_arvore(unsigned char v, arvore *esquerda, arvore *direita){
    arvore *new = (arvore*) malloc(sizeof(arvore));
    new->dir = direita;
    new->esq = esquerda;
    if(esquerda != NULL && direita != NULL){
        new->rep = esquerda->rep + direita->rep;
    }
    else if(esquerda != NULL){
        new->rep = esquerda->rep;
    }
    else if(direita != NULL){
        new->rep = direita->rep;
    }
    else{
        new->rep = 1;
    }
    new->valor = v;

    return new;
}

lista* remover_inicio(lista **head){
    if(*head == NULL) return NULL;

    lista *aux = *head;
    *head = (*head)->prox;
    aux->prox = NULL;

    if(*head != NULL){
        (*head)->tam = aux->tam - 1;
    }

    return aux;
}

int tamanho_arvore(arvore *raiz){
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

void addlist(lista **head, unsigned char v){
    lista *aux, *new = (lista*) malloc(sizeof(lista));
    new->raiz = criar_arvore(v, NULL, NULL);
    if(*head == NULL){
        new->prox = NULL;
        *head = new;
        (*head)->tam = 1;
    }
    else{
        aux = *head;
        while(aux->prox != NULL && aux->raiz->valor != v){
            aux = aux->prox;
        }
        if(aux->raiz->valor == v){
            aux->raiz->rep++;
            free(new->raiz);
            free(new);
        }
        else if(aux->prox == NULL){
            aux->prox = new;
            new->prox = NULL;
            (*head)->tam++;
        }
    }
}

void print_arvore(arvore *a, int altura){
    if(a != NULL){
        //printf(" P:%p", a);
        putchar('(');
        printf("I:%C R:%d A:%d", a->valor, a->rep, altura);
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

void arvore_de_huffman(lista **head){
    printf("%d", (*head)->tam);
    lista *primeiro, *segundo;
    while((*head)->tam > 1){
        printf("\t%d\n", (*head)->tam);
        primeiro = remover_inicio(head);
        segundo = remover_inicio(head);

        lista *new = (lista*) malloc(sizeof(lista));

        new->raiz = criar_arvore('*', primeiro->raiz, segundo->raiz);
        new->tam = ((*head) != NULL) ? (*head)->tam + 1 : 1;
        new->prox = *head;
        *head = new;

        free(primeiro);
        free(segundo);

        insertion_sort_list(*head);
    }
}

int altura(arvore *raiz){
    int altura_da_esquerda, altura_da_direita;
    if(raiz == NULL){
        return -1;
    }
    else{
        altura_da_esquerda = altura(raiz->esq) + 1;
        altura_da_direita = altura(raiz->dir) + 1;
    }
    if(altura_da_direita > altura_da_esquerda){
        return altura_da_direita;
    }
    else{
        return altura_da_esquerda;
    }
}

char** alocar_dicionario(int profundidade){
    char **dicionario;
    int i;
    dicionario = malloc(sizeof(char*) * TAM);

    for(i = 0; i < TAM; i++){
        dicionario[i] = calloc(profundidade, sizeof(char));
    }

    return dicionario;
}

void criar_dicionario(char **dicionario, arvore *raiz, char *caminho, int profundidade){
    char esquerda[profundidade], direita[profundidade];
    
    if(raiz->dir == NULL && raiz->esq == NULL){
        strcpy(dicionario[raiz->valor], caminho);
    }
    else{
        strcpy(esquerda, caminho);
        strcpy(direita, caminho);

        strcat(esquerda, "0");
        strcat(direita, "1");

        criar_dicionario(dicionario, raiz->esq, esquerda, profundidade);
        criar_dicionario(dicionario, raiz->dir, direita, profundidade);
    }
}

uint16_t swap_uint16(uint16_t val) {
    return (val << 8) | (val >> 8);
}

void escrever_metadados(FILE *arquivo, uint16_t arv_tam, uint8_t padding){
    uint16_t metadados = ((padding << 13) | arv_tam);
    metadados = swap_uint16(metadados);
    printf("%x %x %x\n", metadados, padding, arv_tam);
    fwrite(&metadados, sizeof(uint16_t), 1, arquivo);
}

void escrever_arvore(FILE *arquivo, arvore *raiz){
    unsigned char folha;

    if(raiz->dir == NULL && raiz->esq == NULL){
        fwrite(&raiz->valor, sizeof(unsigned char), 1, arquivo);
    }
    else{
        fwrite(&raiz->valor, sizeof(unsigned char), 1, arquivo);

        escrever_arvore(arquivo, raiz->esq);
        escrever_arvore(arquivo, raiz->dir);
    }
}

void ler_metadados(FILE *arquivo){
    uint16_t metadados;
    fread(&metadados, sizeof(uint16_t), 1, arquivo);
    uint8_t metapa = metadados >> 13;
    uint16_t metar = metadados & 0x1FFF;
    printf("%x %x %x\n", metadados, metapa, metar);
}

//FEITO POR IA
void analisar_dicionario(char **dicionario) {
    int min_len = 100;
    int max_len = 0;
    float media_len = 0;
    int count = 0;
    
    for(int i = 0; i < TAM; i++) {
        int len = strlen(dicionario[i]);
        if(len > 0) {
            if(len < min_len) min_len = len;
            if(len > max_len) max_len = len;
            media_len += len;
            count++;
            printf("Caractere: %c (0x%02X) -> Bits: %d, Cdigo: %s\n", 
                   i, i, len, dicionario[i]);
        }
    }
    
    if(count > 0) {
        media_len /= count;
        printf("\nEstatisticas do dicionario:\n");
        printf("Menor codigo: %d bits\n", min_len);
        printf("Maior codigo: %d bits\n", max_len);
        printf("Media: %.2f bits\n", media_len);
        printf("Total de caracteres diferentes: %d\n", count);
    }
}

void comprimir(const char *nomedoarquivo, const char *novoarquivo, lista **list){
    FILE *file;
    FILE *new_file;
    size_t current;
    int profundidade;
    char **dicionario;
    char *codificado;

    file = fopen(nomedoarquivo, "rb");
    if(file == NULL){
        perror("Erro ao ler o arquivo: ");
        return;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    unsigned char *buffer = (unsigned char*) malloc(filesize);
    if(buffer == NULL){
        perror("Erro ao alocar mémoria: ");
        free(buffer);
        fclose(file);
        return;
    }

    current = fread(buffer, 1, filesize, file);
    if(current != filesize){
        perror("Erro ao ler o arquivo: ");
        free(buffer);
        fclose(file);
        return;
    }

    for(long i=0; i < filesize; i++){
        addlist(list, buffer[i]);
    }


    //---------------------------

    insertion_sort_list(*list);
    arvore_de_huffman(list);
    profundidade = altura((*list)->raiz);
    dicionario = alocar_dicionario(profundidade);
    criar_dicionario(dicionario, (*list)->raiz, "", profundidade);
    //analisar_dicionario(dicionario);
    codificado = calloc(filesize * 8 + 1, sizeof(char));
    for(long i = 0; i < filesize; i++){
        strcat(codificado, dicionario[buffer[i]]);
    }
    uint16_t tamanho = tamanho_arvore((*list)->raiz) + 1;
    uint8_t padding = 8 - (strlen(codificado) % 8);
    printf("%d\n%d\n%d\n", strlen(codificado), tamanho, padding);
    //printf("%s\n\n", codificado);

    //----------------------------


    new_file = fopen(novoarquivo, "wb");
    if(new_file == NULL){
        perror("Erro ao escrever o arquivo: ");
        free(buffer);
        free(codificado);
        for (int i = 0; i < TAM; i++) {
            free(dicionario[i]);
        }
        free(dicionario);
        return;
    }

    escrever_metadados(new_file, tamanho, padding);

    escrever_arvore(new_file, (*list)->raiz);

    int bit_no_buffer = 0;
    unsigned buffer_saida = 0;

    for(long i = 0; i < strlen(codificado); i++){
        buffer_saida = (buffer_saida << 1) | (codificado[i] - '0');
        bit_no_buffer++;

        if(bit_no_buffer == 8){
            fwrite(&buffer_saida, sizeof(unsigned char), 1, new_file);
            buffer_saida = 0;
            bit_no_buffer = 0;
        }
    }

    if(bit_no_buffer > 0){
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

int main() {
    int op;
    char nomedoarquivo[TAM];
    char tipo[TAM/4];
    char novoarquivo[TAM];
    lista *list = NULL;
        
    printf("Insira o nome do arquivo:");
    scanf("%s", nomedoarquivo);
    //printf("Tipo do novo arquivo");
    //scanf("%s", tipo);

    //strcat(nomedoarquivo, tipo);

    printf("Insira o nome do arquivo compactado:");
    scanf("%s", novoarquivo);
    //strcat(novoarquivo, ".watts");

    comprimir(nomedoarquivo, novoarquivo, &list);

    //print_lista(list);

    return 0;
}
