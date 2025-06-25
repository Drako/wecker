#pragma once

#include "singleton.hxx"
#include "vec2.hxx"

#include <cstdint>

#include <pico/sync.h>

class Joystick final : public Singleton<Joystick> {
public:
    constexpr static int BAR_WIDTH = 40;
    constexpr static std::uint16_t ADC_MAX = 4096u;

    /**
     * Calculate the position of the joystick based on raw ADC values.
     *
     * @note If both positions are needed, use get_raw_position() and from_raw(),
     *       because there could be an update between get_raw_position() and get_position().
     *
     * @param raw The raw ADC values.
     * @return The joystick position.
     */
    static Vec2<int> from_raw(Vec2<std::uint16_t> const &raw);

    Vec2<std::uint16_t> get_raw_position() const {
        return raw_position_;
    }

    Vec2<int> get_position() const {
        return from_raw(raw_position_);
    }

    void update();

private:
    Joystick();

    friend class Singleton;

    Vec2<std::uint16_t> raw_position_;
    mutable mutex_t mutex_;
};
