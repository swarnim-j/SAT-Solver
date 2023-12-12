#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <set>

enum SAT
{
    satisfied,
    unsatisfied,
    normal
};

/*
 * A class for the CDCL-based SAT solver
 *
 * Member functions:
 *   int unitPropagation(int &decision_level)
 *       Propagates unit clauses
 *       @param decision_level The current decision level
 *       @return SAT::satisfied if the formula is satisfied
 *               SAT::unsatisfied if the formula is unsatisfied
 *               SAT::normal if the formula is normal
 *
 *   int chooseLiteral()
 *       Chooses a literal
 *       @return The chosen literal
 *
 *   int analyzeConflict(int &decision_level)
 *       Analyzes the conflict clause
 *       @param decision_level The current decision level
 *       @return The decision level to backtrack to
 *
 *   void backtrack(std::vector<int> &conflict_clause, int &decision_level)
 *       Backtracks to the given decision level
 *       @param conflict_clause The conflict clause
 *       @param decision_level The decision level to backtrack to
 *
 *   bool solve()
 *       Solves the formula
 *       @return true if the formula is satisfied
 *               false if the formula is unsatisfied
 *
 * Data members:
 *   std::unordered_map<int, int> literals
 *       A map of literals
 *       Key: literal
 *       Value: 1 if the literal is true, 0 if the literal is false, -1 if the literal is unassigned
 *
 *   std::unordered_map<int, int> literal_decision_levels
 *       A map of literal decision levels
 *       Key: literal
 *       Value: the decision level of the literal
 *
 *   std::unordered_map<int, int> literal_antecedent_clauses
 *       A map of literal antecedent clauses
 *       Key: literal
 *       Value: the antecedent clause of the literal
 *
 *   std::unordered_map<int, double> literal_scores
 *      A map of literal scores for VSIDS
 *      Key: literal
 *      Value: the score of the literal
 *
 *   std::vector<std::vector<int>> formula
 *       The formula
 *
 *   int literal_count
 *       The number of literals
 *
 *   int assigned_literal_count
 *       The number of assigned literals
 *
 *   int antecedent_clause
 *       The antecedent clause
 *
 *   int strategy
 *       The strategy
 *       0: basic strategy
 *       1: VSIDS
 *
 *  int num_conflicts
 *     The number of conflicts
 */

class SATSolver
{
private:
    // Member functions
    int unitPropagation(int &);
    int chooseLiteral();
    int analyzeConflict(int);
    void backtrack(std::vector<int> &, int &);
    void printFormula(std::vector<std::vector<int>> &);

    // Data members
    std::unordered_map<int, int> literals; // 1: true, 0: false, -1: unassigned
    std::unordered_map<int, int> literal_decision_levels;
    std::unordered_map<int, int> literal_antecedent_clauses;
    std::unordered_map<int, double> literal_scores;
    std::vector<std::vector<int>> formula;
    int literal_count;
    int assigned_literal_count;
    int antecedent_clause;
    int strategy; // 0: basic strategy, 1: VSIDS
    int num_conflicts;

public:
    // Constructors
    SATSolver(std::vector<std::vector<int>> &);
    SATSolver(std::vector<std::vector<int>> &, int &);

    // Member functions
    bool solve();
};

SATSolver::SATSolver(std::vector<std::vector<int>> &formula)
{
    // Initialize the formula
    this->formula = formula;

    // Initialize the literals
    for (int i = 0; i < formula.size(); i++)
    {
        for (int j = 0; j < formula[i].size(); j++)
        {
            int literal = formula[i][j];
            if (literals.find(literal) == literals.end())
            {
                // If the literal is not in the map, add it
                literals[abs(literal)] = -1;
                literal_decision_levels[abs(literal)] = -1;
                literal_antecedent_clauses[abs(literal)] = -1;
                literal_scores[abs(literal)] = 0;
            }
        }
    }

    // Initialize the literal count
    this->literal_count = literals.size();
    this->assigned_literal_count = 0;

    // Initialize the antecedent clause
    this->antecedent_clause = -1;

    // Initialize the strategy
    this->strategy = 1;

    // Initialize the number of conflicts
    this->num_conflicts = 0;
}

SATSolver::SATSolver(std::vector<std::vector<int>> &formula, int &strategy)
{
    // Initialize the formula
    this->formula = formula;

    // Initialize the literals
    for (int i = 0; i < formula.size(); i++)
    {
        for (int j = 0; j < formula[i].size(); j++)
        {
            int literal = formula[i][j];
            if (literals.find(literal) == literals.end())
            {
                // If the literal is not in the map, add it
                literals[abs(literal)] = -1;
                literal_decision_levels[abs(literal)] = -1;
                literal_antecedent_clauses[abs(literal)] = -1;
                literal_scores[abs(literal)] = 0;
            }
        }
    }

    // Initialize the literal count
    this->literal_count = literals.size();
    this->assigned_literal_count = 0;

    // Initialize the antecedent clause
    this->antecedent_clause = -1;

    // Initialize the strategy
    this->strategy = strategy;

    // Initialize the number of conflicts
    this->num_conflicts = 0;
}

