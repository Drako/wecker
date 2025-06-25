#pragma once

#include <cstdint>

struct Color {
    std::uint8_t r, g, b;

    [[nodiscard]] constexpr std::uint32_t to_ws2812() const {
        return static_cast<std::uint32_t>(r) << 16
               | static_cast<std::uint32_t>(g) << 24
               | static_cast<std::uint32_t>(b) << 8;
    }
};

namespace colors {
    constexpr static Color BLACK{};
    constexpr static Color RED{.r = 0xFF};
    constexpr static Color GREEN{.g = 0xFF};
    constexpr static Color BLUE{.b = 0xFF};
    constexpr static Color WHITE{0xFF, 0xFF, 0xFF};
}
