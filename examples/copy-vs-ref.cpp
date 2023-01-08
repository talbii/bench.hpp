#include <bench.hpp>

#include <cstddef>
#include <ratio>
#include <iostream>

auto by_copy(std::size_t i) -> std::size_t {
    return i + 1;
}

auto by_ref(const std::size_t &i) -> std::size_t {
    return i + 1;
}

int main(void) {
    constexpr auto trials = 100'000'000; 
    std::size_t i {};

    std::cout << "copy:\n"
              << bench<std::nano>(trials, by_copy, i) << '\n';

    std::cout << "ref:\n"
              << bench<std::nano>(trials, by_ref, i) << '\n';
}
