#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arvore{
    void* rep;
    void* valor;
    struct arvore *esq, *dir;
}arvore;

typedef struct lista{
    void* tam;
    struct arvore *raiz;
    struct lista *prox;
}lista;

void swap(lista *a, lista *b){ //Troca os ponteiros de lugar
    arvore *aux = a->raiz;
    a->raiz = b->raiz;
    b->raiz = aux;
}

//(PROVISORIO ATÉ UM QUICK SORT APARECER DO CÉU)
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

arvore* criar_arvore(unsigned char v, arvore *esquerda, arvore *direita){ //Cria uma arvore
    arvore *new = (arvore*) malloc(sizeof(arvore));
    new->dir = direita;
    new->esq = esquerda;
    if(esquerda != NULL && direita != NULL){ //(ACHO QUE DÁ PRA FAZER ISSO MELHOR MAS N TENHO CERTEZA)
        new->rep = *((int*)esquerda->rep) + *((int*)direita->rep);
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

lista* remover_inicio(lista **head){ //remove o primeiro item da lista
    if(*head == NULL) return NULL; //caso não tenha item ele retorna NULL

    lista *aux = *head;
    *head = (*head)->prox;
    aux->prox = NULL; //aponta o proximo pra NULL pra não dar problema de acesso de memoria

    if(*head != NULL){
        (*head)->tam = aux->tam - 1;
    }

    return aux;
}

void addlist(lista **head, unsigned char v){ //Adiciona item na lista
    lista *aux, *new = (lista*) malloc(sizeof(lista));
    new->raiz = criar_arvore(v, NULL, NULL);
    if(*head == NULL){ //Se nao tem intem na lista ele cria um
        new->prox = NULL;
        *head = new;
        (*head)->tam = 1;
    }
    else{
        aux = *head;
        while(aux->prox != NULL && aux->raiz->valor != v){ //Caminha a lista até o final ou até encontrar um item igual
            aux = aux->prox;
        }
        if(aux->raiz->valor == v){ //caso encontre um item igual ele incrementa a repetição
            aux->raiz->rep++;
            free(new->raiz); //como o item já existe não precisamos mais do new ent free pra nao dar problema
            free(new);
        }
        else if(aux->prox == NULL){
            aux->prox = new;
            new->prox = NULL;
            (*head)->tam++;
        }
    }
}

void print_arvore(arvore *a){ //printa a arvore (TBM ACHO QUE NAO VAI PRECISAR MAIS PRA FRENTE)
    if(a != NULL){
        putchar('(');
        printf("I:%X R:%d", a->valor, a->rep);
        print_arvore(a->esq);
        print_arvore(a->dir);
        putchar(')');
    }
    else{
        printf("()");
    }
}

void print_lista(lista *l){ //printa os itens da lista (ACHO QUE NAO VAI PRECISAR DISSO MAIS PRA FRENTE)
    while(l != NULL){
        print_arvore(l->raiz);
        putchar('\n');
        l = l->prox;
    }
}

void criararq(const char *nomedoarquivo, const char *novoarquivo, lista **list){ //Ainda vamos mudar isso
    FILE *file;
    FILE *new_file;
    size_t current;

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

    new_file = fopen(novoarquivo, "wb");
    if(new_file == NULL){
        perror("Erro ao escrever o arquivo: ");
        return;
    }

    current = fwrite(buffer, 1, filesize, new_file);
    if(current != filesize){
        perror("Erro ao escrever o arquivo: ");
        free(buffer);
        fclose(new_file);
        return;
    }


    free(buffer);
    fclose(new_file);
    fclose(file);

}

void arvore_de_huffman(lista **head){ //Pega os dois primeiros itens da lista e cria uma uma lista binaria com eles
    lista *primeiro, *segundo;
    while((*head)->tam > 1){
        primeiro = remover_inicio(head); //Pega o primeiro item da lista
        //Como o primeiro item foi removido da lista o segundo vira o primeiro
        segundo = remover_inicio(head); //Pega o primeiro item da lista que era o segundo

        lista *new = (lista*) malloc(sizeof(lista));

        new->raiz = criar_arvore('\\', primeiro->raiz, segundo->raiz); //Cria uma arvore que tem o primeiro e segundo item como folhas
        new->tam = ((*head) != NULL) ? (*head)->tam + 1 : 1; 
        new->prox = *head;
        *head = new;

        free(primeiro); //Free para evitar memory leak
        free(segundo);

        insertion_sort_list(*head); //insere a arvore na lista e ordena ela
    }
}

int main() {
    int op;
    char nomedoarquivo[256];
    char tipo[64];
    char novoarquivo[256];
    lista *list = NULL;

    do{
        printf("Digite 1 para compactar um arquivo e 2 para descompactar.\n");
        scanf("%d", &op);
        if(op<1 || op>2){
            printf("Número invalido.\n");
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

        criararq(nomedoarquivo, novoarquivo, &list);

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

        criararq(nomedoarquivo, novoarquivo, &list);

        break;
    default:
        break;
    }

    arvore_de_huffman(&list);

    print_lista(list);

    return 0;
}
