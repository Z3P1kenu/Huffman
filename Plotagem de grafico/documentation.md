# Documentação: Comparação de Estruturas de Dados
## Visão Geral
Este programa implementa e compara duas estruturas de dados fundamentais: lista encadeada e árvore binária de busca. O objetivo é demonstrar a diferença de performance na busca de elementos entre estas estruturas.

## Estruturas de Dados

### Lista Encadeada
```c
typedef struct node {
    int item;
    struct node *next;
} node;
```
- Estrutura simples com um valor inteiro e ponteiro para o próximo nó
- Implementação sequencial onde cada nó aponta para o próximo

### Árvore Binária
```c
typedef struct binary_tree {
    int item;
    struct binary_tree *left;
    struct binary_tree *right;
} binary_tree;
```
- Estrutura com um valor inteiro e dois ponteiros (esquerda e direita)
- Organizada de forma hierárquica para busca eficiente

## Funções Principais

### Manipulação da Lista Encadeada
1. `create_linked_list()`
   - Inicializa uma lista vazia retornando NULL
   - Complexidade: O(1)

2. `add_linked_list(node *tail, int item)`
   - Adiciona novo nó ao final da lista
   - Percorre até encontrar o último elemento
   - Complexidade: O(n)

3. `list_seach(node *head, int item, int i)`
   - Busca linear na lista
   - Retorna o número de comparações necessárias
   - Complexidade: O(n)

### Manipulação da Árvore Binária
1. `create_empty_binary_tree()`
   - Inicializa uma árvore vazia retornando NULL
   - Complexidade: O(1)

2. `add_binary_tree(binary_tree *root, int item)`
   - Adiciona novo nó mantendo a propriedade de BST
   - Menores à esquerda, maiores à direita
   - Complexidade: O(log n) em média, O(n) no pior caso

3. `binary_tree_seach(binary_tree *root, int item, int i)`
   - Busca binária na árvore
   - Retorna o número de comparações necessárias
   - Complexidade: O(log n) em média, O(n) no pior caso

### Funções Auxiliares
1. `random_num(int* used_numbers, int size)`
   - Gera números aleatórios únicos
   - Evita repetições verificando array de números usados
   - Complexidade: O(n)

2. `print_linked_list(node *head)` e `print_binary_tree(binary_tree *root)`
   - Funções de impressão para visualização das estruturas
   - A árvore é impressa em ordem (in-order traversal)
   - Complexidade: O(n)

## Função Principal (main)
- Permite ao usuário definir o tamanho minimo e máximo da entrada e se o número sorteado é o último a ser gerado ou aleatório 
- Testa as estruturas com entradas crescentes do mínimo até o tamanho máximo
- Para cada tamanho:
  1. Cria as estruturas
  2. Gera números aleatórios únicos
  3. Insere os números em ambas estruturas
  4. Realiza busca do algum dos elementos inseridos
  5. Compara o número de operações necessárias

## Complexidade de Tempo
- Lista Encadeada:
  - Inserção no final: O(n)
  - Busca: O(n)
- Árvore Binária:
  - Inserção: O(log n) média, O(n) pior caso
  - Busca: O(log n) média, O(n) pior caso

## Uso de Memória
- Lista Encadeada: O(n)
  - Cada nó ocupa espaço para um inteiro e um ponteiro
- Árvore Binária: O(n)
  - Cada nó ocupa espaço para um inteiro e dois ponteiros

## Observações
- O programa demonstra na prática a diferença de eficiência entre busca linear e busca binária
- A eficiência da árvore binária depende de sua balanceamento
- O gerador de números aleatórios garante valores únicos para melhor análise
