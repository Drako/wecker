#pragma once

#include <type_traits>

template<typename ValueType>
struct Vec2 {
    using value_type = ValueType;

    value_type x, y;

    template<typename Other>
    explicit constexpr operator Vec2<Other>() const {
        return {static_cast<Other>(x), static_cast<Other>(y)};
    }

    constexpr Vec2 operator+(Vec2 const &rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    constexpr Vec2 operator-(Vec2 const &rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    constexpr Vec2 operator*(value_type const &rhs) const {
        return {x * rhs, y * rhs};
    }

    constexpr Vec2 operator/(value_type const &rhs) const {
        return {x / rhs, y / rhs};
    }

    template<typename T=value_type, std::enable_if_t<std::is_signed_v<T>, int>  = 0>
    constexpr Vec2 operator-() const {
        return {-x, -y};
    }
};
