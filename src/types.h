#pragma once

#include <memory>

/**
 * @brief Heap-stored smart pointer.
 * Automically frees itself once it goes out of scope.
 */
template<class T>
using Box = std::unique_ptr<T>;

/**
 * @brief Heap-stored smart pointer.
 * Automically frees itself once all references to it go out of scope.
 */
template<class T>
using Rc = std::shared_ptr<T>;

#include <stdint.h>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;
