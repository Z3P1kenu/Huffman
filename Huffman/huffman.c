#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TAM 256

typedef struct arvore{
    void* rep;
    void* chr;
    struct arvore *esq, *dir;
}arvore;

typedef struct lista{
    void* tam;
    struct arvore *raiz;
    struct lista *prox, *ant;
}lista;


arvore* criar_arvore(unsigned char v, arvore *esquerda, arvore *direita); //Cria uma arvore
lista* get_tail(lista *head); //retorna o ultimo item da lista
lista* remover_inicio(lista **head); //remove o primeiro item da lista
int altura(arvore *raiz); //Retorna o caminho da raiz até a folha mais distante
int tamanho_arvore(arvore *raiz); //Retorna o tamanho da árvore
char** alocar_dicionario(int profundidade); //Aloca mémoria para fazer o dicionario
void addlist(lista **head, unsigned char v, int r); //Adiciona item na lista
void arvore_de_huffman(lista **head); //Controi a árvore de Huffman
void compactar(const char *nomedoarquivo, const char *novoarquivo, lista **list); //Compacta o arquivo
void descompactar(const char *nomedoarquivo, const char *novoarquivo); //Descompacta o arquivo
void criar_dicionario(char **dicionario, arvore *raiz, char *caminho, int profundidade); //Cria o dicionario
void escrever_arvore(FILE *arquivo, arvore *raiz, int *tam); //Escreve a árvore em preordem no cabeçalho
void escrever_metadados(FILE *arquivo, uint16_t arv_tam, uint8_t padding); //Escreve o cabeçalho com o trash e o tamanho da arvore
void ler_metadados(FILE *arquivo, uint16_t *tam, uint8_t *trash); //Ler os 2 primeiros bytes do arquivo
void swap(lista *a, lista *b); //Troca os ponteiros de lugar
uint16_t swap_uint16(uint16_t val); //Resolve o edianess fazendo ele escrever o byte mais significativo primeiro

//Funções do quick_sort
lista* partition(lista *low, lista *high);
void quick_sort(lista *low, lista *high);
void quick_sort_linked_list(lista** head);


int main() {
    int op; //Variavel para escolher se compacta ou descompacta
    char nomedoarquivo[TAM]; //Array com o nome do arquivo existente
    char novoarquivo[TAM]; //Array com o nome do arquivo que vai ser criado
    lista *list = NULL;
    FILE *file;

    do{ //Switch case 1 comapacta, 2 descompacta, 3 encerra
        printf("Digite 1 para compactar um arquivo, 2 para descompactar, ou 3 para encerrar o programa.\n");
        scanf("%d", &op);
        
        while (getchar() != '\n'); //Limpa o buffer para escrever o nomedoarquivo
        
        switch (op){
        case 1:
            
            //Receber o nome do arquivo que vai ser compactado
            printf("Insira o nome do arquivo (Ex: imagem.png, Aula_20_-_Huffman.pdf):");
            fgets(nomedoarquivo, TAM, stdin);
            nomedoarquivo[strcspn(nomedoarquivo, "\n")] = '\0'; //Remove a quebra de linha da string

            //Recebe o nome para o novo arquivo que vai ser criado
            printf("Insira o nome para o novo arquivo compactado (SEM A EXTENSAO):");
            fgets(novoarquivo, TAM, stdin);
            novoarquivo[strcspn(novoarquivo, "\n")] = '.'; //Troca a quebra de linha por um .
            strcat(novoarquivo, "watts"); //Contatena "watts" na string

            compactar(nomedoarquivo, novoarquivo, &list); //Chama a função de compactação

            //Verifica se o arquivo novo foi criado
            file = fopen(novoarquivo, "r"); //Abre o novo arquivo em modo de leitura

            if(file != NULL){ //Caso ele consiga ler o arquivo, então ele existe
                printf("Arquivo %s criado\n", novoarquivo);
            }
            else{ //Caso não, ele não consegue
                printf("Arquivo %s nao foi criado\n\n", novoarquivo);
            }
            break;

        case 2:

            //Recebe o nome do arquivo existente
            printf("Insira o nome do arquivo compactado (Com a extensao):");
            fgets(nomedoarquivo, TAM, stdin);
            nomedoarquivo[strcspn(nomedoarquivo, "\n")] = '\0';

            //Recebe o nome do arquivo que vai ser criado
            printf("Insira o nome para o novo arquivo com extensao (Ex: imagem.png, Aula_20_-_Huffman.pdf):");
            fgets(novoarquivo, TAM, stdin);
            novoarquivo[strcspn(novoarquivo, "\n")] = '\0';

            descompactar(nomedoarquivo, novoarquivo); //Chama a função de descompactar
            
            //Verifica se o arquivo novo foi criado
            file = fopen(novoarquivo, "r"); //Abre o novo arquivo em modo de leitura

            if(file != NULL){ //Caso ele consiga ler o arquivo, então ele existe
                printf("Arquivo %s criado\n", novoarquivo);
            }
            else{ //Caso não, ele não consegue
                printf("Arquivo %s nao foi criado\n\n", novoarquivo);
            }
            break;

        case 3:
            //Encerra o programa
            break;

        default:
            printf("Entrada invalida. Por favor, digite 1 ou 2 ou 3.\n");
            break;
        }

    }while(op != 3);

    return 0;
}

