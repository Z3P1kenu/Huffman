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


arvore* criar_arvore(unsigned char v, arvore *esquerda, arvore *direita);//Cria uma arvore
lista* get_tail(lista *head); //retorna o ultimo item da lista
lista* remover_inicio(lista **head);//remove o primeiro item da lista
int altura(arvore *raiz);//Retorna o caminho da raiz até a folha mais distante
int tamanho_arvore(arvore *raiz);//Retorna o tamanho da árvore
char** alocar_dicionario(int profundidade);//Aloca mémoria para fazer o dicionario
void addlist(lista **head, unsigned char v, int r);//Adiciona item na lista
void arvore_de_huffman(lista **head);//Controi a árvore de Huffman
void compactar(const char *nomedoarquivo, const char *novoarquivo, lista **list);
void descompactar(const char *nomedoarquivo, const char *novoarquivo, lista **list);
void criar_dicionario(char **dicionario, arvore *raiz, char *caminho, int profundidade);//Cria o dicionario
void escrever_arvore(FILE *arquivo, arvore *raiz, int *tam);//Escreve a árvore em preordem no cabeçalho
void escrever_metadados(FILE *arquivo, uint16_t arv_tam, uint8_t padding);//Escreve o cabeçalho com o trash e o tamanho da arvore
//void ler_metadados(FILE *arquivo, uint16_t *tam, uint8_t *trash); //AINDA EM PRODUÇÃO
void print_arvore(arvore *a, int altura);//printa a arvore (TBM ACHO QUE NAO VAI PRECISAR MAIS PRA FRENTE)
void print_lista(lista *l); //printa os itens da lista (ACHO QUE NAO VAI PRECISAR DISSO MAIS PRA FRENTE)
void swap(lista *a, lista *b);//Troca os ponteiros de lugar
uint16_t swap_uint16(uint16_t val);//Resolve o edianess fazendo ele escrever o byte mais significativo primeiro
lista* partition(lista *low, lista *high);
void quick_sort(lista *low, lista *high);
void quick_sort_linked_list(lista** head);


int main() {
    int op;
    char nomedoarquivo[TAM];
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

    while (getchar() != '\n');

    switch (op)
    {
    case 1:
        
        printf("Insira o nome do arquivo (Ex: imagem.png, Aula_20_-_Huffman.pdf):");
        fgets(nomedoarquivo, TAM, stdin);
        nomedoarquivo[strcspn(nomedoarquivo, "\n")] = '\0';

        printf("Insira o nome para o novo arquivo compactado:");
        fgets(novoarquivo, TAM, stdin);
        novoarquivo[strcspn(novoarquivo, "\n")] = '.';
        strcat(novoarquivo, "watts");
        
        

        compactar(nomedoarquivo, novoarquivo, &list);

        printf("Arquivo %s criado", novoarquivo);
        break;
    case 2:

        //Fazendo ainda
        printf("Insira o nome do arquivo compactado:");
        fgets(nomedoarquivo, TAM, stdin);
        nomedoarquivo[strcspn(nomedoarquivo, "\n")] = '\0';

        printf("Insira o nome para o novo arquivo com extensão (Ex: imagem.png, Aula_20_-_Huffman.pdf):");
        fgets(novoarquivo, TAM, stdin);
        novoarquivo[strcspn(novoarquivo, "\n")] = '\0';

        descompactar(nomedoarquivo, novoarquivo, &list);

        printf("Arquivo %s criado", novoarquivo);

        break;
    default:
        break;
    }
    
    return 0;
}

uint16_t swap_uint16(uint16_t val) {
    return (val << 8) | (val >> 8); //Desloca um byte pra esquerda e um pra direita para trazer o byte mais significativo para frente
}

void swap(lista *a, lista *b){ //Troca os ponteiros de lugar
    void* aux_valor = a->raiz->valor;
    void* aux_rep = a->raiz->rep;
    arvore *aux_esq = a->raiz->esq;
    arvore *aux_dir = a->raiz->dir;
    
    a->raiz->valor = b->raiz->valor;
    a->raiz->rep = b->raiz->rep;
    a->raiz->esq = b->raiz->esq;
    a->raiz->dir = b->raiz->dir;
    
    b->raiz->valor = aux_valor;
    b->raiz->rep = aux_rep;
    b->raiz->esq = aux_esq;
    b->raiz->dir = aux_dir;
}

void escrever_metadados(FILE *arquivo, uint16_t arv_tam, uint8_t padding){
    //printf("\t%d %d\n", padding, arv_tam);
    uint16_t metadados = ((padding << 13) | arv_tam); //Desloca os bits do padding para o inicio do byte e junta com o tamanho da árvore
    metadados = swap_uint16(metadados); //Troca o byte de maior significado para frente
    //printf("\t%X\n", metadados);
    fwrite(&metadados, sizeof(uint16_t), 1, arquivo); //Escreve o cabeçalho no arquivo
}

