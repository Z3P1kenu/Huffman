#include <stdio.h>
#include <stdlib.h>

#define MAX 3 //ordem da árvore B (máximo de chaves por nó)
#define MIN 2 //mínimo de chaves por nó

typedef struct BTreeNode {
    int keys[MAX]; //elementos de cada nó (também podem ser chamados de chaves)
    struct BTreeNode *children[MAX + 1]; //pode ter no MAX + 1 chaves pois podem existir números maiores que a maior chave da página
    int count; //número atual de chaves no nó
    int leaf;  //1 se for folha, 0 caso contrário
} BTreeNode;

BTreeNode *createNode(int leaf) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));//alocação de memoria para o nodo
    node->leaf = leaf;//ser for 1 é folha, se for 0 não é
    node->count = 0;//número atual de folhas de cada página
    for (int i = 0; i < MAX + 1; i++) {
        node->children[i] = NULL; //criação dos intervalos entre cada chave
    }
    return node;
}

//função pra dividir um filho se o nó estiver cheio durante a inserção
void splitChild(BTreeNode *parent, int i, BTreeNode *child) {

    //cria um novo nó que vai receber metade das chaves do nó filho
    BTreeNode *newNode = createNode(child->leaf);
    //define o número de chaves que o nó terá (MIN - 1 pois o array conta 0 como um número)
    newNode->count = MIN - 1;

    //copiar as últimas chaves de child para o novo nó
    //nesse caso, copia 1 chave já que o min - 1 é 2
    for (int j = 0; j < MIN - 1; j++) {
        newNode->keys[j] = child->keys[j + MIN];
    }

    //se o nó não for folha, precisamos copiar seus filhos
    if (!child->leaf) {

        //copia os últimos MIN filhos do nó filho para o novo nó
        //nesse caso copia o filho 1 e o filho 2
        for (int j = 0; j < MIN; j++) {
            newNode->children[j] = child->children[j + MIN];
        }
    }

    //agora que copiamos tudo, reduzimos o número de filhos do nó para min - 1 de novp
    child->count = MIN - 1;

    //abre espaço no nó pai para o novo filho
    for (int j = parent->count; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    //move todos os filhos uma posição para a direita a partir da posição de i+1
    parent->children[i + 1] = newNode;

    //mover as chaves de parent para a direita para abrir espaço para a chave mediana
    for (int j = parent->count - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    //colocar a chave mediana de child em parent
    parent->keys[i] = child->keys[MIN - 1];
    parent->count++;
}



//comentar mais do código a partir daqui



void insertNonFull(BTreeNode *node, int key) {
    int i = node->count - 1;

    if (node->leaf) {
        //inserir a nova chave na posição correta
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->count++;
    } else {
        //encontrar o filho que deverá receber a nova chave
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;

        if (node->children[i]->count == MAX) {
            splitChild(node, i, node->children[i]);

            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

void insert(BTreeNode **root, int key) {
    BTreeNode *r = *root;

    if (r->count == MAX) {
        BTreeNode *newRoot = createNode(0);
        newRoot->children[0] = r;
        splitChild(newRoot, 0, r);
        insertNonFull(newRoot, key);
        *root = newRoot;
    } else {
        insertNonFull(r, key);
    }
}

void traverse(BTreeNode *root) {
    int i;
    for (i = 0; i < root->count; i++) {
        if (!root->leaf) {
            traverse(root->children[i]);
        }
        printf("%d ", root->keys[i]);
    }
    if (!root->leaf) {
        traverse(root->children[i]);    
    }
}

int main() {
    BTreeNode *root = createNode(1); //criar a árvore vazia

    //exemplo de árvore para demonstrar na apresentação adicionando mais um número
    insert(&root, 10);
    insert(&root, 20);
    insert(&root, 5);
    insert(&root, 6);
    insert(&root, 12);
    insert(&root, 30);
    insert(&root, 7);
    insert(&root, 17);

    printf("Travessia da árvore B:\n");
    traverse(root);
    printf("\n");

    return 0;
}
