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

class SATSolverCDCL
{
private:
    int unitPropagation(CNF &formula, Assignment &assignment);
    int chooseLiteral(CNF &formula);
    std::pair<Clause, int> analyzeConflict(const CNF &formula, const Assignment &assignment);
    // int getBacktrackLevel(Clause &learnt_clause);
    // void backtrack(CNF &formula, Assignment &assignment, int backtrack_level);

    int decision_level;
    std::unordered_map<int, int> decision_stack;

public:
    bool solve(CNF &formula);
};

int SATSolverCDCL::unitPropagation(CNF &formula, Assignment &assignment)
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

int SATSolverCDCL::chooseLiteral(CNF &formula)
{
    // default strategy
    return *(formula[0].begin());
}

std::pair<Clause, int> SATSolverCDCL::analyzeConflict(const CNF &formula, const Assignment &assignment)
{
    Clause conflict_clause;

    for (const Clause &clause : formula)
    {
        for (int literal : clause)
        {
            if (assignment.find(-literal) == assignment.end())
            {
                conflict_clause.insert(literal);
            }
        }
    }

    return conflict_clause;
}

/*int SATSolverCDCL::getBacktrackLevel(Clause &learnt_clause)
{
    return 0;
}*/

/*void SATSolverCDCL::backtrack(CNF &formula, Assignment &assignment, int backtrack_level)
{
}*/

bool SATSolverCDCL::solve(CNF &formula)
{
    Assignment assignment = {};
    Clause learnt_clause = {};

    int result = SATSolverCDCL::unitPropagation(formula, assignment);

    if (result == SAT::satisfied)
    {
        return true;
    }
    else if (result == SAT::unsatisfied)
    {
        return false;
    }

    while (result != SAT::satisfied || !formula.empty())
    {
        result = SATSolverCDCL::unitPropagation(formula, assignment);

        while (result != SAT::satisfied)
        {
            if (decision_level == 0)
            {
                return false;
            }
            
            std::pair<Clause, int> analysis = SATSolverCDCL::analyzeConflict(formula, assignment);
            learnt_clause = analysis.first;
            int backtrack_level = analysis.second;
            
            int k = decision_stack[decision_level] - decision_stack[backtrack_level];

            int i = 0;
            for (auto it = assignment.begin(); it != assignment.end();)
            {
                if (i++ >= k)
                {
                    it = assignment.erase(it);
                }
                else
                {
                    it++;
                }
            }

            decision_level = backtrack_level;

            formula.push_back(learnt_clause);
            result = SATSolverCDCL::unitPropagation(formula, assignment);
        }

        if (!formula.empty())
        {
            decision_stack[decision_level] = assignment.size();
            decision_level++;
            int literal = SATSolverCDCL::chooseLiteral(formula);
            result = SATSolverCDCL::unitPropagation(formula, assignment);
        }
    }
    return true;
}

int main()
{
    CNF formula = {{2}, {-2}, {-3, 4}, {-4, 1}};

    SATSolverDPLL solver;

    if (solver.solve(formula))
    {
        std::cout << "SAT\n";
    }
    else
    {
        std::cout << "UNSAT\n";
    }

    return 0;
}