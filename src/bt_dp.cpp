#include "bt_dp.hpp"
#include <vector>
#include <functional>
#include <algorithm>

namespace {
    inline bool isLowerC(char c)  { return c >= 'a' && c <= 'z'; }
    inline bool isUpperC(char c)  { return c >= 'A' && c <= 'Z'; }
    inline bool isDigitC(char c)  { return c >= '0' && c <= '9'; }
    inline bool isSymbolC(char c) {
        return c == '!' || c == '@' || c == '#' || c == '$' || c == '%';
    }
}

long long countSolutionsDP(const std::string& alphabet, const Policy& policy) {

    long long catSize[4] = {0, 0, 0, 0};  // 0=low, 1=up, 2=dig, 3=sym
    for (char c : alphabet) {
        if (isLowerC(c))       catSize[0]++;
        else if (isUpperC(c))  catSize[1]++;
        else if (isDigitC(c))  catSize[2]++;
        else if (isSymbolC(c)) catSize[3]++;
    }

    const int minReq[4] = {policy.minLower, policy.minUpper, policy.minDigit, policy.minSymbol};
    const int L = policy.length;


    std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<long long>>>>>> memo(
        L + 1,
        std::vector<std::vector<std::vector<std::vector<std::vector<long long>>>>>(
            minReq[0] + 1,
            std::vector<std::vector<std::vector<std::vector<long long>>>>(
                minReq[1] + 1,
                std::vector<std::vector<std::vector<long long>>>(
                    minReq[2] + 1,
                    std::vector<std::vector<long long>>(
                        minReq[3] + 1,
                        std::vector<long long>(5, -1))))));

    std::function<long long(int, int, int, int, int, int)> rec =
        [&](int pos, int low, int up, int dig, int sym, int lastCat) -> long long {
        if (pos == L) {
            return (low >= minReq[0] && up >= minReq[1] &&
                    dig >= minReq[2] && sym >= minReq[3]) ? 1 : 0;
        }

        long long& memoRef = memo[pos][low][up][dig][sym][lastCat];
        if (memoRef != -1) return memoRef;

        long long total = 0;
        int counts[4] = {low, up, dig, sym};
        for (int cat = 0; cat < 4; ++cat) {

            long long avail = catSize[cat] - ((policy.noConsecutiveDuplicates && cat == lastCat) ? 1 : 0);
            if (avail <= 0) continue;

            int nc[4] = {counts[0], counts[1], counts[2], counts[3]};
            nc[cat] = std::min(nc[cat] + 1, minReq[cat]);

            total += avail * rec(pos + 1, nc[0], nc[1], nc[2], nc[3], cat);
        }
        return memoRef = total;
    };

    return rec(0, 0, 0, 0, 0, 4);
}
