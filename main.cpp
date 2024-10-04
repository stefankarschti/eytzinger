#include <eytzinger/eytzinger.hpp>

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <limits>
#include <chrono>

// Ref: https://algorithmica.org/en/eytzinger

int64_t time_ns() {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
}

std::string duration_to_str(int64_t nano) {
    char buffer[256];
    if (nano < 1000l) {
        snprintf(buffer, sizeof(buffer), "%lld ns", nano);
    } else if (nano < 1000000l) {
        snprintf(buffer, sizeof(buffer), "%.3f μs", nano / 1000.0);
    } else if (nano < 1000000000l) // < 1s
    {
        snprintf(buffer, sizeof(buffer), "%.3f ms", nano / 1000000.0);
    } else if (nano < 60000000000l) // < 60s
    {
        snprintf(buffer, sizeof(buffer), "%.3f s", nano / 1000000000.0);
    } else {
        int64_t seconds = nano / 1000000000;
        int64_t minutes = seconds / 60;
        seconds = seconds % 60;
        int64_t hours = minutes / 60;
        minutes = minutes % 60;

        if (hours > 0)
            snprintf(buffer, sizeof(buffer), "%lld hours %lld minutes %lld seconds", hours, minutes, seconds);
        else
            snprintf(buffer, sizeof(buffer), "%lld minutes %lld seconds", minutes, seconds);
    }
    return std::string(buffer);
}

template <typename Func>
void measure_time(const std::string& title, Func&& func) {
    int64_t t1 = time_ns();
    std::cout << title << " ..." << std::endl;
    func();
    int64_t t2 = time_ns();
    std::cout << title << ": done in " << duration_to_str(t2 - t1) << std::endl;
}

// create a sorted vector of random integers
auto create_random_sorted_vector(const size_t n) -> std::vector<int> {
    std::vector<int> a;
    a.reserve(n);
    std::random_device rd;  // Seed
    std::mt19937 gen(rd()); // Mersenne Twister generator
    std::uniform_int_distribution<> distr(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    // std::uniform_int_distribution<> distr(1, 100);
    std::generate_n(std::back_inserter(a), n, [&]() { return distr(gen); });
    std::sort(a.begin(), a.end());
    return a;
}

void print_vector(const std::vector<int>& a) {
    for (const auto& x : a) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    // Generate input
    constexpr size_t n = 1 << 30;
    std::cout << "input size is " << n << std::endl;
    std::vector<int> a;

    measure_time("create_random_sorted_vector", [&]() { a = create_random_sorted_vector(n); });

    if (a.size() < 100) {
        print_vector(a);
    }

    // Build the Eytzinger tree
    decltype(a) b;
    measure_time("eytzinger build", [&]() { b = eytzinger::build(a); });

    // Search for all numbers
    measure_time("eytzinger search", [&]() {
        for (int x : a) {
            int index1 = eytzinger::search(b, x);
            if (!index1) {
                std::cout << "eytzinger: " << x << " fail\n";
            }
        }
    });

    measure_time("lower_bound search", [&]() {
        for (int x : a) {
            auto i2 = std::lower_bound(a.begin(), a.end(), x);
            if (i2 == a.end()) {
                std::cout << "lower_bound: " << x << " fail\n";
            }
        }
    });

    return 0;
}