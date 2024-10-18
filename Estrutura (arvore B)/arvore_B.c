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

// Função auxiliar para pegar o predecessor
int getPred(BTreeNode *node, int idx) {
    BTreeNode *curr = node->children[idx];
    while (!curr->leaf)
        curr = curr->children[curr->count];
    return curr->keys[curr->count - 1];
}

// Função auxiliar para pegar o sucessor
int getSucc(BTreeNode *node, int idx) {
    BTreeNode *curr = node->children[idx + 1];
    while (!curr->leaf)
        curr = curr->children[0];
    return curr->keys[0];
}

// Função auxiliar para preencher o filho na posição idx
void fillChild(BTreeNode *node, int idx) {
    // Tenta pegar uma chave do irmão à esquerda
    if (idx != 0 && node->children[idx-1]->count >= MIN) {
        BTreeNode *child = node->children[idx];
        BTreeNode *sibling = node->children[idx-1];
        
        // Move todas as chaves em child um passo à direita
        for (int i = child->count-1; i >= 0; --i)
            child->keys[i+1] = child->keys[i];
            
        // Se não for folha, move também os filhos
        if (!child->leaf) {
            for (int i = child->count; i >= 0; --i)
                child->children[i+1] = child->children[i];
        }
        
        // Copia a chave do pai para o child
        child->keys[0] = node->keys[idx-1];
        
        // Move a última chave do irmão para o pai
        node->keys[idx-1] = sibling->keys[sibling->count-1];
        
        // Move o último filho do irmão para child
        if (!child->leaf)
            child->children[0] = sibling->children[sibling->count];
            
        child->count += 1;
        sibling->count -= 1;
    }
    
    // Tenta pegar uma chave do irmão à direita
    else if (idx != node->count && node->children[idx+1]->count >= MIN) {
        BTreeNode *child = node->children[idx];
        BTreeNode *sibling = node->children[idx+1];
        
        // Copia a chave do pai para o child
        child->keys[child->count] = node->keys[idx];
        
        // Move a primeira chave do irmão para o pai
        node->keys[idx] = sibling->keys[0];
        
        // Se não for folha, move também o primeiro filho do irmão
        if (!child->leaf)
            child->children[child->count+1] = sibling->children[0];
            
        // Move todas as chaves do irmão um passo à esquerda
        for (int i = 1; i < sibling->count; ++i)
            sibling->keys[i-1] = sibling->keys[i];
            
        // Se não for folha, move também os filhos
        if (!sibling->leaf) {
            for (int i = 1; i <= sibling->count; ++i)
                sibling->children[i-1] = sibling->children[i];
        }
        
        child->count += 1;
        sibling->count -= 1;
    }
    
    // Merge com o irmão
    else {
        if (idx != node->count)
            merge(node, idx);
        else
            merge(node, idx-1);
    }
}

// Função auxiliar para fazer merge de nós
void merge(BTreeNode *node, int idx) {
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx+1];
    
    // Copia a chave do pai para child
    child->keys[MIN-1] = node->keys[idx];
    
    // Copia as chaves de sibling para child
    for (int i = 0; i < sibling->count; ++i)
        child->keys[i+MIN] = sibling->keys[i];
        
    // Se não for folha, copia os filhos também
    if (!child->leaf) {
        for (int i = 0; i <= sibling->count; ++i)
            child->children[i+MIN] = sibling->children[i];
    }
    
    // Move as chaves do nó um passo à esquerda
    for (int i = idx+1; i < node->count; ++i)
        node->keys[i-1] = node->keys[i];
        
    // Move os filhos correspondentes
    for (int i = idx+2; i <= node->count; ++i)
        node->children[i-1] = node->children[i];
        
    child->count += sibling->count + 1;
    node->count--;
    
    free(sibling);
}

// Função principal de remoção
void removeKey(BTreeNode **root, int k) {
    BTreeNode *r = *root;
    
    if (!r) {
        printf("A árvore está vazia!\n");
        return;
    }
    
    removeFromNode(r, k);
    
    // Se a raiz ficou vazia
    if (r->count == 0) {
        BTreeNode *tmp = r;
        if (r->leaf)
            *root = NULL;
        else
            *root = r->children[0];
        free(tmp);
    }
}

// Função auxiliar que remove a chave de um nó
void removeFromNode(BTreeNode *node, int k) {
    int idx = findKey(node, k);
    
    // Se a chave está presente neste nó
    if (idx < node->count && node->keys[idx] == k) {
        if (node->leaf)
            removeFromLeaf(node, idx);
        else
            removeFromNonLeaf(node, idx);
    }
    else {
        // Se este é um nó folha, a chave não existe na árvore
        if (node->leaf) {
            printf("A chave %d não existe na árvore!\n", k);
            return;
        }
        
        // A chave a ser removida está presente no sub-árvore com raiz em node->children[idx]
        bool flag = (idx == node->count);
        
        // Se o filho onde a chave supostamente existe tem menos que MIN chaves
        if (node->children[idx]->count < MIN)
            fillChild(node, idx);
            
        // Se o último filho foi merged
        if (flag && idx > node->count)
            removeFromNode(node->children[idx-1], k);
        else
            removeFromNode(node->children[idx], k);
    }
}

// Remove a chave de um nó folha
void removeFromLeaf(BTreeNode *node, int idx) {
    // Move todas as chaves depois de idx um passo para trás
    for (int i = idx+1; i < node->count; ++i)
        node->keys[i-1] = node->keys[i];
        
    node->count--;
}

// Remove a chave de um nó não-folha
void removeFromNonLeaf(BTreeNode *node, int idx) {
    int k = node->keys[idx];
    
    // Se o filho que precede k tem pelo menos MIN chaves
    if (node->children[idx]->count >= MIN) {
        int pred = getPred(node, idx);
        node->keys[idx] = pred;
        removeFromNode(node->children[idx], pred);
    }
    
    // Se o filho que sucede k tem pelo menos MIN chaves
    else if (node->children[idx+1]->count >= MIN) {
        int succ = getSucc(node, idx);
        node->keys[idx] = succ;
        removeFromNode(node->children[idx+1], succ);
    }
    
    // Se ambos os filhos têm menos que MIN chaves
    else {
        merge(node, idx);
        removeFromNode(node->children[idx], k);
    }
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
