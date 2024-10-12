#include <stdio.h>
#include <stdlib.h>

#define MAX 3 // Ordem da árvore B (máximo de chaves por nó)
#define MIN 2 // Mínimo de chaves por nó

typedef struct BTreeNode {
    int keys[MAX];
    struct BTreeNode *children[MAX + 1];
    int count; // Número atual de chaves no nó
    int leaf;  // 1 se for folha, 0 caso contrário
} BTreeNode;

BTreeNode *createNode(int leaf) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    node->leaf = leaf;
    node->count = 0;
    for (int i = 0; i < MAX + 1; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void splitChild(BTreeNode *parent, int i, BTreeNode *child) {
    BTreeNode *newNode = createNode(child->leaf);
    newNode->count = MIN - 1;

    // Copiar as últimas chaves de child para newNode
    for (int j = 0; j < MIN - 1; j++) {
        newNode->keys[j] = child->keys[j + MIN];
    }

    // Se child não for folha, copiar os últimos filhos de child para newNode
    if (!child->leaf) {
        for (int j = 0; j < MIN; j++) {
            newNode->children[j] = child->children[j + MIN];
        }
    }

    child->count = MIN - 1;

    // Mover os filhos de parent para a direita para abrir espaço para o novo filho
    for (int j = parent->count; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    // Colocar newNode como filho de parent
    parent->children[i + 1] = newNode;

    // Mover as chaves de parent para a direita para abrir espaço para a chave mediana
    for (int j = parent->count - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    // Colocar a chave mediana de child em parent
    parent->keys[i] = child->keys[MIN - 1];
    parent->count++;
}

void insertNonFull(BTreeNode *node, int key) {
    int i = node->count - 1;

    if (node->leaf) {
        // Inserir a nova chave na posição correta
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->count++;
    } else {
        // Encontrar o filho que deverá receber a nova chave
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
    BTreeNode *root = createNode(1); // Criar uma árvore B vazia

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
