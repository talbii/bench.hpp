#include <bench.hpp>

#include <execution> // what we are benchmarking
#include <numeric>   // std::reduce 
#include <vector>    // reduce a vector
#include <iostream>  // std::cout

template<typename T>
concept is_addable = requires (T a, T b) {
    a + b;
};

/* We will be benchmarking summarize for different
 * values for policy: seq, par, par_unseq, unseq. */
template<typename A, typename execution_policy> requires is_addable<A>
A summarize (const std::vector<A> &vec, const execution_policy &policy) {
    return std::reduce(policy, vec.cbegin(), vec.cend());
}

int main(void) {
    using namespace std::execution;

    constexpr auto trials = 10u;
    constexpr auto vec_size = 1'000'000'000u; // 10^9, approx ~1s to iterate over

    /* Make vector contain nonzero values
     * _just to make sure_ the compiler 
     * will not remove our computation */
    std::vector<int> vec(vec_size, 123);

    /*
     * Our benchmarking is to check the
     * effects of different execution
     * policies on the time it takes to
     * reduce a vector of integers.
     * */

    std::cout << "seq (synchronised, slow):\n"
              << bench(trials, summarize<int, sequenced_policy>, vec, seq) << '\n';

    std::cout << "par (parallel):\n" 
              << bench(trials, summarize<int, parallel_policy>, vec, par) << '\n';

    std::cout << "par_unseq (parallel, vectorised):\n" 
              << bench(trials, summarize<int, parallel_unsequenced_policy>, vec, par_unseq) << '\n';

    std::cout << "unseq (vectorised):\n" 
              << bench(trials, summarize<int, unsequenced_policy>, vec, unseq) << '\n';

     /* since summarize<A, P> is a templated function
      * then it is not possible to infer by passing
      * just `summarize` to bench(). Instead, we
      * need to directly supply A and P. */
}
