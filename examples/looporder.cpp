#include <bench.hpp>

#include <iostream>
#include <vector>
#include <cstddef>
#include <random>
using mat = float *__restrict;
#define loop(Z) for(auto Z = 0u; Z < n; ++Z)
#define get(X, a, b) X[a * n + b]

void loopij(std::size_t n, mat A, mat B, mat C) {
    loop(i)
        loop(j) {
            float x {};
            loop(k)
                x = x + get(A, i, k) * get(B, k, j);
            get(C, i, j) = x;
        }
}


void loopji(std::size_t n, mat A, mat B, mat C) {
    loop(j)
        loop(i) {
            float x {};
            loop(k)
                x = x + get(A, i, k) * get(B, k, j);
            get(C, i, j) = x;
        }
}

std::vector<float> randvec(const std::size_t n) {
    static std::random_device rd;
    static std::mt19937 mt_gen( rd() );
    static std::uniform_real_distribution<float> dist;

    std::vector<float> res(n, {});
    for(auto &x : res)
        x = dist(mt_gen);
    return res;
}

int main(void) {
    constexpr auto trials = 100u;
    constexpr auto n = 2500u;
    constexpr auto vecsize = n * n;
                                          
//    std::cout  << "Expected time: " << trials * n * n * n / 5e7 << " seconds\n";
    std::vector<float> A = randvec(vecsize),
                  B = randvec(vecsize),
                  C(vecsize, {});

    std::cout << "Generated random data.\n"
              << "----------------------\n";


    std::cout << "loopij:\n"
              << bench(trials, loopij, n, A.data(), B.data(), C.data()) << '\n';

    std::cout << "loopji:\n"
              << bench(trials, loopji, n, A.data(), B.data(), C.data()) << '\n';
}


/* 
 * NOTE: this takes a really long time to run! Here are my results:
 
 Generated random data.
 ----------------------
 loopij:
     Total: 1.91656e+06ms (N = 100)
   Average: 19165.6ms
       Min: 19028.2ms
       Max: 19790.8ms
 
 loopji:
     Total: 1.89337e+06ms (N = 100)
   Average: 18933.7ms
       Min: 18842.5ms
       Max: 18999ms

 * **/
