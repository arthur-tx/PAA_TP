#if !defined(COMMONS)
#define COMMONS

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <bitset>

using namespace std;

struct Item{
    double weight, value, volume;  
};

struct Solution {
    vector<bool> vetor;
    double value;
};

struct Problem {
    double backpack_w, backpack_v;
    int qntd_itens;
    vector<Item> vector_itens;
};

struct Density_Itens {
    int id;
    double density;
};

bool read_problem(FILE* file, Problem &p);

#endif // COMMONS