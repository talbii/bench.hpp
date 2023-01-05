#include <bench.hpp>

#include <ratio>
#include <functional>
#include <iostream>


int main(void) {
    /* Since addition is quite fast, let 
     * us measure in nanoseconds instead
     * of the default microseconds. */

#define STR_(S) #S
#define STR(S) STR_(S)
#define print_type(T) STR(T) " (" << sizeof(T) << " bytes):"

    constexpr auto trials = 1'000'000u;

    std::cout << print_type(int) << '\n'
              << bench<std::nano>(trials, std::plus<int>{}, 1, 2) << '\n';

    std::cout << print_type(long long) << '\n'
              << bench<std::nano>(trials, std::plus<long long>{}, 1LL, 2LL) << '\n';

    std::cout << print_type(float) << '\n'
              << bench<std::nano>(trials, std::plus<float>{}, 1.0f, 2.0f) << '\n';

    std::cout << print_type(double) << '\n'
              << bench<std::nano>(trials, std::plus<double>{}, 1.0d, 2.0d) << '\n';

    std::cout << print_type(long double) << '\n'
              << bench<std::nano>(trials, std::plus<long double>{}, 1.0L, 2.0L) << '\n';
}
