# bench.hpp - Simple C++ benching utility

`bench.hpp` is a simple library I wrote to allow me to quickly time function runtime, whilst offering high customizability.

The library extensively uses the excellent `<chrono>` standard library to perform all time measurement, and more specifically uses `std::invoke` in order to call functions.

Requires C++17 to compile and run.

## Limitations

The main limitation comes when attempting to bench a function without anything else. Because a modern CPU operates using cache, the initial running time of a function is much higher then if one runs multiple iterations of it, because of cache misses. As such, if you need to bench several functions, it is probably best to "warmup" with some random calculations:

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

These changes would lead to a much more accurate value of the maximum running time. If the maximal running time for a single call is not important, it is also possible to avoid this problem by setting `trials` to a large enough value.

## Examples

In the `bench.hpp` header there are two functions: `bench` and `print_stat`. Benching has the signature `bench(trials, f, args...)` and offers a template type called `Period`, which by default is set to `std::milli`. Setting `Period` to any ratio defined in `<ratio>` (specifically, from `atto` to `exa`) will result in a measurement in the unit `Period`. For example:

```c++
bench(.); // measures in milliseconds
bench<std::nano>(.); // measures in nanoseconds
bench<std::ratio<1>>(.); // measures in seconds
bench<std::ratio<60>>(.); // measures in minutes
```

In the case where the macro `talbi__DISABLE_TIME_SPECIALIZATIONS` is *not* defined, printing a `bench_stat` obtained by the periods `std::ratio<60>` (minutes), `std::ratio<3600>` (hours) and `std::ratio<86400>` (days) is also possible.

Printing a `bench_stat` object will output the information:
- Total running time, as well as the number of trials
- Maximal running time over all trials
- Minimal running time over all trials
- Average running time 

If needed, it is possible to save the `bench_stat` object and only print/manipulate those values, using the respective `.<property>()` methods.



