#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {
    int item;
    struct node *next;
} node;

typedef struct binary_tree {
    int item;
    struct binary_tree *left;
    struct binary_tree *right;
} binary_tree;

node* create_linked_list() {
    return NULL;
}

node* add_linked_list(node *tail, int item) { // adiciona um novo nó na calda
    node *new_node = (node*) malloc(sizeof(node));
    new_node->item = item;
    new_node->next = NULL;
    if (tail == NULL) {
        return new_node;
    }
    node *current = tail;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return tail;
}

void print_linked_list(node *head) {
    while (head != NULL) {
        printf("%d ", head->item);
        head = head->next;
    }
    printf("\n");
}

int list_seach(node *head , int item , int i) {
    while (head != NULL) {
        if (head->item == item) {
            printf("item(%d) foi encontrado\n", item);
            return i;
        }
        head = head->next;
        i++;
    }
    printf("item(%d) não encontrado\n", item);
    return 0;
}

binary_tree* create_empty_binary_tree() {
    return NULL;
}

binary_tree* add_binary_tree(binary_tree *root, int item) {
    if (root == NULL) {
        binary_tree *new_binary_tree = (binary_tree*) malloc(sizeof(binary_tree));
        new_binary_tree->item = item;
        new_binary_tree->left = NULL;
        new_binary_tree->right = NULL;
        return new_binary_tree;
    }
    if (item < root->item) {
        root->left = add_binary_tree(root->left, item);
    } else {
        root->right = add_binary_tree(root->right, item);
    }
    return root;
}

void print_binary_tree(binary_tree *root) { // Função para imprimir a árvore binária em ordem (in-order)
    if (root != NULL) {
        print_binary_tree(root->left);  // Visita os nós à esquerda
        printf("%d ", root->item);      // Visita a raiz
        print_binary_tree(root->right); // Visita os nós à direita
    }
}

int random_num(int* used_numbers, int size) { // Função para gerar número aleatório
    int num;
    int is_unique;
    do {
        num = rand() % (size + 1); // Gera um número aleatório entre 0 e size
        //printf("%d e unico?\n", num);
        is_unique = 1; // Assume que é único
        for (int i = 0; i < size; i++) {// Verifica se o número já foi gerado
            if (used_numbers[i] == num) {
                is_unique = 0;
                break;
            }
        }
    } while (!is_unique); // Repete até encontrar um número único
    return num;
}

int binary_tree_seach(binary_tree *root, int item , int i) {
    if (root == NULL || root->item == item) {
        return i;
    } else if (root->item > item) {
        return binary_tree_seach(root->left, item, i + 1);
    } else {
        return binary_tree_seach(root->right, item, i + 1);
    }
}

int main() { //adicionar um switch case para caso individual(faz a comparação com uma entrada x) e para o caso da sequencia(faz a comparação com uma entrada de 0 a x)
    int  max_size;
    srand(time(NULL)); // gera uma semente de geração aleatoria para a da função Rand() 
    printf("Qual o tamanho Maximo da entrada da entrada? ");
    scanf("%d", &max_size);
    for(int size = 1; size <= max_size ; size++){ //testa as estruturas com uma entrada de 1 ao número maximo definido pelo usuário 
        node* lista = create_linked_list(); //cria a lista encadeada
        binary_tree* arvore = create_empty_binary_tree(); //cria a árvore binária 
        int used_numbers[max_size]; // Array para armazenar números gerados
        for (int i = 0; i < size; i++) {
            int num = random_num(used_numbers, i); //gera um número aleatorio
            used_numbers[i] = num; // Armazena o número gerado para evitar repetição
            lista = add_linked_list(lista, num); //adicionar o número aleatorio gerado a lista
            arvore = add_binary_tree(arvore, num); //adicionar o número aleatorio gerado a árvore
        }
        printf("Lista encadeada: ");// Imprimir lista e árvore
        print_linked_list(lista);
        printf("Árvore binária: "); //imprime a árvore em ordem
        print_binary_tree(arvore);
        printf("\n");
        int search_item = used_numbers[size-1]; //procura o ultimo item (também pode procurar um numero aleatorio, é so trocar [size-1] por [rand() % size])
        printf("Procurando o item: %d\n", search_item);
        int num_list = list_seach(lista, search_item, 0); // Buscar o número na lista e contar comparações
        int num_binary = binary_tree_seach(arvore, search_item, 0); // Buscar o número na árvore e contar comparações
        printf("Número de comparações até achar:\nNa Árvore de busca binária: %d\nNa Lista encadeada: %d\n", num_binary, num_list);
    }
}
       
/* se você não for jayme, ignore isso, é um backup da ultima versão do código (sempre faço isso antes de editar)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node{
    int item;
    struct node *next;
}node;

typedef struct binary_tree{
    int item;
    struct binary_tree *left;
    struct binary_tree *right;
} binary_tree;

node* create_linked_list() {
    return NULL;
}

node* add_linked_list(node *tail, int item) { //adiciona um novo no na calda
    node *new_node = (node*) malloc(sizeof(node));
    new_node->item = item;
    new_node->next = NULL;
    if (tail == NULL) {
        return new_node;
    }
    node *current = tail;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return tail;
}

void print_linked_list(node *head) {
    while (head != NULL) {
        printf("%d ", head->item);
        head = head->next;
    }
    printf("\n");
}

int list_seach(node *head , int item , int i){
    while(head != NULL){
        if(head->item == item){
            printf("item(%d) foi encontrado\n", item);
            return i;
        }
        head = head->next;
        i++;
    }
    printf("item(%d) não encontrado\n", item);
    return 0;
}

binary_tree* create_empty_binary_tree(){
    return NULL;
}

binary_tree* add_binary_tree(binary_tree *root, int item) {
    if (root == NULL) {
        binary_tree *new_binary_tree = (binary_tree*) malloc(sizeof(binary_tree));
        new_binary_tree->item = item;
        new_binary_tree->left = NULL;
        new_binary_tree->right = NULL;
        return new_binary_tree;
    }
    if (item < root->item) {
        root->left = add_binary_tree(root->left, item);
    } else {
        root->right = add_binary_tree(root->right, item);
    }
    return root;
}

void print_binary_tree(binary_tree *root){

}

int binary_tree_seach(binary_tree *root, int item , int i){
    if((root == NULL) || root->item == item){
        return i;
    } 
    else if(root->item > item){
        i++;
        return binary_tree_seach(root->left, item , i);
    }
    else{
        i++;
        return binary_tree_seach(root->right, item , i);
    }
}

int random_num_generetor(int array[], int i , int size){
    int num;
    int unique;
    do{
        num = rand() % size;
        for(int i = 0; i < size)
    } while (!unique);
    
}

int main(){
    node* lista = create_linked_list();
    binary_tree* arvore = create_empty_binary_tree();
    int size;
    printf("qual o tamanho da entrada ?");
    scanf("%d", &size);
    int random_num[size];
    srand(time(NULL)); //srand() gera sementes para a função rand() e time() retorna o valor em segundos que se passaram desde 1º de janeiro de 1970, 00:00:00 UTC 
    for(int i = 0 ; i < size ; i++ ){
        int num;
        random_num[i] = random_num_generetor(random_num , i , size);
        add_linked_list(lista, random_num[i]);
        add_binary_tree(arvore, random_num[i]);
    }

    print_linked_list(lista);
    print_binary_tree(arvore);
    int num_list = list_seach(lista , size , 0);
    int num_binary = binary_tree_seach(arvore, size , 0);
    printf("Número de comparações até achar:\n Na Arvoré de busca binaria: %d\n na Lista encadeada: %d");
}

*/