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
 *   int get_literal_index(int &literal)
 *      Gets the index of the given literal
 *      @param literal The literal
 *      @return The index of the literal
 *             -1 if the literal is not in the map
 *
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
 *   std::vector<Literal> literals
 *      The literals
 *          Literal:
 *              int literal
 *                  The literal
 *              int value
 *                  The value of the literal
 *                  1: true, 0: false, -1: unassigned
 *              int decision_level
 *                  The decision level of the literal
 *              int antecedent_clause
 *                  The antecedent clause of the literal
 *              double score
 *                  The score of the literal
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
 */

class SATSolver
{
private:
    // Member functions
    int get_literal_index(int &);
    int unitPropagation(int &);
    int chooseLiteral();
    int analyzeConflict(int);
    void backtrack(std::vector<int> &, int &);
    void printFormula(std::vector<std::vector<int>> &);

    // Data members
    // std::unordered_map<int, int> literals;
    // std::unordered_map<int, int> literal_decision_levels;
    // std::unordered_map<int, int> literal_antecedent_clauses;
    struct Literal
    {
        int literal; // 1: true, 0: false, -1: unassigned
        int value;
        int decision_level;
        int antecedent_clause;
        double score;
        Literal(int literal, int value, int decision_level, int antecedent_clause = -1)
        {
            this->literal = literal;
            this->value = value;
            this->decision_level = decision_level;
            this->antecedent_clause = -1;
            this->score = 0;
        }
    };

    std::vector<Literal> literals;
    // std::unordered_map<int, double> literal_scores;
    std::vector<std::vector<int>> formula;
    int literal_count;
    int assigned_literal_count;
    int antecedent_clause;
    int strategy; // 0: basic strategy, 1: VSIDS

public:
    // Constructors
    SATSolver(std::vector<std::vector<int>> &);
    SATSolver(std::vector<std::vector<int>> &, int &);

    // Member functions
    bool solve();
    std::vector<std::pair<int, bool>> getAssignment();
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
            int index = SATSolver::get_literal_index(literal);
            if (index == -1)
            {
                // If the literal is not in the map, add it
                literals.push_back(Literal(literal, -1, -1));
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
            int index = SATSolver::get_literal_index(literal);
            if (index == -1)
            {
                // If the literal is not in the map, add it
                literals.push_back(Literal(literal, -1, -1));
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
}

int SATSolver::get_literal_index(int &literal)
{
    for (int i = 0; i < literals.size(); i++)
    {
        if (literals[i].literal == literal)
        {
            return i;
        }
    }
    return -1;
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
            int unassigned_literal_index;

            bool clause_satisfied = false;

            for (int j = 0; j < formula[i].size(); j++)
            {
                int literal = formula[i][j];
                int index = SATSolver::get_literal_index(literal);
                // If the literal is unassigned, increment the unassigned count
                if (literals[index].value == -1)
                {
                    unassigned_count++;
                    unassigned_literal = literal;
                    unassigned_literal_index = index;
                }
                // If the literal is assigned and satisfied, skip the clause
                else if ((literals[index].value == 1 && literal < 0) || (literals[index].value == 0 && literal > 0))
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
                literals[unassigned_literal_index].value = unassigned_literal > 0 ? 1 : 0;
                literals[unassigned_literal_index].decision_level = decision_level;
                literals[unassigned_literal_index].antecedent_clause = i;
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
        for (auto &literal : literals)
        {
            if (literal.value == -1)
            {
                return literal.literal;
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
            if (literal.value == -1 && literal.score >= max_score)
            {
                max_score = literal.score;
                max_score_literal = literal.literal;
            }

            // Decay the score of the literal
            literal.score *= decay_factor;
        }

        return max_score_literal;
    }

    return 0;
}

int SATSolver::analyzeConflict(int decision_level)
{
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
            int index = SATSolver::get_literal_index(literal);

            // If the literal is assigned at the current decision level, increment the count
            if (literals[index].decision_level == conflict_decision_level)
            {
                this_level_count++;
            }

            // If the literal is assigned at the current decision level
            // and is unassigned, it is the resolver literal
            if (literals[index].decision_level == conflict_decision_level && literals[index].antecedent_clause != -1)
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
        int resolver_index = SATSolver::get_literal_index(resolver_literal);
        std::vector<int> second_clause = formula[literals[resolver_index].antecedent_clause];

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
    for (auto &literal : conflict_clause)
    {
        int index = SATSolver::get_literal_index(literal);
        literals[index].score += 1.0;
    }

    formula.push_back(conflict_clause);

    // Backtrack level is the decision level of the literal that is assigned
    // at the current decision level and is unassigned
    int backtrack_level = 0;

    for (int i = 0; i < conflict_clause.size(); i++)
    {
        int literal = conflict_clause[i];
        int index = SATSolver::get_literal_index(literal);
        if (literals[index].decision_level != conflict_decision_level && literals[index].decision_level > backtrack_level)
        {
            backtrack_level = literals[index].decision_level;
        }
    }

    backtrack(conflict_clause, backtrack_level);

    return backtrack_level;
}

void SATSolver::backtrack(std::vector<int> &conflict_clause, int &decision_level)
{
    for (auto &literal : literals)
    {
        // If the literal is assigned at a higher decision level, unassign it
        if (literal.decision_level > decision_level)
        {
            literal.value = -1;
            literal.decision_level = -1;
            literal.antecedent_clause = -1;
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
        int index = SATSolver::get_literal_index(literal);

        // Increase the decision level and assign the newly chosen literal
        decision_level++;
        literals[index].value = literal > 0 ? 1 : 0;
        literals[index].decision_level = decision_level;
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

std::vector<std::pair<int, bool>> SATSolver::getAssignment()
{
    std::vector<std::pair<int, bool>> assignment;
    for (auto &literal : literals)
    {
        std::pair<int, bool> literal_assignment;
        literal_assignment.first = literal.literal;
        literal_assignment.second = literal.value == 1 ? true : false;
        assignment.push_back(literal_assignment);
    }
    std::sort(assignment.begin(), assignment.end(), [](const std::pair<int, bool> &a, const std::pair<int, bool> &b)
              { return a.first < b.first; });
    return assignment;
}