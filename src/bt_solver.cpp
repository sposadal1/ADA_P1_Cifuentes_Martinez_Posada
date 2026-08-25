#include "bt_solver.hpp"
#include <chrono>

BTSolver::BTSolver(const std::string& alphabet, const Policy& policy)
    : alphabet(alphabet), policy(policy) {}

void BTSolver::backtrackFast(
    std::string& current, 
    int low, int up, int dig, int sym, 
    BTMetrics& metrics
) {
    metrics.nodesGenerated++;
    int k = current.length();

    // 1. Caso Base
    if (k == policy.length) {
        if (low >= policy.minLower && up >= policy.minUpper && 
            dig >= policy.minDigit && sym >= policy.minSymbol) {
            metrics.solutionsFound++;
        }
        return;
    }

    // 2. PODA GLOBAL INMEDIATA
    int remaining = policy.length - k;
    int reqLow = (policy.minLower > low) ? (policy.minLower - low) : 0;
    int reqUp  = (policy.minUpper > up)  ? (policy.minUpper - up)  : 0;
    int reqDig = (policy.minDigit > dig) ? (policy.minDigit - dig) : 0;
    int reqSym = (policy.minSymbol > sym) ? (policy.minSymbol - sym) : 0;

    if (reqLow + reqUp + reqDig + reqSym > remaining) {
        metrics.nodesPruned++;
        return; // Corte de la rama
    }

    char lastChar = k > 0 ? current.back() : '\0';

    // 3. Expansión de estados
    for (char c : alphabet) {
        if (policy.noConsecutiveDuplicates && k > 0 && c == lastChar) {
            metrics.nodesPruned++;
            continue;
        }

        current.push_back(c);
        backtrackFast(
            current, 
            low + isLower(c), up + isUpper(c), 
            dig + isDigit(c), sym + isSymbol(c), 
            metrics
        );
        current.pop_back(); // Retroceso
    }
}

bool BTSolver::isValidFull(const std::string& current) const {
    if ((int)current.length() != policy.length) return false;

    int low = 0, up = 0, dig = 0, sym = 0;
    for (size_t i = 0; i < current.length(); ++i) {
        if (policy.noConsecutiveDuplicates && i > 0 && current[i] == current[i - 1]) {
            return false;
        }
        if (isLower(current[i])) low++;
        else if (isUpper(current[i])) up++;
        else if (isDigit(current[i])) dig++;
        else if (isSymbol(current[i])) sym++;
    }

    return low >= policy.minLower && up >= policy.minUpper &&
           dig >= policy.minDigit && sym >= policy.minSymbol;
}

void BTSolver::backtrackWithoutPruning(std::string& current, BTMetrics& metrics) {
    metrics.nodesGenerated++;

    if ((int)current.length() == policy.length) {
        if (isValidFull(current)) {
            metrics.solutionsFound++;
        }
        return;
    }

    for (char c : alphabet) {
        current.push_back(c);
        backtrackWithoutPruning(current, metrics);
        current.pop_back();
    }
}

BTMetrics BTSolver::solveWithPruning() {
    BTMetrics metrics;
    std::string current = "";
    
    auto start = std::chrono::high_resolution_clock::now();
    backtrackFast(current, 0, 0, 0, 0, metrics);
    auto end = std::chrono::high_resolution_clock::now();
    
    metrics.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    return metrics;
}

BTMetrics BTSolver::solveWithoutPruning() {
    BTMetrics metrics;
    std::string current = "";
    
    auto start = std::chrono::high_resolution_clock::now();
    backtrackWithoutPruning(current, metrics);
    auto end = std::chrono::high_resolution_clock::now();
    
    metrics.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    return metrics;
}