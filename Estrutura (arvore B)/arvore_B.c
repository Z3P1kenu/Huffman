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



// Função auxiliar para encontrar o índice da primeira chave maior ou igual a k
int findKey(BTreeNode *node, int k) {
    int idx = 0;
    while (idx < node->count && node->keys[idx] < k)
        idx++;
    return idx;
}

// Função de busca na árvore B
BTreeNode* search(BTreeNode *root, int k) {
    int i = findKey(root, k);
    
    // Se encontrou a chave no nó atual
    if (i < root->count && root->keys[i] == k)
        return root;
        
    // Se é uma folha e não encontrou, a chave não existe
    if (root->leaf)
        return NULL;
        
    // Recursivamente busca no filho apropriado
    return search(root->children[i], k);
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

    //abre espaço no nó pai para o novo filho movendo todos os outros para a direita a partir da posição i+1
    for (int j = parent->count; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    //conecta o novo nó na posição i+1
    parent->children[i + 1] = newNode;

    //abrindo espaço no nó pai para todas as chaves
    //move todas as chaves para uma posição a direita a partir da posição i
    for (int j = parent->count - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    //a chave do nó filho (posição MIN - 1) sobre para o pai (vira um dos filhos do pai)
    parent->keys[i] = child->keys[MIN - 1];
    //aumenta o número de chaves no nó pai
    parent->count++;
}

//função de inserir quando o nodo do respectivo número não está cheio
void insertNonFull(BTreeNode *node, int key) {
    //o int i é a quantidade de chaves - 1 pois queremos utilizar nos arrays
    int i = node->count - 1;

    //se o nodo for uma folha nós apenas colocamos ele na posição certa
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

        //se o filho estiver cheio, realiza o split antes de inserir
        if (node->children[i]->count == MAX) {
            splitChild(node, i, node->children[i]);

            //após o split mostra em qual filho a chave deve ser inserida
            if (key > node->keys[i]) {
                i++;
            }
        }

        //insere o filho de maneira recursiva
        insertNonFull(node->children[i], key);
    }
}

void insert(BTreeNode **root, int key) {
    BTreeNode *r = *root;

    //se a raiz estiver cheia, cria uma nova raiz
    if (r->count == MAX) {
        BTreeNode *newRoot = createNode(0);
        newRoot->children[0] = r;
        //divide a raiz antiga e insere a chave
        splitChild(newRoot, 0, r);
        insertNonFull(newRoot, key);
        *root = newRoot;
    } else {
        //se a raiz não estiver cheia insere diretamente
        insertNonFull(r, key);
    }
}

//função que printa em ordem (comicamente foi a que mais deu trabalho)
void traverse(BTreeNode *root) {

    int i;

    //para cada chave do nó
    for (i = 0; i < root->count; i++) {
        if (!root->leaf) {
            //se não for folha, visita o filho à esquerda
            traverse(root->children[i]);
        }
        //printa o filho atual
        printf("%d ", root->keys[i]);
    }

    //se não for folha visita o filho mais a direita
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

    printf("Travessia da árvore B:\n"); //só pra ser bonitinho
    traverse(root);
    printf("\n");

    return 0;
}
