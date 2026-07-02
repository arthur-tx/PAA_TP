#if !defined(DYNAMICPROG)
#define DYNAMICPROG

// imports
#include "commons.hpp"

bool read_problem(FILE* file, Problem &p);
vector<int> reconstruct_solution(const vector<vector<int>>& dp, const vector<Item>& itens, int W, int V, int max_val);
int solve_knapsack_dp(const Problem &p, vector<int>& selected_items, double& tempo_exec);
bool method_dynamicProg(FILE* file);

#endif // DYNAMICPROG