uint16_t swap_uint16(uint16_t val) {
    return (val << 8) | (val >> 8); //Desloca um byte pra esquerda e um pra direita e faz uma operacão de OU para trazer o byte mais significativo para frente
}

void swap(lista *a, lista *b){ //Troca os ponteiros de lugar
    int aux_chr = *((unsigned char*)a->raiz->chr); //Variavel auxiliar para guardar o char de a
    int aux_rep = *((int*)a->raiz->rep); //Variavel auxiliar para guardar as repetições de a
    arvore *aux_esq = a->raiz->esq; //Variavel auxiliar para guardar o filho da esquerda de a
    arvore *aux_dir = a->raiz->dir; //Variavel auxiliar para guardar o filho da direita de a
    
    *((unsigned char*)a->raiz->chr) = *((unsigned char*)b->raiz->chr);
    *((int*)a->raiz->rep) = *((int*)b->raiz->rep);
    a->raiz->esq = b->raiz->esq;
    a->raiz->dir = b->raiz->dir;
    
    *((unsigned char*)b->raiz->chr) = aux_chr;
    *((int*)b->raiz->rep) = aux_rep;
    b->raiz->esq = aux_esq;
    b->raiz->dir = aux_dir;
}

void escrever_metadados(FILE *arquivo, uint16_t arv_tam, uint8_t padding){
    uint16_t metadados = ((padding << 13) | arv_tam); //Desloca os bits do padding para o inicio do byte e junta com o tamanho da árvore
    metadados = swap_uint16(metadados); //Troca o byte de maior significado para frente
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
    *tam = metadados & 0x1FFF;
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
        strcpy(dicionario[*(unsigned char*)raiz->chr], caminho);
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
    if(raiz->dir == NULL && raiz->esq == NULL){ //
        if((*((unsigned char*)raiz->chr) == '*' || *((unsigned char*)raiz->chr) == '\\')){
            unsigned char barra = '\\';
            fwrite(&barra, sizeof(unsigned char), 1, arquivo);
            (*tam)++;
            fwrite((unsigned char*)raiz->chr, sizeof(unsigned char), 1, arquivo);
        }
        else{
            fwrite(((unsigned char*)raiz->chr), sizeof(unsigned char), 1, arquivo); //Escreve o char da raiz no cabeçalho
        }
    }
    else{ //
        fwrite(((unsigned char*)raiz->chr), sizeof(unsigned char), 1, arquivo); //Escreve o char da raiz no cabeçalho

        escrever_arvore(arquivo, raiz->esq, tam); //Chama a função recursivamente. Chama o filho da esquerda primeiro para ser escrido em preordem
        escrever_arvore(arquivo, raiz->dir, tam);
    }
}

arvore* ler_arvore(unsigned char *buffer, int tamanho){
    static int posicao = 0;
    
    if (posicao >= tamanho) {
        return NULL;
    }

    arvore *no = criar_arvore('\0', NULL, NULL);
    if (buffer[posicao] == '*'){
        posicao++;
        no->chr = malloc(sizeof(unsigned char));
        *(unsigned char*)no->chr = '*';
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
        no->chr = malloc(sizeof(unsigned char));
        *(unsigned char*)no->chr = buffer[posicao];
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

    new->chr = malloc(sizeof(unsigned char));

    *(unsigned char*)new->chr = v;

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

    if(*head == NULL){ //Se nao tem item na lista ele cria um
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
    unsigned char bit_no_buffer = 0;
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
        char *code = dicionario[buffer[i]];
        for(long j = 0; code[j]; j++){
            //porcent = (((i + 1)*100)/filesize);
            //printf("\t\t%d%%\n",porcent);
            if(code[j] == '1') {
                bit_no_buffer |= (1 << (7 - bit_count));
            }
            bit_count++;
            total_bits++;

            if(bit_count == 8){
                fwrite(&bit_no_buffer, 1, 1, new_file);
                bit_no_buffer = 0;
                bit_count = 0;
            }
        }
    }

    // Escrever os bits restantes, se houver
    if(bit_count > 0){
        fwrite(&bit_no_buffer, 1, 1, new_file);
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

void descompactar(const char *nomedoarquivo, const char *novoarquivo){ //Ainda vamos mudar isso
    FILE *file;
    FILE *new_file;
    arvore *aux;
    arvore *huff = NULL;
    size_t current;
    uint8_t trash;
    uint16_t tamanho;
    unsigned char byte;

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

    aux = huff;
    int porcent;
    for(long i = tamanho + 2; i < filesize; i++){
        byte = buffer[i];
        if(i < filesize - 1){
            for (int i = 7; i >= 0; i--){
                if ((byte & (1 << i)) != 0){
                    aux = aux->dir;
                } else {
                    aux = aux->esq;
                }
                
                if (aux->esq == NULL && aux->dir == NULL){
                    fwrite(aux->chr, 1, 1, new_file);
                    aux = huff; 
                }
            }
        }
        else{
            for (int i = 7; i >= trash; i--){
                if ((byte & (1 << i)) != 0){
                    aux = aux->dir;
                } else{
                    aux = aux->esq;
                }
                
                if (aux->esq == NULL && aux->dir == NULL){
                    fwrite(aux->chr, 1, 1, new_file);
                    aux = huff; 
                }
            }
        }
    }
}