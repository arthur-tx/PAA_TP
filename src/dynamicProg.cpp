// src/dynamicProg.cpp
// author: Yann

using namespace std;

#include "headers/dynamicProg.hpp"

// Resolução por Programação Dinâmica (Retorna o valor máximo e modifica 'tempo_exec' por referência)
int solve_knapsack_dp(const Problem &p, vector<int>& selected_items, double& tempo_exec) {
    int W = static_cast<int>(p.backpack_w);
    int V = static_cast<int>(p.backpack_v);
    const auto& itens = p.vector_itens;
    int n = p.qntd_itens;

    if (n == 0 || W <= 0 || V <= 0) return 0;

    auto start = chrono::high_resolution_clock::now();

    // Tabela de valores (2D para economizar memória)
    vector<vector<int>> dp(W + 1, vector<int>(V + 1, 0));
    
    // Tabela de decisões (3D, mas usando bool/bits para ser 32x mais leve que int)
    vector<vector<vector<bool>>> keep(n, vector<vector<bool>>(W + 1, vector<bool>(V + 1, false)));

    // percorre todos itens
    for (int i = 0; i < n; ++i) {
        int weigth_item = static_cast<int>(itens[i].weight);
        int volume_item = static_cast<int>(itens[i].volume);
        int value_item = static_cast<int>(itens[i].value);

        // Percorremos de trás para frente para usar a mesma linha da DP
        for (int weight_cap = W; weight_cap >= weigth_item; --weight_cap) {
            for (int volume_cap = V; volume_cap >= volume_item; --volume_cap) {
                int profit_item = dp[weight_cap - weigth_item][volume_cap - volume_item] + value_item;
                if (profit_item > dp[weight_cap][volume_cap]) {
                    dp[weight_cap][volume_cap] = profit_item;
                    keep[i][weight_cap][volume_cap] = true; // Marca que o item i foi usado neste estado (bool)
                }
            }
        }
    }

    // Reconstrução da solução
    selected_items.clear();
    int curr_w = W;
    int curr_v = V;
    for (int i = n - 1; i >= 0; --i) {
        if (keep[i][curr_w][curr_v]) {
            selected_items.push_back(i);
            curr_w -= static_cast<int>(itens[i].weight);
            curr_v -= static_cast<int>(itens[i].volume);
        }
    }

    auto end = chrono::high_resolution_clock::now();
    tempo_exec = chrono::duration<double>(end - start).count();

    return dp[W][V];
}

// Função principal da programação dinâmica
bool method_dynamicProg(FILE* file) {
    Problem problem;
    if (!read_problem(file, problem)) {
        cerr << "Error reading the input file." << endl;
        return false;
    }

    cout << endl << "Instance loaded: " << problem.qntd_itens
        << " items, W = " << problem.backpack_w
        << ", V = " << problem.backpack_v << endl << endl;

    vector<int> selected;
    double tempo = 0.0;

    int lucro_max = solve_knapsack_dp(problem, selected, tempo);

    // Cálculo do peso e volume usados (para exibir)
    int peso_usado = 0, volume_usado = 0;
    for (int idx : selected) {
        const Item& it = problem.vector_itens[idx];
        peso_usado += it.weight;
        volume_usado += it.volume;
    }

    cout << "Maximum profit: " << lucro_max << endl;
    cout << "Used weight: " << peso_usado << " / " << problem.backpack_w << endl;
    cout << "Used volume: " << volume_usado << " / " << problem.backpack_v << endl;
    cout << "Selected items: ";

    if (selected.empty()) {
        cout << "none" << endl;
    } else {
        sort(selected.begin(), selected.end());
        for (size_t i = 0; i < selected.size(); ++i) {
            cout << (selected[i] + 1);
            if (i < selected.size() - 1) cout << ", ";
        }
        cout << endl;
    }

    cout << fixed << setprecision(6);
    cout << "Execution time: " << tempo << " seconds" << endl << endl;

    return true;
}