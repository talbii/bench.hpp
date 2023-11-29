# `bench.hpp` is a simple C++ benching utility

`bench.hpp` is a simple library I wrote to allow me to quickly time function runtime, whilst offering high customizability.

The library extensively uses the excellent `<chrono>` standard library to perform all time measurement, and more specifically uses `std::invoke` in order to call functions.

Requires C++17 to compile and run.

## Limitations

A limitation comes when one attempts to benchmark multiple functions in a single program. Because of how a modern CPU operates, things like cache, memory layout, and many more can cause initial inaccurate benchmarking results. One such fix, is to simply set the `trials` variable to be larger, however that may not always be possible.

The table below demonstrates a simple way to generally improve results, by running a short "warming-up" loop, which calls all functions in the benchmark, before performing said benchmark.

<table>
<tr>
<td> Bad </td> <td> Good </td>
</tr>
<tr>
<td>

```c++
auto r1 = bench(trials, f1, args1...),
     r2 = bench(trials, f2, args2...),
     r3 = bench(trials, f3, args3...);
...
```

</td>
<td>
    
```c++
for(auto i = 0; i < 1'000u; ++i) { // warmup cache by calling all functions
    f1(args1...);
    f2(args2...);
    f3(args3...);
    ...
}

// actual benching
auto r1 = bench(trials, f1, args1...),
     r2 = bench(trials, f2, args2...),
     r3 = bench(trials, f3, args3...);
...
```
</td>
</tr>
</table>

These changes would lead to a much more accurate value of the *maximum* running time. If such metric is not important, it is also possible to avoid this problem by setting `trials` to a large enough value.

Another limitation is the performance of the clock `steady_clock`. It may impact your performance, and actually eat a good chunk of the time spent computing. Read more in 'Clock' section.

## Basic Usage

Include `<bench.hpp>` (it is a header-only library). Benchmarking is done using `bench<Period>(trials, f, args...)` (with `Period` being any ratio defined in `<ratio>` such as `std::micro`, `std::milli`, etc.). Returned is `bench_stat` (just save it using `auto`, or just print it straight away). Printing is possible using either `print_stat(std::ostream&, bench_stat&&)` or an `operator<<`.

### Okay, and in more detail?

In the `bench.hpp` header there are two functions: `bench` and `print_stat`. Benching has the signature `bench(trials, f, args...)` and offers a template type called `Period`, which by default is set to `std::milli`. Setting `Period` to any ratio defined in `<ratio>` (specifically, from `atto` to `exa`) will result the measurements to be stored in the unit `Period`. For example:

```c++
bench(.); // default, stores results milliseconds
bench<std::nano>(.); // stores results in nanoseconds
bench<std::ratio<1>>(.); // stores results in seconds
bench<std::ratio<60>>(.); // stores results in minutes
```

In the case where the macro `talbi__DISABLE_TIME_SPECIALIZATIONS` is *not* defined, printing a `bench_stat` obtained by the periods `std::ratio<60>` (minutes), `std::ratio<3600>` (hours) and `std::ratio<86400>` (days) is also possible.

Printing a `bench_stat` object will output the information:

- Total running time, as well as the number of trials
- Maximal running time over all trials
- Minimal running time over all trials
- Average running time

If needed, it is possible to save the `bench_stat` object and only print/manipulate those values, using the respective `.<property>()` methods.

### Minimal working example

```c++
constexpr std::size_t trials = 100'000u;
using P1 = std::milli; // output is in milliseconds
using P2 = std::micro; // output is in microseconds

// with saving:
auto b1 = bench<P1>(trials, f1, arg1, arg2, ...);
std::cout << "f1 took at most " << b1.max() << ' ' << b1.unit << "s\n";

// without saving:
std::cout << "f2 benchamrk:\n" << bench<P2>(trials, f2, arg1, arg2, ...) << '\n';
```

### Clock

By default, clock measurement is done using `std::chrono::steady_clock`. This is because `steady_clock` guarantees monotonic time, which is especially important when benchmarking fast tasks. Previously, `std::chrono::high_resolution_clock` was used. It is possible to revert to this old behavior using `#define BENCH_LEGACY`.

### Actual Code Example

Code examples are available under the directory [examples/](examples/).

