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
public:
    int unitPropogation(CNF &formula, Assignment &assignment);
    int pureElimination(CNF &formula, Assignment &assignment);
    bool isFormulaSatisfied(const CNF &formula, const Assignment &assignment);
    // int chooseLiteral_Random(const CNF &formula, const Assignment &assignment);

public:
    bool solve(CNF &formula);
};

int SATSolverDPLL::unitPropogation(CNF &formula, Assignment &assignment)
{
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

int SATSolverDPLL::pureElimination(CNF &formula, Assignment &assignment)
{
    std::unordered_set<int> literals;
    for (Clause& clause : formula)
    {
        for (int literal : clause)
        {
            literals.insert(literal);
        }
    }
    for (int literal : literals)
    {
        if (literals.find(-literal) == literals.end())
        {
            assignment.insert(literal);
            for (auto it = formula.begin(); it != formula.end();)
            {
                Clause clause = *it;
                if (find(clause.begin(), clause.end(), literal) != clause.end())
                {
                    it = formula.erase(it);
                }
                else
                {
                    it++;
                }
            }
        }
        if (formula.size() == 0)
        {
            return SAT::satisfied;
        }
    }
    return SAT::normal;
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

bool SATSolverDPLL::solve(CNF &formula)
{
    Assignment assignment;
    int decision_level = 0;

    while (true)
    {
        int result = SATSolverDPLL::unitPropogation(formula, assignment);
        if (result == SAT::satisfied)
        {
            return true;
        }
        if (result == SAT::unsatisfied)
        {
            return false;
        }
        if (formula.empty())
        {
            return true;
        }

        //int literal = chooseLiteral_Random(formula, assignment);
        decision_level++;

        if (solve(formula))
        {
            return true;
        }
        //backtrack()
    }
    return false;
}

int main()
{
    // CNF formula = {{-1}, {-3, 4}, {2, 3}, {1, -2}};
    CNF formula = {{3}, {-1, -5}, {5, 3}, {-3, 4}};
    CNF formula_original(formula);
    Assignment assignment = {};

    SATSolverDPLL solver;

    solver.unitPropogation(formula, assignment);

    if (/*solver.isFormulaSatisfied(formula, assignment)*/ true)
    {
        std::cout << "SAT:\n";
        for (auto literal : assignment)
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
        }
    }
    else
    {
        std::cout << "UNSAT\n";
    }

    return 0;
}