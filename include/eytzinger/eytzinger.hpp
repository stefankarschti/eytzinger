#pragma once

#include <vector>
#include <functional>

namespace eytzinger {

template <typename T>
auto build(const std::vector<T>& input) -> std::vector<T> {
    const int n = input.size();
    const int block_size = 64 / sizeof(T);
    std::vector<T> b(n + 1);

    std::function<int(int, int)> eytzinger = [&](int i, int k) -> int {
        if (k <= n) {
            i = eytzinger(i, 2 * k);
            b[k] = input[i++];
            i = eytzinger(i, 2 * k + 1);
        }
        return i;
    };

    eytzinger(0, 1);

    return b;
}

template <typename T>
auto search(const std::vector<T>& b, T x) -> int {
    const int n = b.size() - 1;
    constexpr int block_size = 64 / sizeof(T);
    int k = 1;
    while (k <= n) {
        __builtin_prefetch(b.data() + k * block_size);
        k = 2 * k + (b[k] < x);
    }
    k >>= __builtin_ffs(~k);
    return k;
}

} // namespace eytzinger