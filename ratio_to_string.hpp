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

#define talbi__func template<> constexpr const char* ratio_to_string

talbi__func<std::atto>(void) { return "a"; }
talbi__func<std::femto>(void) { return "f"; }
talbi__func<std::pico>(void) { return "p"; }
talbi__func<std::nano>(void) { return "n"; }
talbi__func<std::micro>(void) { return "µ"; }
talbi__func<std::milli>(void) { return "m"; }
talbi__func<std::centi>(void) { return "c"; }
talbi__func<std::deci>(void) { return "d"; }
talbi__func<std::ratio<1>>(void) { return ""; }
talbi__func<std::deca>(void) { return "da"; }
talbi__func<std::hecto>(void) { return "h"; }
talbi__func<std::kilo>(void) { return "k"; }
talbi__func<std::mega>(void) { return "M"; }
talbi__func<std::giga>(void) { return "G"; }
talbi__func<std::tera>(void) { return "T"; }
talbi__func<std::peta>(void) { return "P"; }
talbi__func<std::exa>(void) { return "E"; }

#ifndef talbi__DISABLE_TIME_SPECIALIZATIONS
talbi__func<std::ratio<60>>(void) { return "min"; }
talbi__func<std::ratio<3600>>(void) { return "h"; }
talbi__func<std::ratio<86400>>(void) { return "day"; }
#endif

#undef talbi__func

template<class R>
constexpr const char* ratio_to_string(void) {
    static_assert(talbi::static_false<R>::value,
        "talbi::ratio_to_string() is not implemented for custom ratio types (i.e., those not defined by <ratio>)");
    return nullptr;
}


#endif //talbi_RATIO_TO_STRING_HPP
