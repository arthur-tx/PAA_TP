#if !defined(BRANCHBOUND)
#define BRANCHBOUND

// imports
#include "commons.hpp"

bool read_problem(FILE* file, Problem &p);
bool is_promising(const Problem &p, int level, double cur_w, double cur_v, double cur_val, double best_val, const vector<Density_Itens>& d);
bool is_complete(const Problem &p, int level);
bool is_consistent(const Problem &p, double current_w, double current_v);
bool generates_initial_solution(const Problem &p, Solution &s_best, Density_Itens &d);
void solve(int level, const Problem &p, double cur_w, double cur_v, double cur_val, Solution &s_curr, Solution &s_best, const vector<Density_Itens>& d);
void BBOtimizacao(const Problem &p, Solution &s_initial, Solution &s_best, const vector<Density_Itens>& d);
void method_branchBound(FILE* file);

#endif // BRANCHBOUND
