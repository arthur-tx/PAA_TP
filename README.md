# Problema da Mochila 0-1 Bidimensional (TP_PAA)

Este repositório contém a implementação empírica de algoritmos para resolver uma variante do Problema da Mochila 0-1, que incorpora duas restrições simultâneas: **peso** máximo e **volume** máximo.

## Algoritmos Implementados
Foram desenvolvidos três algoritmos exatos em C++ para comparar seus comportamentos assintóticos:
1. **Programação Dinâmica:** Abordagem pseudo-polinomial que resolve otimamente subproblemas.
2. **Backtracking:** Busca exaustiva profunda sem uso de podas teóricas heurísticas.
3. **Branch and Bound:** Algoritmo em árvore que poda sub-ramos ruins usando a relaxação fracionária do problema como um limitante superior (*upper bound*).

## Estrutura do Repositório
- `src/`: Código fonte em C++ dos algoritmos implementados.
- `scripts/`: Scripts em Python responsáveis por gerar testes (instâncias), fazer os benchmarks (compilando e medindo o tempo do C++) e plotar gráficos/estatísticas.
- `relatorio.tex`: Arquivo LaTeX contendo o relatório científico completo do projeto.

## Como Compilar e Rodar
1. **Compilar os Algoritmos:**
   Na raiz do projeto, execute o `make`:
   ```bash
   make clean
   make all
   ```
   Isso gerará o binário em `bin/programa.exe`.

2. **Gerar as Instâncias de Teste:**
   ```bash
   python3 scripts/generator.py
   ```
   Serão criadas até 10.000 instâncias combinando diferentes restrições.

3. **Rodar o Benchmark (Bateria de Testes):**
   ```bash
   python3 scripts/benchmark.py
   ```
   *Atenção: A bateria completa pode levar algumas horas.*

4. **Gerar os Relatórios e Gráficos:**
   ```bash
   python3 scripts/generate_report.py
   ```
   Isso compilará os resultados e gerará automaticamente gráficos de tempo vs parâmetros, salvos na pasta `scripts/`.
