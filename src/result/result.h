#pragma once

#include <cassert>

#include "error.h"

/**
 * @brief Project standard result template.
 */
template<class T>
class result {
    using E = error;

    const bool has_value = false;
    const union {
        T value;
        E* error;
    };

public:
    /* Construct result from its inner value type. */
    template<class U = T>
    constexpr result(U&& v) : value(v), has_value(true) {};
    template<class U = T>
    constexpr result(U& v) : value(v), has_value(true) {};

    constexpr result(E&& error) : error(new E(error)), has_value(false) {};
    constexpr result(E& error) : error(new E(error)), has_value(false) {};

    ~result() { if (not has_value && error) delete error; };

    /* Explicit way to create result from its inner value type */
    template<class U = T>
    static result<U> ok(U& v) { return result<U>(v); };

    /* Explicit way to create result as error */
    static result<T> err(E&& e) { return result<T>(e); };
    static result<T> err(E& e) { return result<T>(e); };

    /**
     * @brief [UNSAFE] Unsafe way to extract value from result.
     */
    T unwrap() const {
        if (has_value) return value;

        if (error) {
            printf("Unwrap failed! [%s]\n  \"%s\"\n", error->loc.data(), error->msg.data());
            abort(); /* Exit non-gracefully */
        }
        assert(!"unwrap failed!"); /* Crash! */
    }
};

/** Error shortcut macro */
#define err(msg) (error(msg, __FILE__, __func__, __LINE__))
