#ifndef talbi__BENCH_HPP
#define talbi__BENCH_HPP

/*
 * Set the macro `Parallel` to either std::execution::par_unseq,
 *  (if the compiler supports it), or nothing.  This makes summation
 *  in bench_stat::total() (much) faster.
 * */
#if(defined(__GNUC__) && !defined(__llvm__) && __cplusplus >= 201703L)
#include <execution>
#define Parallel std::execution::par_unseq, 
#else
#define Parallel
#endif

#include "ratio_to_string.hpp"
#include <functional> // std::invoke
#include <vector>     // std::vector (bench_stat)
#include <optional>   // bench_stat::property
#include <chrono>     // std::chrono::high_resolution_clock
#include <numeric>    // std::reduce
#include <algorithm>  // std::minmax_element
#include <iostream>   // std::cout (default parameter for print_stat)
#include <ostream>    // std::basic_ostream

/*
 * bench.hpp: talbi's simple benching utilities.
 *
 *  Usage. run `bench(trials, f, Args...)` to test the call f(Args...) for `trials` times.
 *         Returns a `bench_stat` struct which contains some information about the bench.
 *         Print a `bench_stat` using the standard operator<<, or access the data directly
 *         by accessing bench_stat.data, a field of std::vector<double>.
 *
 *         It is possible to change the measure time (e.g., measure time in nanoseconds instead 
 *         of microseconds by) changing the value of the template argument Period. In practice,
 *         it suffices to call bench<std::nano>(.) to measure in nanoseconds instead of the default
 *         microseconds. bench<Period>(.) should support all of the ratios defined in <ratio>,
 *         as well as ratio<60> (minute), ratio<3600> (hour) and ratio<86400> (day).
 *
 *         One may also access the fields min(), max(), avg() and total(). Once the respective
 *         functions have been called access is also possible using bench_stat.m_<property>.
 *
 *   Note. Compatible both with g++ and clang++. May produce faster code using g++ (parallel for_each, reduce) 
 * */

struct bench_stat {
    using property = std::optional<double>;

    const char *unit;
    std::vector<double> data;

    const std::size_t trials;
    property m_min; 
    property m_max;
    property m_avg;
    property m_total;

    bench_stat(const std::vector<double>& data, const char *unit) : data(data), trials(data.size()),
                                                                                     unit(unit) {}

    constexpr double min(void) {
        if(m_min) return m_min.value();
        this->get_minmax();
        return m_min.value();
    }

    constexpr double max(void) {
        if(m_max) return m_max.value();
        this->get_minmax();
        return m_max.value();
    }

    constexpr double avg(void) {
        if(m_avg) return m_avg.value();
        this->get_totalavg();
        return m_avg.value();
    }

    constexpr double total(void) {
        if(m_total) return m_total.value();
        this->get_totalavg();
        return m_total.value();
    }  

private:
    void get_minmax(void) {
        auto [min_, max_] = std::minmax_element(data.cbegin(), data.cend());
        m_min = *min_;
        m_max = *max_;
    }

    void get_totalavg(void) {
        m_total = std::reduce(Parallel data.cbegin(), data.cend());
        m_avg = m_total.value() / trials;
    }
};

#undef constexpr_if

template<class Period = std::milli,
            class F, class... Args>
bench_stat bench(std::size_t trials, F f, Args... args) {
    using hrc = std::chrono::high_resolution_clock;
    std::vector<double> data(trials);
    std::for_each(Parallel data.begin(), data.end(), [&](auto &i) { 
            auto before = hrc::now();
            std::invoke(f, args...);
            auto after = hrc::now();
            i = std::chrono::duration<double, Period>(after - before).count();
    });
    
    return bench_stat(data, ratio_to_string<Period>());
}

/*
 * prints a bench_stat object to the ostream `out`.
 *  Prints the following statistics:
 *    - Total execution time 
 *    - Average execution time
 *    - Minimal execution time
 *    - Maximum execution time
 *  with the corresponding unit, as defined by bench().
 * */

template<class Char, class Traits = std::char_traits<Char>>
auto& print_stat(bench_stat& data, std::basic_ostream<Char, Traits> &out = std::cout) {
    return out << "    Total: " << data.total() << data.unit << 's' << " (N = " << data.trials << ')' << '\n'
               << "  Average: " << data.avg() << data.unit << 's' << '\n'
               << "      Min: " << data.min() << data.unit << 's' << '\n'
               << "      Max: " << data.max() << data.unit << 's' << '\n';
}

template<class Char, class Traits = std::char_traits<Char>>
auto& print_stat(bench_stat &&data, std::basic_ostream<Char, Traits> &out = std::cout) {
    return print_stat(data, out);
}

/* Provided is also an operator<< */

template<class Char, class Traits = std::char_traits<Char>>
auto& operator<<(std::basic_ostream<Char, Traits> &out, bench_stat &data) {
    return print_stat(data, out);
}

template<class Char, class Traits = std::char_traits<Char>>
auto& operator<<(std::basic_ostream<Char,Traits> &out, bench_stat &&data) {
    return print_stat(data, out);
}

#undef Parallel
#endif //talbi__BENCH_HPP
