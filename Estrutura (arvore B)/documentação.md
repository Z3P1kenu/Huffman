# Documentação da Árvore B - Funções de Busca e Main

## Função de Busca (search)

### Visão Geral
A função `search` implementa a busca de uma chave em uma árvore B. Ela trabalha em conjunto com a função auxiliar `findKey` para localizar elementos na estrutura.

### Funções

#### `findKey(BTreeNode *node, int k)`
```c
int findKey(BTreeNode *node, int k)
```

**Descrição:** Função auxiliar que encontra o índice da primeira chave maior ou igual ao valor buscado.

**Parâmetros:**
- `node`: Ponteiro para o nó atual da árvore
- `k`: Valor da chave a ser buscada

**Retorno:**
- Retorna o índice da primeira chave maior ou igual a k no nó atual

#### `search(BTreeNode *root, int k)`
```c
BTreeNode* search(BTreeNode *root, int k)
```

**Descrição:** Função principal de busca que percorre a árvore recursivamente para encontrar uma chave.

**Parâmetros:**
- `root`: Ponteiro para a raiz da árvore ou subárvore atual
- `k`: Valor da chave a ser buscada

**Retorno:**
- Retorna o ponteiro para o nó onde a chave foi encontrada
- Retorna NULL se a chave não existir na árvore

**Funcionamento:**
1. Utiliza `findKey` para localizar a posição potencial da chave no nó atual
2. Se a chave for encontrada no nó atual, retorna o nó
3. Se o nó for uma folha e a chave não for encontrada, retorna NULL
4. Caso contrário, busca recursivamente no filho apropriado

## Função Main

### Visão Geral
A função `main` serve como ponto de entrada do programa e demonstra o uso da árvore B através de um exemplo prático.

```c
int main()
```

**Funcionalidade:**
1. Cria uma árvore B vazia com um nó raiz (folha)
2. Insere uma sequência de valores para demonstração:
   - 10, 20, 5, 6, 12, 30, 7, 17
3. Realiza uma travessia da árvore para exibir os elementos em ordem
4. Retorna 0 para indicar execução bem-sucedida

**Exemplo de Uso:**
```c
BTreeNode *root = createNode(1);  // Cria árvore vazia
insert(&root, 10);               // Insere valores
insert(&root, 20);
// ... mais inserções ...
traverse(root);                  // Exibe árvore em ordem
```

**Saída Esperada:**
Uma sequência ordenada dos números inseridos na árvore.