#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Ref: https://algorithmica.org/en/eytzinger

const int n = (1 << 20);
const int block_size = 16; // = 64 / 4 = cache_line_size / sizeof(int)
alignas(64) int a[n], b[n + 1];

int eytzinger(int i = 0, int k = 1) {
    if (k <= n) {
        i = eytzinger(i, 2 * k);
        b[k] = a[i++];
        i = eytzinger(i, 2 * k + 1);
    }
    return i;
}

int search(int x) {
    int k = 1;
    while (k <= n) {
        __builtin_prefetch(b + k * block_size);
        k = 2 * k + (b[k] < x);
    }
    k >>= __builtin_ffs(~k);
    return k;
}

int main(int argc, char* argv[]) {
    std::iota(a, a + n, 1);
    eytzinger();

    int x = 4;
    std::cout << "x=";
    std::cin >> x;

    int index1 = search(x);

    auto index2 = std::lower_bound(a, a + n, x) - a;

    if (index1) {
        std::cout << "index1=" << index1 << " b[index1]=" << b[index1] << std::endl;
    } else {
        std::cout << "index1=0. not found\n";
    }

    if (index2 != n && a[index2] == x) {
        std::cout << "index2=" << index2 << " a[index2]=" << a[index2] << std::endl;
    } else {
        std::cout << "index2=n. not found\n";
    }

    return 0;
}