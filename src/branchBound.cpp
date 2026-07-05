// src\branchBound.cpp

// imports
using namespace std;
#include "headers/branchBound.hpp"

#define DEBBUG 0

// Guloso inicial pra já ter uma solucao base (ajuda muito a podar a arvore depois)
bool generates_initial_solution(const Problem &p, Solution &s_best, vector<Density_Itens> &d){
    Density_Itens temp;
    size_t i = 0;
    d.clear();

    // calculo da densidade dos itens (valor por peso e volume).
    for ( auto &item : p.vector_itens) {
        temp.density = item.value / (item.volume + item.weight);
        temp.id = i++;
        d.push_back(temp);
    }
    
    // ordenação das densidades.
    sort(d.begin(), d.end(), [](Density_Itens a, Density_Itens b) {
        return a.density > b.density;
    });


    // inicialização de todos vetores.
    double cur_w = 0, cur_v = 0, total_value = 0;
    for(int j = 0; j < p.qntd_itens; j++) s_best.vetor[j] = false;

    // Vai tacando item inteiro na mochila ate ela chorar (bater no limite)
    for(auto &item : d) {
        if((cur_w + p.vector_itens[item.id].weight <= p.backpack_w) && (cur_v + p.vector_itens[item.id].volume <= p.backpack_v)) {
            total_value += p.vector_itens[item.id].value;
            cur_w += p.vector_itens[item.id].weight;
            cur_v += p.vector_itens[item.id].volume;
            s_best.vetor[item.id] = true;
           }
        else
            s_best.vetor[item.id] = false;
    }

    s_best.value = total_value;
    
    #if DEBBUG
        printf("sbest value: %.0f\n", s_best.value);
    #endif

    return true;
}

// Testa se esse galho da arvore vale a pena (calcula o upper bound da mochila fracionaria)
bool is_promising(const Problem &p, int level, double cur_w, double cur_v, double cur_val, double best_val, const vector<Density_Itens>& d){
    double bound = cur_val;
    double rw = p.backpack_w - cur_w;
    double rv = p.backpack_v - cur_v;

    // Percorre os possíveis itens restantes seguindo a ordem de densidade (vetor d)
    for (int i = level; i < p.qntd_itens; i++) {        
        // Pega o ID original através do vetor de densidade ordenado
        int idx = d[i].id; 
        const Item &item = p.vector_itens[idx];

        // se couber um item inteiro, coloque na mochila
        if (item.weight <= rw && item.volume <= rv) {
            rw -= item.weight;
            rv -= item.volume;
            bound += item.value;
        } else {
            // se nao couber inteiro, taca a fracao do item so pra calcular o bound ideal e quebra o laco
            double frac_w = (item.weight > 0) ? rw / item.weight : 0;
            double frac_v = (item.volume > 0) ? rv / item.volume : 0;
            double fraction = min((double)1.0, min(frac_w, frac_v));
            
            if (fraction > 0) bound += item.value * fraction;
            break; 
        }
    }
    return bound > best_val;
}

// retorna se preencheu todos valores possíveis
bool is_complete(const Problem &p, int level){
    return level == p.qntd_itens;
}

// retorna se satisfaz as restrições de espaço
bool is_consistent(const Problem &p, double cur_w, double cur_v){
    return (cur_w <= p.backpack_w) && (cur_v <= p.backpack_v);
}


void solve(int level, const Problem &p, double cur_w, double cur_v, double cur_val, Solution &s_curr, Solution &s_best, const vector<Density_Itens>& d) {

    // Deu ruim: se a mochila estourou o peso ou volume, nem continua
    if (!is_consistent(p, cur_w, cur_v)) return;

    // Atualização do Ótimo
    if (cur_val > s_best.value) {
        s_best.value = cur_val;
        s_best.vetor = s_curr.vetor;
    }

    // Se já decidimos sobre todos os itens
    if (is_complete(p, level)) return;

    // Se o teto desse ramo for pior q o q a gente ja achou, corta o ramo fora (Pruning salvador)
    if (!is_promising(p, level, cur_w, cur_v, cur_val, s_best.value, d)) {
        return;
    }

    // Identifica o próximo item a ser processado com base na ordem de densidade
    int real_idx = d[level].id; 
    const Item &it = p.vector_itens[real_idx];

    // Inclui o item na mochila
    s_curr.vetor[real_idx] = true;
    solve(level + 1, p, cur_w + it.weight, cur_v + it.volume, cur_val + it.value, s_curr, s_best, d);

    // Desmarca o item para explorar a próxima opção sem interferência
    s_curr.vetor[real_idx] = false;

    // Não inclui o item na mochila
    solve(level + 1, p, cur_w, cur_v, cur_val, s_curr, s_best, d);
}

// executa todo o processo de otimização (podas)
void BBOtimizacao(const Problem &p, Solution &s_initial, Solution &s_best, const vector<Density_Itens>& d) {
    fill(s_initial.vetor.begin(), s_initial.vetor.end(), false);
    s_initial.value = 0;

    solve(0, p, 0, 0, 0, s_initial, s_best, d);

    s_initial = s_best;
}

// executa tudo (branch bound)
void method_branchBound(FILE* file){
    Problem p;
    Solution s_best;
    Solution s_initial;
    vector<Density_Itens> d;

    // leitura problema
    if(!read_problem(file, p)) {
        printf("Error reading the input file.\n");
        return;
    }

    printf("\nInstance loaded: %d items, W = %.0f, V = %.0f\n\n",
           p.qntd_itens, p.backpack_w, p.backpack_v);

    // inicialização
    for(int i = 0; i < p.qntd_itens; i++) {
        s_best.vetor.push_back(false);
        s_best.value = 0;
        s_initial.vetor.push_back(false);
        s_initial.value = 0;
    }

    #if DEBBUG
    printf("\nBackpack Weight: %.0f Backpack Volume: %.0f\n", p.backpack_w, p.backpack_v);
    for (auto& item : p.vector_itens) {
        printf("weight:%.0f, volume:%.0f, value:%.0f\n", item.weight, item.volume, item.value);
    }
    #endif 

    // geração solução inicial
    if(!generates_initial_solution(p, s_best, d)) {
        printf("Error generating initial solution.\n");
        return;
    }

    // tempo de execução
    auto start = chrono::high_resolution_clock::now();

    BBOtimizacao(p, s_initial, s_best, d);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    // printa tudo referente ao resultado
    double used_weight = 0;
    double used_volume = 0;
    vector<int> selected;

    for (size_t i = 0; i < s_initial.vetor.size(); i++) {
        if (s_initial.vetor[i]) {
            used_weight += p.vector_itens[i].weight;
            used_volume += p.vector_itens[i].volume;
            selected.push_back(i);
        }
    }

    printf("Maximum profit: %.0f\n", s_initial.value);
    printf("Used weight: %.0f / %.0f\n", used_weight, p.backpack_w);
    printf("Used volume: %.0f / %.0f\n", used_volume, p.backpack_v);

    printf("Selected items: ");
    if (selected.empty()) {
        printf("none\n");
    } else {
        for (size_t i = 0; i < selected.size(); i++) {
            printf("%d", (selected[i] + 1));
            if (i < selected.size() - 1) printf(", ");
        }
        printf("\n");
    }

    printf("Execution time: %.6f seconds\n\n", duration.count());
}

