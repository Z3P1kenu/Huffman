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