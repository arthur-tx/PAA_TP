#ifndef BACKTRACKING_HPP
#define BACKTRACKING_HPP

#include "commons.hpp"

using namespace std;

struct ItemOrdenado {
    int id;               // Índice original do item
    double densidade;     // Valor/(peso+volume)
    
    ItemOrdenado(int i = 0, double d = 0.0) : id(i), densidade(d) {}
};

class BacktrackingSolver {
private:
    long long nos_visitados;  // Contador de nós explorados
    
    bool is_consistent(const Problem &p, double cur_w, double cur_v);
    
    bool is_complete(int level, int total_items);
    
    void backtrack(int level, const Problem &p, double cur_w, double cur_v,
                   double cur_val, vector<bool> &current_sol, Solution &best_sol,
                   const vector<ItemOrdenado> &ordered_items);
    
    vector<ItemOrdenado> order_by_density(const Problem &p);
    
    Solution greedy_solution(const Problem &p);

public:
    
    BacktrackingSolver();
    Solution execute_backtracking(const Problem &p, long long &nodes_visited);
};

void method_backtracking(FILE* file);

#endif // BACKTRACKING_HPP