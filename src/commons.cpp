// libs\commons.cpp
// author: Andrei

// imports
#include "commons.hpp"

// leitura do problema
bool read_problem(FILE* file, Problem &p){

    if(!file) return false;

    if (fscanf(file, "%lf %lf", &p.backpack_w, &p.backpack_v) != 2) return false;

    p.vector_itens.clear();

    size_t i = 0;
    Item temp;
    while(fscanf(file, "%lf %lf %lf", &temp.weight, &temp.volume, &temp.value) == 3) {   
        p.vector_itens.push_back(temp);
        i++;
    };

    p.qntd_itens = i;
    return true;   
}