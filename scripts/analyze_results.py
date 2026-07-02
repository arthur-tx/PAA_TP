import csv
import collections
import os

def main():
    results_file = "results_examples.csv"
    if not os.path.exists(results_file):
        print("Results file not found. Run benchmark first.")
        return
    
    data = []
    with open(results_file, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            data.append(row)
    
    # Relatório de Lucro e Tempo por instância
    instances = sorted(list(set(r['instance'] for r in data)))
    methods = sorted(list(set(r['method'] for r in data)))
    
    print("\n" + "="*80)
    print(f"{'Instância':<15} | {'Método':<15} | {'Lucro':<10} | {'Tempo (s)':<12} | {'Nós'}")
    print("-" * 80)
    
    for inst in instances:
        for method in methods:
            row = next((r for r in data if r['instance'] == inst and r['method'] == method), None)
            if row:
                profit = row['profit']
                time_val = row['time']
                nodes = row['nodes']
                print(f"{inst:<15} | {method:<15} | {profit:<10} | {time_val:<12} | {nodes}")
        print("-" * 80)

    # Resumo Geral Médio
    print("\nRESUMO MÉDIO POR ALGORITMO:")
    for m in methods:
        m_data = [r for r in data if r['method'] == m and r['time'] != 'TIMEOUT']
        if m_data:
            avg_time = sum(float(r['time']) for r in m_data) / len(m_data)
            print(f"- {m:<15}: {avg_time:.6f} segundos (média)")
        else:
            print(f"- {m:<15}: N/A (todos falharam ou timeout)")

if __name__ == "__main__":
    main()
