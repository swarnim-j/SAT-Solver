#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

enum SAT
{
    satisfied,
    unsatisfied,
    normal
};

class SATSolverCDCL
{
private:
    int unitPropagation(int&);
    int chooseLiteral();
    int analyzeConflict(int&);
    void backtrack(std::vector<int>&, int&);

    std::unordered_map<int, int> literals; // 1: true, 0: false, -1: unassigned
    std::unordered_map<int, int> literal_decision_levels;
    std::unordered_map<int, int> literal_antecedent_clauses;
    std::vector<std::vector<int>> formula;
    int literal_count;
    int assigned_literal_count;
    int antecedent_clause;

public:
    SATSolverCDCL(std::vector<std::vector<int>>&);

    bool solve();
};

SATSolverCDCL::SATSolverCDCL(std::vector<std::vector<int>> &formula)
    {
        this->formula = formula;
        for (int i = 0; i < formula.size(); i++)
        {
            for (int j = 0; j < formula[i].size(); j++)
            {
                int literal = formula[i][j];
                if (std::find(literals.begin(), literals.end(), literal) == literals.end())
                {
                    literals[abs(literal)] = -1;
                    literal_decision_levels[abs(literal)] = -1;
                    literal_antecedent_clauses[abs(literal)] = -1;
                }
            }
        }
        this->literal_count = literals.size();
        this->assigned_literal_count = 0;
        this->antecedent_clause = -1;
    }

int SATSolverCDCL::unitPropagation(int& decision_level)
{
    bool unit_clause_found = true;

    while (unit_clause_found)
    {
        unit_clause_found = false;

        for (int i = 0; i < formula.size() && !unit_clause_found; i++)
        {
            int unassigned_count = 0;
            int unassigned_literal;

            bool clause_satisfied = false;

            for (int j = 0; j < formula[i].size(); j++)
            {
                int literal = formula[i][j];
                
                if (literals[abs(literal)] == -1)
                {
                    unassigned_count++;
                    unassigned_literal = literal;
                }
                else if ((literals[abs(literal)] == 1 && literal < 0) || (literals[abs(literal)] == 0 && literal > 0))
                {
                    continue;
                }
                else
                {
                    clause_satisfied = true;
                    break;
                }
            }

            if (clause_satisfied)
            {
                continue;
            }

            if (unassigned_count == 1)
            {
                literals[abs(unassigned_literal)] = unassigned_literal > 0 ? 1 : 0;
                literal_decision_levels[abs(unassigned_literal)] = decision_level;
                literal_antecedent_clauses[abs(unassigned_literal)] = i;
                assigned_literal_count++;

                unit_clause_found = true;
            }
            else if (unassigned_count == 0)
            {
                antecedent_clause = i;
                return SAT::unsatisfied;
            }
        }
    }
    return SAT::normal;
}

int SATSolverCDCL::chooseLiteral()
{
    // default strategy
    for (auto literal : literals)
    {
        if (literal.second == -1)
        {
            return literal.first;
        }
    }
    return 0;
}

int SATSolverCDCL::analyzeConflict(int& decision_level)
{
    std::vector<int> conflict_clause = formula[antecedent_clause];

    int conflict_decision_level = decision_level;

    int this_level_count = 0;

    int resolver_literal = 0;

    while (true)
    {
        this_level_count = 0;

        for (int i = 0; i < conflict_clause.size(); i++)
        {
            int literal = conflict_clause[i];
            
            if (literal_decision_levels[abs(literal)] == conflict_decision_level)
            {
                this_level_count++;
            }

            if (literal_decision_levels[abs(literal)] == conflict_decision_level && literals[abs(literal)] == -1)
            {
                resolver_literal = literal;
            }
        }

        if (this_level_count == 1)
        {
            break;
        }

        std::vector<int> first_clause = conflict_clause;
        std::vector<int> second_clause = formula[literal_antecedent_clauses[abs(resolver_literal)]];
        conflict_clause.clear();
        conflict_clause.reserve(first_clause.size() + second_clause.size());
        conflict_clause.insert(conflict_clause.end(), first_clause.begin(), first_clause.end());
        conflict_clause.insert(conflict_clause.end(), second_clause.begin(), second_clause.end());
        for (int i = 0; i < conflict_clause.size(); i++)
        {
            if (conflict_clause[i] == -resolver_literal || conflict_clause[i] == resolver_literal)
            {
                conflict_clause.erase(conflict_clause.begin() + i);
                i--;
            }
        }
        sort(conflict_clause.begin(), conflict_clause.end());
        conflict_clause.erase(unique(conflict_clause.begin(), conflict_clause.end()), conflict_clause.end());
    }

    formula.push_back(conflict_clause);

    int backtrack_level = 0;

    for (int i = 0; i < conflict_clause.size(); i++)
    {
        int literal = conflict_clause[i];
        if (literal_decision_levels[abs(literal)] != conflict_decision_level && literal_decision_levels[abs(literal)] > backtrack_level)
        {
            backtrack_level = literal_decision_levels[abs(literal)];
        }
    }

    backtrack(conflict_clause, backtrack_level);

    return backtrack_level;
}

void SATSolverCDCL::backtrack(std::vector<int>& conflict_clause, int& decision_level)
{
    for (auto literal : literals)
    {
        int literal_value = literal.first;
        if (literal_decision_levels[literal_value] > decision_level)
        {
            literals[literal_value] = -1;
            literal_decision_levels[literal_value] = -1;
            literal_antecedent_clauses[literal_value] = -1;
            assigned_literal_count--;
        }
    }
}

bool SATSolverCDCL::solve()
{
    sort(formula.begin(), formula.end(), [](const std::vector<int> &a, const std::vector<int> &b) {
        return a.size() < b.size();
    });

    int decision_level = 0;

    int result = SATSolverCDCL::unitPropagation(decision_level);

    if (result == SAT::satisfied)
    {
        return true;
    }
    else if (result == SAT::unsatisfied)
    {
        return false;
    }

    while (literal_count != assigned_literal_count)
    {
        int literal = SATSolverCDCL::chooseLiteral();

        decision_level++;

        literals[abs(literal)] = literal > 0 ? 1 : 0;
        literal_decision_levels[abs(literal)] = decision_level;
        assigned_literal_count++;

        while (true)
        {
            result = SATSolverCDCL::unitPropagation(decision_level);

            if (result == SAT::unsatisfied)
            {
                if (decision_level == 0)
                {
                    return false;
                }
                
                decision_level = SATSolverCDCL::analyzeConflict(decision_level);
            }
            else
            {
                break;
            }
        }

    }

    return true;
}