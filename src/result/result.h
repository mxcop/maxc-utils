#pragma once

/* Inspired by <https://github.com/oktal/result> */

#include <functional>
#include <type_traits>

namespace types {

template <typename T>
struct ok {
    ok(const T& val) : val(val) {}
    ok(T&& val) : val(std::move(val)) {}

    T val;
};

template <>
struct ok<void> {};

template <typename E>
struct err {
    err(const E& val) : val(val) {}
    err(E&& val) : val(std::move(val)) {}

    E val;
};

}  // namespace types

/* Functions for making an Ok result */
template <typename T, typename CleanT = typename std::decay<T>::type>
inline types::ok<CleanT> ok(T&& val) {
    return types::ok<CleanT>(std::forward<T>(val));
}

inline types::ok<void> ok() { return types::ok<void>(); }

/* Functions for making an Err result */
template <typename E, typename CleanE = typename std::decay<E>::type>
inline types::err<CleanE> err(E&& val) {
    return types::err<CleanE>(std::forward<E>(val));
}

namespace hidden {

/* Type tags */
struct ok_tag {};
struct err_tag {};

/* Storage structure for Type OR Error */
template <typename T, typename E>
struct Storage {
    static constexpr size_t Size = sizeof(T) > sizeof(E) ? sizeof(T) : sizeof(E);
    static constexpr size_t Align = sizeof(T) > sizeof(E) ? alignof(T) : alignof(E);

    typedef typename std::aligned_storage<Size, Align>::type type;

    Storage() {}

    void init(types::ok<T> ok) { new (&data) T(ok.val); }
    void init(types::err<E> err) { new (&data) E(err.val); }

    template <typename U>
    void raw_init(U&& val) {
        typedef typename std::decay<U>::type CleanU;

        new (&data) CleanU(std::forward<U>(val));
    }

    template <typename U>
    const U& get() const {
        return *reinterpret_cast<const U*>(&data);
    }

    template <typename U>
    U& get() {
        return *reinterpret_cast<U*>(&data);
    }

    void destroy(ok_tag) { get<T>().~T(); }

    void destroy(err_tag) { get<E>().~E(); }

    type data = {};
};

/* Storage structure for Void OR Error */
/* So we can have errors that have no return type */
template <typename E>
struct Storage<void, E> {
    typedef typename std::aligned_storage<sizeof(E), alignof(E)>::type type;

    void init(types::ok<void> ok) {}
    void init(types::err<E> err) { new (&data) E(err.val); }

    template <typename U>
    void raw_init(U&& val) {
        typedef typename std::decay<U>::type CleanU;

        new (&data) CleanU(std::forward<U>(val));
    }

    template <typename U>
    const U& get() const {
        return *reinterpret_cast<const U*>(&data);
    }

    template <typename U>
    U& get() {
        return *reinterpret_cast<U*>(&data);
    }

    void destroy(ok_tag) {}

    void destroy(err_tag) { get<E>().~E(); }

    type data = {};
};

/* Constructor for Ok and Err tagged storage */
template <typename T, typename E>
struct Constructor {
    static void move(Storage<T, E>&& src, Storage<T, E>& dst, ok_tag) {
        dst.raw_init(std::move(src.template get<T>()));
        src.destroy(ok_tag());
    }

    static void copy(const Storage<T, E>& src, Storage<T, E>& dst, ok_tag) { dst.raw_init(src.template get<T>()); }

    static void move(Storage<T, E>&& src, Storage<T, E>& dst, err_tag) {
        dst.raw_init(std::move(src.template get<E>()));
        src.destroy(err_tag());
    }

    static void copy(const Storage<T, E>& src, Storage<T, E>& dst, err_tag) { dst.raw_init(src.template get<E>()); }
};

}  // namespace hidden

/**
 * @brief Rust inspired Result<T, E> type for propagating errors.
 */
template <typename T, typename E>
struct result {
    static_assert(!std::is_same<E, void>::value, "void error type is not allowed");

    typedef hidden::Storage<T, E> storage_type;

    result(types::ok<T> ok) : ok(true) { storage.init(std::move(ok)); }
    result(types::err<E> err) : ok(false) { storage.init(std::move(err)); }

    result(result&& other) {
        if (other.is_ok()) {
            hidden::Constructor<T, E>::move(std::move(other.storage), storage, hidden::ok_tag());
            ok = true;
            return;
        }
        hidden::Constructor<T, E>::move(std::move(other.storage), storage, hidden::err_tag());
        ok = false;
    }

    result(const result& other) {
        if (other.is_ok()) {
            hidden::Constructor<T, E>::copy(other.storage, storage, hidden::ok_tag());
            ok = true;
            return;
        }
        hidden::Constructor<T, E>::copy(other.storage, storage, hidden::err_tag());
        ok = false;
    }

    ~result() {
        if (ok) {
            storage.destroy(hidden::ok_tag());
            return;
        }
        storage.destroy(hidden::err_tag());
    }

    /**
     * @return True if the result contains a value.
     */
    inline bool is_ok() const { return ok; }
    /**
     * @return True if the result contains an error.
     */
    inline bool is_err() const { return not ok; }

    /**
     * @brief [UNSAFE] Unsafe way to extract value from result.
     * (Will terminate the program if the result is an error)
     */
    T unwrap() const {
        if (is_err()) {
            std::fprintf(stderr, "unwrap failed!\n");
            std::terminate();
        }
        return expect_impl(std::is_same<T, void>());
    }

    /**
     * @brief [UNSAFE] Unsafe way to extract value from result.
     * (Will print message and terminate the program if the result is an error)
     */
    T expect(const char* str) const {
        if (is_err()) {
            std::fprintf(stderr, "%s\n", str);
            std::terminate();
        }
        return expect_impl(std::is_same<T, void>());
    }

   private:
    T expect_impl(std::true_type) const {}
    T expect_impl(std::false_type) const { return storage.template get<T>(); }

    bool ok = false;
    storage_type storage;
};
