#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void print_arvore(arvore *a){
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

void print_lista(lista *l){
    while(l != NULL){
        print_arvore(l->raiz);
        putchar('\n');
        l = l->prox;
    }
}

void criararq(const char *nomedoarquivo, const char *novoarquivo, lista **list){
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

void arvore_de_huffman(lista **head){
    lista *primeiro, *segundo;
    while((*head)->tam > 1){
        primeiro = remover_inicio(head);
        segundo = remover_inicio(head);

        lista *new = (lista*) malloc(sizeof(lista));

        new->raiz = criar_arvore('\\', primeiro->raiz, segundo->raiz);
        new->tam = ((*head) != NULL) ? (*head)->tam + 1 : 1;
        new->prox = *head;
        *head = new;

        free(primeiro);
        free(segundo);

        insertion_sort_list(*head);
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
