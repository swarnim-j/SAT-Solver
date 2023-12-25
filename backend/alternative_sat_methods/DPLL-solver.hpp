#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

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
    normal
};

class SATSolverDPLL
{
private:
    int unitPropagation(CNF &formula, Assignment &assignment);
    int chooseLiteral(CNF &formula);

public:
    bool solve(CNF &formula);
};

int SATSolverDPLL::unitPropagation(CNF &formula, Assignment &assignment)
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

    int result = unitPropagation(formula, assignment);

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

        int result = unitPropagation(formula, assignment);

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
        }
    }
}

