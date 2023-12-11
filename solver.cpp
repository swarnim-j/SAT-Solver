#include "CDCL-solver.hpp"
#include <iostream>

using namespace std;

int main()
{
    std::vector<std::vector<int>> formula = {{1}, {-1, 3}, {-3, 4, 5}, {-4}, {-5}};

    SATSolverCDCL solver(formula);

    if (solver.solve())
    {
        std::cout << "SAT\n";
    }
    else
    {
        std::cout << "UNSAT\n";
    }

    return 0;
}