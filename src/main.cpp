// src\main.cpp

// imports
using namespace std;
#include "headers/commons.hpp"
#include "headers/backtracking.hpp"
#include "headers/branchBound.hpp"
#include "headers/dynamicProg.hpp"

int main(int argc, char const *argv[])
{
    if(argc != 3) {
        cout << "Wrong arguments provided." << endl;
        cout << "Usage: " << argv[0] << " <method> <input_file>" << endl;
        cout << "Methods: dynamicProg, backtracking, branchBound" << endl;
        return 1;
    }
    
    string method = argv[1];
    string input_file = argv[2];
    input_file = "data/" + input_file;
    FILE* file;

    if ((file = fopen(input_file.c_str(), "r")) == NULL) {
        cout << "Archive not exist." <<endl;
        return 1;
    }

    if(method == "dynamicProg") {
        method_dynamicProg(file);
        cout << "Dynamic Programming method selected with input file: " << input_file << endl << endl;
    } else if(method == "backtracking") {
        method_backtracking(file);
        cout << "Backtracking method selected with input file: " << input_file << endl << endl;
    } else if(method == "branchBound") {
        method_branchBound(file);
        cout << "Branch and Bound method selected with input file: " << input_file << endl << endl;
    } else {
        cout << "Unknown method: " << method << endl;
        cout << "Methods: dynamicProg, backtracking, branchBound" << endl << endl;
        return 1;
    }
    
    fclose(file);
    return 0;
}
