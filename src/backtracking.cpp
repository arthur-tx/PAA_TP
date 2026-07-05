// src\backtracking.cpp

#include "headers/backtracking.hpp"

#define DEBUG 0

// ============================================================================
// CONSTRUTOR
// ============================================================================
BacktrackingSolver::BacktrackingSolver() : nos_visitados(0) {}

// ============================================================================
// MÉTODOS PRIVADOS
// ============================================================================

bool BacktrackingSolver::is_consistent(const Problem &p, double cur_w, double cur_v) {
    return (cur_w <= p.backpack_w) && (cur_v <= p.backpack_v);
}

bool BacktrackingSolver::is_complete(int level, int total_items) {
    return level == total_items;
}

vector<ItemOrdenado> BacktrackingSolver::order_by_density(const Problem &p) {
    vector<ItemOrdenado> ordered_items;
    
    for (int i = 0; i < p.qntd_itens; i++) {
        const Item &item = p.vector_itens[i];
        double density = item.value / (item.weight + item.volume);
        ordered_items.push_back(ItemOrdenado(i, density));
    }
    
    // Ordena por densidade decrescente
    sort(ordered_items.begin(), ordered_items.end(),
         [](const ItemOrdenado &a, const ItemOrdenado &b) {
             return a.densidade > b.densidade;
         });
    
    return ordered_items;
}

Solution BacktrackingSolver::greedy_solution(const Problem &p) {
    Solution sol;
    sol.vetor.resize(p.qntd_itens, false);
    sol.value = 0;
    
    vector<ItemOrdenado> ordered_items = order_by_density(p);
    
    double cur_w = 0;
    double cur_v = 0;
    
    // Guloso basicao: soca itens ordenados ate nao caber mais
    for (const auto &item_ord : ordered_items) {
        int id = item_ord.id;
        const Item &item = p.vector_itens[id];
        
        if (cur_w + item.weight <= p.backpack_w &&
            cur_v + item.volume <= p.backpack_v) {
            
            sol.vetor[id] = true;
            sol.value += item.value;
            cur_w += item.weight;
            cur_v += item.volume;
        }
    }
    
    return sol;
}

void BacktrackingSolver::backtrack(int level, const Problem &p, double cur_w, double cur_v, double cur_val, 
                                   vector<bool> &current_sol, Solution &best_sol, const vector<ItemOrdenado> &ordered_items) {
    nos_visitados++;
    
    // Bateu no limite da mochila, faz o backtrack (volta pro pai)
    if (!is_consistent(p, cur_w, cur_v)) {
        return;
    }
    
    // Atualiza melhor solução se encontrou uma melhor
    if (cur_val > best_sol.value) {
        best_sol.value = cur_val;
        best_sol.vetor = current_sol;
    }
    
    // Condição de parada: já decidiu sobre todos os itens
    if (is_complete(level, p.qntd_itens)) {
        return;
    }
    
    // Pega próximo item na ordem de densidade
    int real_id = ordered_items[level].id;
    const Item &item = p.vector_itens[real_id];
    
    // Opcao 1: Poe o item na bag e ve no que da
    current_sol[real_id] = true;
    backtrack(level + 1, p,
              cur_w + item.weight,
              cur_v + item.volume,
              cur_val + item.value,
              current_sol, best_sol, ordered_items);
    
    // Desfazer escolha
    current_sol[real_id] = false;
    
    // Opcao 2: Finge q o item nao existe e segue a vida
    backtrack(level + 1, p,
              cur_w, cur_v, cur_val,
              current_sol, best_sol, ordered_items);
}

// Métodos publicos
Solution BacktrackingSolver::execute_backtracking(const Problem &p, long long &nodes_visited) {
    nos_visitados = 0;
    
    Solution best_sol;
    best_sol.vetor.resize(p.qntd_itens, false);
    best_sol.value = 0;
    
    // Taca a solucao gulosa de limite logo de cara pq alivia mto a arvore
    Solution greedy_sol = greedy_solution(p);
    if (greedy_sol.value > best_sol.value) {
        best_sol = greedy_sol;
    }
    
    vector<bool> current_sol(p.qntd_itens, false);
    vector<ItemOrdenado> ordered_items = order_by_density(p);
    
    // Executa backtracking recursivo
    backtrack(0, p, 0, 0, 0, current_sol, best_sol, ordered_items);
    
    nodes_visited = nos_visitados;
    return best_sol;
}

// Função principal do método
void method_backtracking(FILE* file) {
    if (!file) {
        printf("Error: Invalid file!\n");
        return;
    }
    
    // Ler problema do arquivo
    Problem problem;
    if (!read_problem(file, problem)) {
        printf("Error reading problem from file!\n");
        return;
    }
    
    printf("\nInstance loaded: %d items, W = %.0f, V = %.0f\n\n",
           problem.qntd_itens, problem.backpack_w, problem.backpack_v);
    
    // Criar solver
    BacktrackingSolver solver;
    long long nodes_visited = 0;
    Solution best_solution;
    
    auto start = chrono::high_resolution_clock::now();
    
    // Executar backtracking (sempre ordenado)
    best_solution = solver.execute_backtracking(problem, nodes_visited);
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    
    // Calcular peso e volume utilizados
    double used_weight = 0, used_volume = 0;
    vector<int> selected_items;
    
    for (int i = 0; i < problem.qntd_itens; i++) {
        if (best_solution.vetor[i]) {
            used_weight += problem.vector_itens[i].weight;
            used_volume += problem.vector_itens[i].volume;
            selected_items.push_back(i);
        }
    }
    
    // Saída padronizada
    printf("Maximum profit: %.0f\n", best_solution.value);
    printf("Used weight: %.0f / %.0f\n", used_weight, problem.backpack_w);
    printf("Used volume: %.0f / %.0f\n", used_volume, problem.backpack_v);
    
    printf("Selected items: ");
    if (selected_items.empty()) {
        printf("none");
    } else {
        for (size_t i = 0; i < selected_items.size(); i++) {
            printf("%d", selected_items[i] + 1);
            if (i < selected_items.size() - 1) printf(", ");
        }
    }
    printf("\n");
    
    // Informação específica do backtracking
    printf("Nodes visited: %lld\n", nodes_visited);
    printf("Execution time: %.6f seconds\n\n", duration.count());
}