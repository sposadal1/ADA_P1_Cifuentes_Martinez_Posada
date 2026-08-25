#ifndef BT_SOLVER_HPP
#define BT_SOLVER_HPP

#include <string>
#include <vector>
#include <cstdint>

struct Policy {
    int length;
    int minLower;
    int minUpper;
    int minDigit;
    int minSymbol;
    bool noConsecutiveDuplicates;
};

struct BTMetrics {
    uint64_t nodesGenerated = 0;
    uint64_t nodesPruned = 0;
    uint64_t solutionsFound = 0;
    double executionTimeMs = 0.0;
};

class BTSolver {
private:
    std::string alphabet;
    Policy policy;

    inline bool isLower(char c) const { return c >= 'a' && c <= 'z'; }
    inline bool isUpper(char c) const { return c >= 'A' && c <= 'Z'; }
    inline bool isDigit(char c) const { return c >= '0' && c <= '9'; }
    inline bool isSymbol(char c) const {
        return c == '!' || c == '@' || c == '#' || c == '$' || c == '%';
    }

    void backtrackFast(
        std::string& current, 
        int low, int up, int dig, int sym, 
        BTMetrics& metrics
    );

    bool isValidFull(const std::string& current) const;
    void backtrackWithoutPruning(std::string& current, BTMetrics& metrics);

public:
    BTSolver(const std::string& alphabet, const Policy& policy);
    BTMetrics solveWithPruning();
    BTMetrics solveWithoutPruning();
};

#endif // BT_SOLVER_HPP