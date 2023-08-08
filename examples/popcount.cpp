#include <bench.hpp>

#include <ratio>
#include <bit>
#include <cstddef>
#include <iostream>

using i64 = std::int64_t;
using u64 = std::uint64_t;

auto manual_popcount (u64 x) -> int {
    int count{0};

    while(x) {
        count += x & 1;
        x >>= 1;
    }

    return count;
}

int main(void) {
    constexpr auto trials = 100'000'000u;
    const u64 R = -1;

    std::cout << "std::popcount:\n"
              << bench<std::micro>(trials, std::popcount<u64>, R) << '\n';

    std::cout << "manual_popcount:\n"
              << bench<std::micro>(trials, manual_popcount, R) << '\n';
}
