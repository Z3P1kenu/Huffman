import matplotlib.pyplot as plt
import numpy as np

# Geração de dados
n = np.arange(1, 100)  # Tamanhos de entrada de 1 a 1000
linear = n  # O(n) para Busca Linear
binary = np.log2(n)  # O(log n) para Busca Binária

# Plotagem
plt.figure(figsize=(5,3))
plt.plot(n, linear, label="O(n) - Busca Linear", color='r')
plt.plot(n, binary, label="O(log n) - Busca Binária", color='b')
plt.xlabel('Tamanho da Entrada (n)')
plt.ylabel('Operações (Tempo de Execução)')
plt.title('Complexidade de Algoritmos de Busca')
plt.legend()
plt.grid(True)
plt.show()
