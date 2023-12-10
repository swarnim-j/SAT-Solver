#include "CDCL-solver.hpp"
#include "DPLL-solver.hpp"
#include <iostream>

// Type definition for a clause
typedef std::set<int> Clause;

// Type definition for a CNF formula
typedef std::vector<Clause> CNF;

// Type definition for a CNF formula (indexed)
typedef std::vector<std::pair<Clause, int>> IndexedCNF;

// Type definition for an assignment
typedef std::set<int> Assignment;

using namespace std;

int main()
{
    // CNF formula = {{2}, {-3, 4}, {-4, 1}};

    std::vector<std::vector<int>> formula = {{1}, {-1}};

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