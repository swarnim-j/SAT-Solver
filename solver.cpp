#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <map>

// Type definition for a clause
typedef std::set<int> Clause;

// Type definition for a CNF formula
typedef std::vector<Clause> CNF;

// Type definition for an assignment
typedef std::set<int> Assignment;

class SATSolverDPLL
{
public:
    void unitPropogation(CNF &formula, Assignment &assignment);
    bool isFormulaSatisfied(const CNF &formula, const Assignment &assignment);

public:
    bool DPLL(CNF &formula, Assignment &assignment);
};

void SATSolverDPLL::unitPropogation(CNF &formula, Assignment &assignment)
{
    bool unit_clause_found = true;
    std::map<int, bool> clause_satisfied;
    while (unit_clause_found)
    {
        unit_clause_found = false;
        for (auto it = formula.begin(); it < formula.end();)
        {
            int unassigned_count = 0;
            int unassigned_literal = 0;

            for (auto itt = (*it).begin(); itt != (*it).end(); itt++)
            {
                int literal = *itt;
                if (clause_satisfied[it - formula.begin()])
                {
                    if (assignment.find(literal) != assignment.end() || assignment.find(-literal) != assignment.end())
                    {
                        itt = (*it).erase(itt);
                    }
                }
                else if (assignment.find(literal) != assignment.end())
                {
                    unit_clause_found = true;
                    (*it).erase(literal);
                    clause_satisfied[it - formula.begin()] = true;
                    break;
                }
                else if (assignment.find(-literal) != assignment.end())
                {
                    (*it).erase(literal);
                }
                else
                {
                    unassigned_count++;
                    unassigned_literal = literal;
                    // std::cout << "unassigned: " << unassigned_literal << "\n";
                }
            }

            /*std::cout << "count: " << unassigned_count << "\n";
            std::cout << "variable: ";

            for (auto literal : *it)
                std::cout << literal << " ";

            std::cout << "\n";*/

            if (unassigned_count == 1)
            {
                assignment.insert(unassigned_literal);
                // std::cout << "inserted: " << unassigned_literal;
                it = formula.erase(it);
                unit_clause_found = true;
            }
            else if (unassigned_count == 0)
            {
                it = formula.erase(it);
            }
            else
            {
                it++;
            }

            // std::cout << "\n\n";
        }
    }
}

bool SATSolverDPLL::isFormulaSatisfied(const CNF &formula, const Assignment &assignment)
{
    for (const Clause &clause : formula)
    {
        bool check = false;
        for (int literal : clause)
        {
            if (assignment.find(literal) != assignment.end())
            {
                check = true;
                break;
            }
        }

        if (!check)
        {
            return false;
        }
    }
    return true;
}

bool SATSolverDPLL::DPLL(CNF &formula, Assignment &assignment)
{
    return false;
}

int main()
{
    // CNF formula = {{-1}, {-3, 4}, {2, 3}, {1, -2}};
    CNF formula = {{3}, {-1, -2}, {2, 3}, {-3, 4}};
    Assignment assignment = {};

    SATSolverDPLL solver;

    solver.unitPropogation(formula, assignment);

    if (solver.isFormulaSatisfied(formula, assignment))
    {
        std::cout << "SAT:\n";
        for (auto literal : assignment)
        {
            std::cout << "Variable " << abs(literal) << ": " << (literal > 0 ? "true" : "false");
            std::cout << "\n";
        }

        for (auto& clause : formula)
        {
            std::cout << "Clauses:\n";
            for (auto literal : clause)
            {
                std::cout << literal <<  " ";
            }
            std::cout << "\n";
        }
    }
    else
    {
        std::cout << "UNSAT\n";
    }

    return 0;
}