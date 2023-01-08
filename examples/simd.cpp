#include <bench.hpp>

#include <iostream>
#include <algorithm>
#include <vector>
#include <array>

template<typename Q> 
auto regular(const std::vector<Q> &A, const std::vector<Q> &B) -> std::vector<Q> {
    std::vector<Q> C( std::min(A.size(), B.size()), {} );

    for(auto i = 0u; i < C.size(); ++i)
        C[i] = A[i] * B[i];

    return C;
}

template<typename Q> 
auto simd(const std::vector<Q> &A, const std::vector<Q> &B) -> std::vector<Q> {
    std::vector<Q> C( std::min(A.size(), B.size()), {} );

#pragma omp simd
    for(auto i = 0u; i < C.size(); ++i)
        C[i] = A[i] * B[i];

    return C;
}

template<typename Q> 
auto parallel(const std::vector<Q> &A, const std::vector<Q> &B) -> std::vector<Q> {
    std::vector<Q> C( std::min(A.size(), B.size()), {} );

#pragma omp parallel
    for(auto i = 0u; i < C.size(); ++i)
        C[i] = A[i] * B[i];

    return C;
}

template<typename Q>
auto parallel_simd(const std::vector<Q> &A, const std::vector<Q> &B) -> std::vector<Q> {
    std::vector<Q> C( std::min(A.size(), B.size()), {} );

#pragma omp parallel
#pragma omp simd
    for(auto i = 0u; i < C.size(); ++i)
        C[i] = A[i] * B[i];

    return C;
}

template<typename Q>
auto simd_parallel(const std::vector<Q> &A, const std::vector<Q> &B) -> std::vector<Q> {
    std::vector<Q> C( std::min(A.size(), B.size()), {} );

#pragma omp parallel for simd 
    for(auto i = 0u; i < C.size(); ++i)
        C[i] = A[i] * B[i];
    return C;
}
struct {
    // Normally I would read random data
    // from /dev/random (or maybe /urandom).
    //  This also makes implementation simpler.
    //
    // However, in order to do this in compile-
    // time, we have to do a more primitive 
    // approach.
    
/*__TIME__ = hh:mm:ss*/
#define time_as_number() (((((__TIME__[0] * 10 + __TIME__[1]) * 10 + __TIME__[3]) * 10 + __TIME__[4]) * 10 + __TIME__[6]) * 10 + __TIME__[7])
    unsigned int seed { time_as_number() };

    constexpr unsigned next(void) { 
        // Simple x^2 + 1 PRG
        unsigned x = seed * seed + 1;
        seed = x;
        return x;
    }

    /* Reads 'size' random bytes into x. */
    constexpr void read_into(void *x, std::size_t size) {
        unsigned *px = static_cast<unsigned *>(x);
        constexpr auto block = sizeof(unsigned);

        for(auto i = 0u; i < size / block; ++i)
            px[i] = next();

        if (auto missing = size % block; missing != 0) {
            char *ppx = static_cast<char *>(x);
            for(auto i = 0u; i < missing; ++i)
                ppx[size / block + i] = next(); // here we slice some data
        }
    }
} random_channel;

/*
 * Generates 'size' random values of Q.
 *
 * Note: will probably assign garbage values
 *  for nontrivial Q. The random generation
 *  is done using 'random_channel', which assigns
 *  a random bitstring of size 'sizeof(Q)' to each
 *  item in the returned vector.
 * */
template<typename Q>
constexpr auto gen_random_vec(const std::size_t size) -> std::vector<Q> {
    
    constexpr const auto bytes = sizeof(Q);
    std::vector<Q> result( size, {} );


    std::for_each(result.begin(), result.end(), [](auto &x) {
        static_assert(bytes == sizeof(Q));
        random_channel.read_into(&x, bytes);
    });

    return result;
}

int main(void) {
    constexpr auto trials = 10u;
    constexpr auto vecsize = 125'000'000; // 1.25*10^8, which is 500MB of floats
    using Q = float;

    /* constexpr */ std::vector<Q> 
        A{gen_random_vec<Q>(vecsize)},
        B{gen_random_vec<Q>(vecsize)};

     // Unfortunately, I learnt (the hard way)
     // that compilers place a limit on constexpr
     // loop size, I do not wish to change it, 
     // for the sake of my computer's integrity :)

    std::cout << "Created random data.\n"
              << "--------------------\n\n";
            

    std::cout << "regular:\n"
              << bench(trials, regular<Q>, A, B) << '\n';

    std::cout << "parallel:\n"
              << bench(trials, parallel<Q>, A, B) << '\n';

    std::cout << "simd:\n"
              << bench(trials, simd<Q>, A, B) << '\n';

    std::cout << "parallel_simd:\n"
              << bench(trials, parallel_simd<Q>, A, B) << '\n';

    std::cout << "simd_parallel:\n"
              << bench(trials, simd_parallel<Q>, A, B) << '\n';
}
