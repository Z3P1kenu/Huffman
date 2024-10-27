import matplotlib.pyplot as plt
import re

def read_search_data(filepath):
    # Dicionários para armazenar os valores
    linked_list_values = {}
    binary_tree_values = {}
    
    try:
        with open(filepath, 'r', encoding='utf-8') as file:
            content = file.read()
            
            # Extrair valores da lista encadeada
            ll_pattern = r'L\((\d+)\)\s*=\s*(\d+)'
            ll_matches = re.finditer(ll_pattern, content)
            for match in ll_matches:
                index = int(match.group(1))
                value = int(match.group(2))
                linked_list_values[index] = value
            
            # Extrair valores da árvore binária
            bt_pattern = r'T\((\d+)\)\s*=\s*(\d+)'
            bt_matches = re.finditer(bt_pattern, content)
            for match in bt_matches:
                index = int(match.group(1))
                value = int(match.group(2))
                binary_tree_values[index] = value
                
        return linked_list_values, binary_tree_values
    
    except FileNotFoundError:
        print(f"Erro: O arquivo {filepath} não foi encontrado.")
        return None, None
    except Exception as e:
        print(f"Erro ao ler o arquivo: {str(e)}")
        return None, None

def plot_efficiency_comparison(linked_list, binary_tree):
    plt.figure(figsize=(12, 6))
    
    # Ordenar os dados pelos índices
    x_ll = sorted(linked_list.keys())
    y_ll = [linked_list[k] for k in x_ll]
    
    x_bt = sorted(binary_tree.keys())
    y_bt = [binary_tree[k] for k in x_bt]
    
    # Plotar os dados
    plt.plot(x_ll, y_ll, 'b-o', label='Lista Encadeada', linewidth=2, markersize=8)
    plt.plot(x_bt, y_bt, 'r-s', label='Árvore de Busca Binária', linewidth=2, markersize=8)
    
    # Configurar o gráfico
    plt.title('Comparação de Eficiência: Lista Encadeada vs Árvore de Busca Binária', 
              fontsize=14, pad=20)
    plt.xlabel('Número de Elementos (n)', fontsize=12)
    plt.ylabel('Número de Comparações', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(fontsize=10)
    
    # Ajustar os limites dos eixos
    plt.xlim(0, max(max(x_ll), max(x_bt)) + 1)
    plt.ylim(0, max(max(y_ll), max(y_bt)) + 1)
    
    # Adicionar pontos de grade
    plt.xticks(range(0, max(max(x_ll), max(x_bt)) + 2, 2))
    plt.yticks(range(0, max(max(y_ll), max(y_bt)) + 2, 2))
    
    return plt

def main():
    # Caminho do arquivo
    filepath = r"D:\Documentos\Scripts\C\Testes\output\resultados_comparacoes.txt"
    
    # Ler os dados
    linked_list_data, binary_tree_data = read_search_data(filepath)
    
    if linked_list_data is not None and binary_tree_data is not None:
        # Criar o gráfico
        plt = plot_efficiency_comparison(linked_list_data, binary_tree_data)
        
        # Mostrar o gráfico
        plt.show()
    else:
        print("Não foi possível criar o gráfico devido a erros na leitura do arquivo.")

if __name__ == "__main__":
    main()
