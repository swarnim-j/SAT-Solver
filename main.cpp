#include "sat_solver.h"
#include <iostream>
#include <fstream>
#include <sstream>

/*
*  Parses a DIMACS input file and stores the formula in a vector of vectors.
*/

bool parseDIMACS(std::string &dimacs_input, std::vector<std::vector<int>> &formula)
{
    std::istringstream iss(dimacs_input);
    std::string line;

    while (std::getline(iss, line))
    {
        if (line[0] == 'c' || line.empty())
        {
            continue;
        }
        else if (line[0] == 'p')
        {
            std::istringstream problem_line(line);
            std::string token;
            problem_line >> token; // Ignore 'p'
            problem_line >> token; // Ignore 'cnf'

            int num_variables, num_clauses;
            
            if (!(problem_line >> num_variables >> num_clauses))
            {
                std::cerr << "Error parsing problem line.\n";
                return false;
            }
        }
        else
        {
            std::istringstream clause_line(line);
            std::vector<int> clause;
            int literal = 1;
            while (clause_line >> literal && literal != 0)
            {
                clause.push_back(literal);
            }
            formula.push_back(clause);
        }
    }

    /*std::cout << "Formula:\n";

    for (auto clause : formula)
    {
        for (auto literal : clause)
        {
            std::cout << literal << " ";
        }
        std::cout << "\n";
    }*/

    return true;
}

/*
* Main function.
*/

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " '<DIMACS input>'\n";
        return 1;
    }

    std::string dimacs_input = argv[1];

    std::vector<std::vector<int>> formula;

    if (parseDIMACS(dimacs_input, formula))
    {
        SATSolver solver(formula);

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

    return 1;
}