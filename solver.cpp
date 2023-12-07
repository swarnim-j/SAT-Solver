#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <unordered_set>

// Type definition for a clause
typedef std::set<int> Clause;

// Type definition for a CNF formula
typedef std::vector<Clause> CNF;

// Type definition for an assignment
typedef std::set<int> Assignment;

enum SAT
{
    satisfied,
    unsatisfied,
    normal,
    completed
};

class SATSolverDPLL
{
private:
    int unitPropogation(CNF &formula, Assignment &assignment);
    int chooseLiteral(CNF &formula);

public:
    bool solve(CNF &formula);
};

int SATSolverDPLL::unitPropogation(CNF &formula, Assignment &assignment)
{
    if (formula.size() == 0)
    {
        return SAT::satisfied;
    }

    bool unit_clause_found = true;
    while (unit_clause_found)
    {
        unit_clause_found = false;

        for (auto it = formula.begin(); it < formula.end();)
        {
            int unassigned_count = 0;
            int unassigned_literal = 0;

            for (auto itt = (*it).begin(); itt != (*it).end();)
            {
                int literal = *itt;
                if (assignment.find(literal) != assignment.end())
                {
                    unit_clause_found = true;
                    it = formula.erase(it);
                    break;
                }
                else if (assignment.find(-literal) != assignment.end())
                {
                    itt = (*it).erase(itt);
                }
                else
                {
                    unassigned_count++;
                    unassigned_literal = literal;
                    itt++;
                }
            }

            if (unassigned_count == 1)
            {
                assignment.insert(unassigned_literal);
                it = formula.erase(it);
                unit_clause_found = true;
            }
            else if ((*it).size() == 0)
            {
                return SAT::unsatisfied;
            }
            else
            {
                it++;
            }

            if (formula.size() == 0)
            {
                return SAT::satisfied;
            }
        }
    }
    return SAT::normal;
}

int SATSolverDPLL::chooseLiteral(CNF &formula)
{
    // default strategy
    return *(formula[0].begin());
}

bool SATSolverDPLL::solve(CNF &formula)
{
    Assignment assignment = {};
    int decision_level = 0;

    int result = unitPropogation(formula, assignment);

    if (result == SAT::satisfied)
    {
        return true;
    }
    else if (result == SAT::unsatisfied)
    {
        return false;
    }

    while (true)
    {
        int literal = SATSolverDPLL::chooseLiteral(formula);

        assignment.insert(literal);

        decision_level++;

        int result = unitPropogation(formula, assignment);

        while (true)
        {
            if (result == SAT::satisfied)
            {
                return true;
            }
            else if (result == SAT::unsatisfied)
            {
                decision_level--;
                assignment.erase(literal);
                assignment.insert(-literal);
                if (decision_level == 0)
                {
                    return false;
                }
                else
                {
                    result = unitPropogation(formula, assignment);
                }
            }
            else
            {
                break;
            }
        }
    }
}

int main()
{
    // CNF formula = {{-1}, {-3, 4}, {2, 3}, {1, -2}};
    CNF formula = {{1}, {-1, -5}, {5, 3}, {4}, {-4}};
    CNF formula_original(formula);
    Assignment assignment = {};

    SATSolverDPLL solver;

    if (solver.solve(formula))
    {
        std::cout << "SAT\n";
        /*for (auto literal : assignment)
        {
            std::cout << "Variable " << abs(literal) << ": " << (literal > 0 ? "true" : "false");
            std::cout << "\n";
        }
        std::cout << ((formula.size() > 0) ? "Clauses:\n" : "");
        for (auto &clause : formula)
        {
            for (auto literal : clause)
            {
                std::cout << literal << " ";
            }
            std::cout << "\n";
        }*/
    }
    else
    {
        std::cout << "UNSAT\n";
    }

    return 0;
}