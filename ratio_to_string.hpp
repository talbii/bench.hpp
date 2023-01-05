#ifndef talbi_RATIO_TO_STRING_HPP
#define talbi_RATIO_TO_STRING_HPP

#include <string>
#include "static_false.hpp"
#include <ratio>

/*
 * Given a ratio type R, ratio_to_string<R>() will return the string representing that ratio,
 * e.g.:
 *  - ratio_to_string<std::micro>() => µ
 *  - ratio_to_string<std::milli>() => m
 *
 *  > suffixes come from here: https://en.cppreference.com/w/cpp/chrono/duration/operator_ltlt.
 *
 *  It might be handy to `#define talbi__DISABLE_TIME_SPECIALIZATIONS` before including if 
 *   one wishes to disable the template specializations for std::ratio<.>s defining minute, hour, day.
 *
 *  Since for some reason this is not implemented globally yet and only works for std::duration<.> .. :/
 * */

template<class R>
constexpr const char* ratio_to_string(void);

#define talbi__define_ratio_to_string(T, L) template<> constexpr const char* ratio_to_string<T>(void) { return L; }

talbi__define_ratio_to_string(std::atto, "a")
talbi__define_ratio_to_string(std::femto, "f")
talbi__define_ratio_to_string(std::pico, "p")
talbi__define_ratio_to_string(std::nano, "n") 
talbi__define_ratio_to_string(std::micro, "µ")
talbi__define_ratio_to_string(std::milli, "m")
talbi__define_ratio_to_string(std::centi, "c")
talbi__define_ratio_to_string(std::deci, "d")
talbi__define_ratio_to_string(std::ratio<1>, "")
talbi__define_ratio_to_string(std::deca, "da")
talbi__define_ratio_to_string(std::hecto, "h")
talbi__define_ratio_to_string(std::kilo, "k")
talbi__define_ratio_to_string(std::mega, "M")
talbi__define_ratio_to_string(std::giga, "G")
talbi__define_ratio_to_string(std::tera, "T")
talbi__define_ratio_to_string(std::peta, "P")
talbi__define_ratio_to_string(std::exa, "E")

#ifndef talbi__DISABLE_TIME_SPECIALIZATIONS
talbi__define_ratio_to_string(std::ratio<60>, "min")
talbi__define_ratio_to_string(std::ratio<3600>, "h")
talbi__define_ratio_to_string(std::ratio<86400>, "day")
#endif

#undef talbi__define_ratio_to_string

template<class R>
constexpr const char* ratio_to_string(void) {
    static_assert(talbi::static_false<R>::value,
        "talbi::ratio_to_string() is not implemented for custom ratio types (i.e., those not defined by <ratio>)");
    return nullptr;
}


#endif //talbi_RATIO_TO_STRING_HPP
