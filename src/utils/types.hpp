#pragma once

#include <cstdint>
#include <limits>
#include <stdfloat>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;   // Соответствует стандартному float
using f64 = double;  // Соответствует стандартному double

using size_t = std::size_t;

using Entity = size_t;
constexpr size_t INVALID_ENTITY = std::numeric_limits<Entity>::max();
constexpr size_t INVALID_COMPONENT = std::numeric_limits<size_t>::max();