lista* get_tail(lista *head) {
    lista *aux = head;
    while (aux != NULL && aux->prox != NULL) {
        aux = aux->prox;
    }
    return aux;
}

void quick_sort_linked_list(lista** head) {
    if (*head == NULL || (*head)->prox == NULL)
        return;

    lista* last = get_tail(*head);
    quick_sort(*head, last);
}

void quick_sort(lista *low, lista *high) {
    if (high != NULL && low != high && low != high->prox) {
        lista *pivot = partition(low, high);
        quick_sort(low, pivot->ant);
        quick_sort(pivot->prox, high);
    }
}

void ler_metadados(FILE *arquivo, uint16_t *tam, uint8_t *trash){
    uint16_t metadados;
    fread(&metadados, sizeof(uint16_t), 1, arquivo);
    *trash = (uint8_t)(metadados >> 13);
    //printf("%d\n", trash);
    *tam = metadados & 0x1FFF;
    //printf("%d\n", tam);
}


void print_lista(lista *l){
    int i = 0;
    while(l != NULL){
        print_arvore(l->raiz, i);
        putchar('\n');
        l = l->prox;
    }
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

char** alocar_dicionario(int profundidade){ //Aloca mémoria para o dicionario baseado na profundidade da árvore já que é isso que diz quantos bits o novo valor vai ter
    char **dicionario;
    int i;
    dicionario = malloc(sizeof(char*) * TAM);

    for(i = 0; i < TAM; i++){
        dicionario[i] = calloc(profundidade, sizeof(char));
    }

    return dicionario;
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

void print_arvore(arvore *a, int altura){
    if(a != NULL){
        //printf(" P:%p", a);
        putchar('(');
        printf("I:%C R:%d A:%d ", *((unsigned char*)a->valor), *((int*)a->rep), altura);
        altura++;
        printf("E:");
        print_arvore(a->esq, altura);
        printf("DIR:");
        print_arvore(a->dir, altura);
        putchar(')');
    }
    else{
        printf("()");
    }
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
        *((int*)(*head)->tam) = *((int*)aux->tam) - 1;
    }

    return aux;
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

void escrever_arvore(FILE *arquivo, arvore *raiz, int *tam){
    //printf("%c\n", *((unsigned char*)raiz->valor));
    if(raiz->dir == NULL && raiz->esq == NULL){ //
        if((*((unsigned char*)raiz->valor) == '*' || *((unsigned char*)raiz->valor) == '\\')){
            unsigned char barra = '\\';
            fwrite(&barra, sizeof(unsigned char), 1, arquivo);
            (*tam)++;
            fwrite((unsigned char*)raiz->valor, sizeof(unsigned char), 1, arquivo);
        }
        else{
            fwrite(((unsigned char*)raiz->valor), sizeof(unsigned char), 1, arquivo); //Escreve o valor da raiz no cabeçalho
        }
    }
    else{ //
        fwrite(((unsigned char*)raiz->valor), sizeof(unsigned char), 1, arquivo); //Escreve o valor da raiz no cabeçalho

        escrever_arvore(arquivo, raiz->esq, tam); //Chama a função recursivamente. Chama o filho da esquerda primeiro para ser escrido em preordem
        escrever_arvore(arquivo, raiz->dir, tam);
    }
}

arvore* ler_arvore(unsigned char *buffer, int tamanho){
    static int posicao = 0;
    //printf("%c\n", buffer[posicao]);
    
    if (posicao >= tamanho) {
        return NULL;
    }

    arvore *no = criar_arvore('\0', NULL, NULL);
    if (buffer[posicao] == '*'){
        posicao++;
        no->valor = malloc(sizeof(unsigned char));
        *(unsigned char*)no->valor = '*';
        no->esq = ler_arvore(buffer, tamanho);
        no->dir = ler_arvore(buffer, tamanho);
    } 
    else{
        if (buffer[posicao] == '\\'){
            posicao++;
            if (posicao >= tamanho){
                free(no);
                return NULL;
            }
        }
        no->valor = malloc(sizeof(unsigned char));
        *(unsigned char*)no->valor = buffer[posicao];
        posicao++;
    }

    return no;
}

lista* partition(lista *low, lista *high) {
    int pivot = *(int*)(high->raiz->rep);
    lista *i = low->ant;
    
    for (lista *j = low; j != high; j = j->prox) {
        if (*(int*)(j->raiz->rep) <= pivot) {
            if (i == NULL)
                i = low;
            else
                i = i->prox;
            swap(i, j);
        }
    }
    
    if (i == NULL)
        i = low;
    else
        i = i->prox;
    swap(i, high);
    return i;
}

arvore* criar_arvore(unsigned char v, arvore *esquerda, arvore *direita){ //Cria uma arvore
    arvore *new = (arvore*) malloc(sizeof(arvore));

    new->dir = direita;
    new->esq = esquerda;

    new->valor = malloc(sizeof(unsigned char));

    *(unsigned char*)new->valor = v;

    return new;
}

void addlist(lista **head, unsigned char v, int r){ //Adiciona item na lista
    lista *aux, *new = (lista*) malloc(sizeof(lista));
    if(new == NULL){
        perror("Erro ao alocar memoria para lista");
        return;
    }

    new->raiz = criar_arvore(v, NULL, NULL);

    new->raiz->rep = malloc(sizeof(int));

    *((int*)new->raiz->rep) = r;
    
    new->tam = malloc(sizeof(int));

    new->prox = *head;
    new->ant = NULL;

    if(*head == NULL){
        *((int*)new->tam) = 1;
    }
    else{
        *((int*)new->tam) = *((int*)(*head)->tam) + 1;
    }

    *head = new;


}

void arvore_de_huffman(lista **head){ //Pega os dois primeiros itens da lista e cria uma uma lista binaria com eles
    lista *primeiro, *segundo;
    

    while((*((int*)(*head)->tam)) > 1){
        //printf("\t%d\n", *((int*)(*head)->tam));
        primeiro = remover_inicio(head); //Pega o primeiro item da lista
        //Como o primeiro item foi removido da lista o segundo vira o primeiro
        segundo = remover_inicio(head); //Pega o primeiro item da lista que era o segundo

        lista *new = (lista*) malloc(sizeof(lista)); //Aloca mémoria para o novo item da lista

        new->tam = malloc(sizeof(int));

        new->raiz = criar_arvore('*', primeiro->raiz, segundo->raiz); //Cria uma arvore que tem o primeiro e segundo item como folhas

        new->raiz->rep = malloc(sizeof(int));
        *((int*)new->raiz->rep) = *((int*)primeiro->raiz->rep) + *((int*)segundo->raiz->rep);

        if (*head != NULL) {
            *((int*)new->tam) = *((int*)(*head)->tam) + 1; 
        } 
        else {
            *((int*)new->tam) = 1;
        }//Caso a lista não esteja vazia o novo tamanho é igual ao da cabeça + 1, caso contrario o tamanho é 1


        new->prox = *head;
        *head = new;

        free(primeiro->tam); //Free para evitar memory leak
        free(segundo->tam);
        free(primeiro); //Free para evitar memory leak
        free(segundo);

        quick_sort_linked_list(head);//insere a arvore na lista e ordena ela
        //print_lista(*head);
    }
}

void compactar(const char *nomedoarquivo, const char *novoarquivo, lista **list){ //Ainda vamos mudar isso
    int profundidade;
    char **dicionario;
    char *codificado;
    long filesize;
    FILE *file, *new_file;
    size_t current;
    uint8_t trash;
    uint16_t tamanho;
    unsigned char bit_buffer = 0;
    int bit_count = 0;
    long total_bits = 0;

    //Leitura do arquivo

    file = fopen(nomedoarquivo, "rb"); //Abre o arquivo que vai ser compactado
    if(file == NULL){ //Conferir se foi aberto corretamente
        perror("Erro ao ler o arquivo: ");
        return;
    }

    fseek(file, 0, SEEK_END); //Salta o ponteiro para o final do arquivo
    filesize = ftell(file); //Quarda a posição atual do arquivo que no caso é o final logo quarda o tamanho do arquivo
    rewind(file); //Retorna para o começo do arquivo

    unsigned char *buffer = (unsigned char*) malloc(filesize); //Aloca mémoria do buffer
    if(buffer == NULL){ //Tratamento de erro
        perror("Erro ao alocar mémoria: ");
        free(buffer);
        fclose(file);
        return;
    }

     //Lê o conteudo do arquivo e salva no buffer

    current = fread(buffer, 1, filesize, file);
    if(current != filesize){
        perror("Erro ao ler o arquivo: ");
        free(buffer);
        fclose(file);
        return;
    }

    unsigned int repeticoes[TAM] = {0};

    for(long i=0; i < filesize; i++){ //Adiciona o conteudo do arquivo na lista
        repeticoes[buffer[i]]++;
    }

    for(int i = 0; i < TAM; i++){
        if(repeticoes[i] > 0){
            addlist(list, i, repeticoes[i]);
        }
    }

    if(*list == NULL){
        perror("Falha ao criar a lista");
        free(buffer);
        fclose(file);
        return;
    }

    //----------------------------
    //Compactação

    

    quick_sort_linked_list(list);


    arvore_de_huffman(list);


    profundidade = altura((*list)->raiz);



    dicionario = alocar_dicionario(profundidade);
    criar_dicionario(dicionario, (*list)->raiz, "", profundidade);

    //----------------------------
    //Escrevendo o arquivo

    new_file = fopen(novoarquivo, "wb");
    if(new_file == NULL) {
        perror("Erro ao escrever o arquivo");
        free(buffer);
        for(int i = 0; i < TAM; i++) {
            free(dicionario[i]);
        }
        free(dicionario);
        fclose(file);
        return;
    }

    //Reservar espaço para os metadados
    fseek(new_file, sizeof(uint16_t), SEEK_SET);

    //print_lista(*list);
    int sobra = 0;
    escrever_arvore(new_file, (*list)->raiz, &sobra);
    //printf("%d", sobra);

    
    int porcent;
    for(long i = 0; i < filesize; i++){
        const char *code = dicionario[buffer[i]];
        for(long j = 0; code[j]; j++){
            //porcent = (((i + 1)*100)/filesize);
            //printf("\t\t%d%%\n",porcent);
            if(code[j] == '1') {
                bit_buffer |= (1 << (7 - bit_count));
            }
            bit_count++;
            total_bits++;

            if(bit_count == 8){
                fwrite(&bit_buffer, 1, 1, new_file);
                bit_buffer = 0;
                bit_count = 0;
            }
        }
    }

    // Escrever os bits restantes, se houver
    if(bit_count > 0){
        fwrite(&bit_buffer, 1, 1, new_file);
    }

    // Calcular e escrever os metadados
    tamanho = tamanho_arvore((*list)->raiz) + 1 + sobra;
    trash = (8 - (total_bits % 8)) % 8;
    uint16_t metadados = (trash << 13) | tamanho;
    metadados = swap_uint16(metadados);

    fseek(new_file, 0, SEEK_SET);
    fwrite(&metadados, sizeof(uint16_t), 1, new_file);

    fclose(new_file);
    free(buffer);
    fclose(file);
    for(int i = 0; i < TAM; i++){
        free(dicionario[i]);
    }
    free(dicionario);
}

void descompactar(const char *nomedoarquivo, const char *novoarquivo, lista **list){ //Ainda vamos mudar isso
    char **dicionario;
    char *codificado;
    FILE *file;
    FILE *new_file;
    arvore *aux;
    arvore *huff = NULL;
    size_t current;
    uint8_t trash;
    uint16_t tamanho;

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

    current = fread(buffer, 1, filesize, file);
    if(current != filesize){
        perror("Erro ao ler o arquivo: ");
        free(buffer);
        fclose(file);
        return;
    }

    uint16_t metadados = (uint16_t) (buffer[0] << 8) | buffer[1];
    trash = (buffer[0] >> 5);
    tamanho = metadados & 0x1FFF;

    huff = ler_arvore(buffer + 2, tamanho);

    //print_arvore(huff, 0);
    //putchar('\n');

    new_file = fopen(novoarquivo, "wb");
    if (new_file == NULL) {
        perror("Error opening output file");
        fclose(new_file);
        return;
    }


    arvore *aux = huff;
    unsigned char byte;
    
    int porcent;
    for(long i = tamanho + 2; i < filesize; i++){
        byte = buffer[i];
        //porcent = (((i + 1)*100)/filesize);
        //printf("\t\t%d%%\n",porcent);
        //printf("ENTROU\n");
        if(i < filesize - 1){
            for (int i = 7; i >= 0; i--){
                if ((byte & (1 << i)) != 0){
                    //printf("\tDIR\n");
                    aux = aux->dir;
                } else {
                    //printf("\tESQ\n");
                    aux = aux->esq;
                }
                
                if (aux->esq == NULL && aux->dir == NULL){
                    //printf("FOIA\n");
                    //printf("%c", *(unsigned char*)aux->valor);
                    fwrite(aux->valor, 1, 1, new_file);
                    aux = huff; 
                }
            }
        }
        else{
            for (int i = 7; i >= trash; i--){
                if ((byte & (1 << i)) != 0){
                    //printf("\tDIR\n");
                    aux = aux->dir;
                } else{
                    //printf("\tESQ\n");
                    aux = aux->esq;
                }
                
                if (aux->esq == NULL && aux->dir == NULL){
                    //printf("FOIA\n");
                    //printf("%c", *(unsigned char*)aux->valor);
                    fwrite(aux->valor, 1, 1, new_file);
                    aux = huff; 
                }
            }
        }
    }
}