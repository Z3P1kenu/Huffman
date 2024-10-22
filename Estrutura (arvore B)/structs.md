# Documentação das Estruturas da Árvore B

## Constantes Definidas

### `MAX`
```c
#define MAX 3
```
**Descrição:** Define a ordem da árvore B, ou seja, o número máximo de chaves que cada nó pode conter.
- Valor definido: 3
- Uso: Controla o tamanho máximo de cada nó da árvore

### `MIN`
```c
#define MIN 2
```
**Descrição:** Define o número mínimo de chaves que cada nó não-folha deve conter.
- Valor definido: 2
- Uso: Garante o balanceamento da árvore
- Observação: É calculado como ⌈MAX/2⌉ (arredondamento para cima de MAX/2)

## Estrutura Principal

### `BTreeNode`
```c
typedef struct BTreeNode {
    int keys[MAX];                    // Array de chaves
    struct BTreeNode *children[MAX + 1]; // Array de ponteiros para os filhos
    int count;                        // Contador de chaves
    int leaf;                         // Flag para nó folha
} BTreeNode;
```

### Campos da Estrutura

#### 1. `keys[MAX]`
- **Tipo:** Array de inteiros
- **Tamanho:** MAX (3 posições)
- **Descrição:** Armazena as chaves (elementos) do nó
- **Indexação:** 0 até (count-1)
- **Característica:** As chaves são mantidas em ordem crescente dentro do array

#### 2. `children[MAX + 1]`
- **Tipo:** Array de ponteiros para BTreeNode
- **Tamanho:** MAX + 1 (4 posições)
- **Descrição:** Armazena os ponteiros para os nós filhos
- **Indexação:** 0 até count
- **Característica:** Cada posição i contém um ponteiro para uma subárvore com valores:
  - Menores que keys[i] (para children[i])
  - Maiores que keys[i] (para children[i+1])

#### 3. `count`
- **Tipo:** Inteiro
- **Descrição:** Número atual de chaves armazenadas no nó
- **Restrições:**
  - 0 ≤ count ≤ MAX (para todos os nós)
  - MIN-1 ≤ count ≤ MAX-1 (para nós não-folha, exceto raiz)

#### 4. `leaf`
- **Tipo:** Inteiro (usado como booleano)
- **Valores:**
  - 1: Indica que o nó é uma folha
  - 0: Indica que o nó é um nó interno
- **Descrição:** Flag que identifica se o nó é uma folha ou não

### Propriedades da Estrutura

1. **Ordenação:**
   - As chaves em cada nó estão em ordem crescente
   - keys[0] < keys[1] < ... < keys[count-1]

2. **Balanceamento:**
   - Todos os nós folha estão no mesmo nível
   - Nós não-folha (exceto raiz) têm pelo menos MIN filhos
   - Cada nó tem no máximo MAX chaves

3. **Relacionamento entre Chaves e Filhos:**
   - Para um nó não-folha, children[i] contém ponteiros para nós com chaves menores que keys[i]
   - children[i+1] contém ponteiros para nós com chaves maiores que keys[i]

### Exemplo de Uso

```c
// Criar um novo nó
BTreeNode *node = createNode(1);  // 1 indica que é um nó folha

// Acessar elementos
node->keys[0] = 10;              // Primeira chave
node->count = 1;                 // Atualizar contador
node->children[0] = NULL;        // Primeiro filho (NULL para folhas)
```