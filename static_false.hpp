#ifndef talbi__STATIC_FALSE_HPP
#define talbi__STATIC_FALSE_HPP

namespace talbi {

    /*
     * static_false<T> is a struct which has the compile-time ::value of false.
     * 
     * Usage.    Use in template functions on which you don't want to provide implementations,
     *  for use with static_assert.
     * Rational. static_assert(false, str) will simply not compile, thus making the code below 
     *  to not compile:
     *
     * template<class T> auto f(const T&);
     *
     * template<>
     * auto f(const std::string& s) {
     *     std::cout << s << '\n';
     * }
     *  
     * template<class T>
     * auto f(const T&) {
     *     static_assert(false, "Not implemented.");
     * }
     *
     *
     *  replacing false with static_false<T>::value, however, will compile successfully.
     * */
    template<class T>
    struct static_false {
        static constexpr bool value = false;
    };

};

#endif //talbi__STATIC_FALSE_HPP