int SATSolver::unitPropagation(int &decision_level)
{
    bool unit_clause_found = true;

    // Unit propagation
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

                // If the literal is unassigned, increment the unassigned count
                if (literals[abs(literal)] == -1)
                {
                    unassigned_count++;
                    unassigned_literal = literal;
                }
                // If the literal is assigned and satisfied, skip the clause
                else if ((literals[abs(literal)] == 1 && literal < 0) || (literals[abs(literal)] == 0 && literal > 0))
                {
                    continue;
                }
                // If the literal is assigned and unsatisfied, the clause is satisfied
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

            // If the clause is unit, assign the literal
            if (unassigned_count == 1)
            {
                literals[abs(unassigned_literal)] = unassigned_literal > 0 ? 1 : 0;
                literal_decision_levels[abs(unassigned_literal)] = decision_level;
                literal_antecedent_clauses[abs(unassigned_literal)] = i;
                assigned_literal_count++;

                unit_clause_found = true;
            }
            // If the clause is empty, return unsatisfied
            else if (unassigned_count == 0)
            {
                antecedent_clause = i;
                return SAT::unsatisfied;
            }
        }
    }
    return SAT::normal;
}

int SATSolver::chooseLiteral()
{
    // Basic strategy
    if (strategy == 0)
    {
        // Choose the first unassigned literal
        for (auto literal : literals)
        {
            if (literal.second == -1)
            {
                return literal.first;
            }
        }
    }
    // VSIDS
    else if (strategy == 1)
    {
        // Choose the literal with the highest score
        double max_score = 0.0;
        int max_score_literal = 0;

        // Decay the scores of the literals
        double decay_factor = 0.98;

        for (auto &literal : literals)
        {
            if (literal.second == -1 && literal_scores[literal.first] >= max_score)
            {
                max_score = literal_scores[literal.first];
                max_score_literal = literal.first;
            }

            // Decay the score of the literal
            literal_scores[literal.first] *= decay_factor;
        }

        return max_score_literal;
    }

    return 0;
}

int SATSolver::analyzeConflict(int decision_level)
{
    // Increment the number of conflicts
    num_conflicts++;

    // Conflict clause is the antecedent clause
    std::vector<int> conflict_clause = formula[antecedent_clause];

    // Decision level of the conflict clause
    int conflict_decision_level = decision_level;

    // Number of literals assigned at the current decision level
    int this_level_count = 0;

    // Resolver literal is the literal that is assigned at the current decision level
    int resolver_literal = 0;

    while (true)
    {
        this_level_count = 0;

        for (int i = 0; i < conflict_clause.size(); i++)
        {
            int literal = conflict_clause[i];

            // If the literal is assigned at the current decision level, increment the count
            if (literal_decision_levels[abs(literal)] == conflict_decision_level)
            {
                this_level_count++;
            }

            // If the literal is assigned at the current decision level
            // and is unassigned, it is the resolver literal
            if (literal_decision_levels[abs(literal)] == conflict_decision_level && literal_antecedent_clauses[abs(literal)] != -1)
            {
                resolver_literal = literal;
            }
        }

        // If there is only one literal assigned at the current decision level, break
        if (this_level_count == 1)
        {
            break;
        }

        // Conflict clause is the union of the antecedent clauses of the resolver literal
        // and the conflict clause without the resolver literal
        std::vector<int> first_clause = conflict_clause;
        std::vector<int> second_clause = formula[literal_antecedent_clauses[abs(resolver_literal)]];

        first_clause.insert(first_clause.end(), second_clause.begin(), second_clause.end());

        for (int i = 0; i < first_clause.size(); i++)
        {
            if (first_clause[i] == resolver_literal || first_clause[i] == -resolver_literal)
            {
                first_clause.erase(first_clause.begin() + i);
                i--;
            }
        }

        std::sort(first_clause.begin(), first_clause.end());
        first_clause.erase(std::unique(first_clause.begin(), first_clause.end()), first_clause.end());

        conflict_clause = first_clause;
    }

    // Increment the score of each literal in the conflict clause
    for (auto literal : conflict_clause)
    {
        literal_scores[abs(literal)] += 1.0;
    }

    formula.push_back(conflict_clause);

    // Backtrack level is the decision level of the literal that is assigned
    // at the current decision level and is unassigned
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

void SATSolver::backtrack(std::vector<int> &conflict_clause, int &decision_level)
{
    for (auto literal : literals)
    {
        int literal_value = literal.first;

        // If the literal is assigned at a higher decision level, unassign it
        if (literal_decision_levels[literal_value] > decision_level)
        {
            literals[literal_value] = -1;
            literal_decision_levels[literal_value] = -1;
            literal_antecedent_clauses[literal_value] = -1;
            assigned_literal_count--;
        }
    }
}

bool SATSolver::solve()
{
    // Sort the formula by clause length
    sort(formula.begin(), formula.end(), [](const std::vector<int> &a, const std::vector<int> &b)
         { return a.size() < b.size(); });

    int decision_level = 0;

    int result = SATSolver::unitPropagation(decision_level);

    // If the formula is satisfied, return true
    if (result == SAT::satisfied)
    {
        return true;
    }

    // If the formula is unsatisfied, return false
    else if (result == SAT::unsatisfied)
    {
        return false;
    }

    // If the formula is normal, assign literals until the formula is satisfied or unsatisfied
    while (literal_count != assigned_literal_count)
    {
        // Choose a literal
        int literal = SATSolver::chooseLiteral();

        // Increase the decision level and assign the newly chosen literal
        decision_level++;
        literals[abs(literal)] = literal > 0 ? 1 : 0;
        literal_decision_levels[abs(literal)] = decision_level;
        assigned_literal_count++;

        while (true)
        {
            result = SATSolver::unitPropagation(decision_level);

            if (result == SAT::unsatisfied)
            {
                // If the decision level is 0, return false
                if (decision_level == 0)
                {
                    return false;
                }

                // Otherwise, backtrack
                decision_level = SATSolver::analyzeConflict(decision_level);
            }
            else
            {
                break;
            }
        }
    }

    return true;
}