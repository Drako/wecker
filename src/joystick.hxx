#pragma once

#include "singleton.hxx"
#include "vec2.hxx"

#include <cstdint>

#include <pico/sync.h>

class Joystick final : public Singleton<Joystick> {
public:
    constexpr static int MAX_VALUE = 128;

    Vec2<std::uint16_t> get_raw_position() const;

    // values range from -MAX_VALUE to MAX_VALUE
    Vec2<int> get_position() const;

private:
    Joystick();

    friend class Singleton;

    mutable mutex_t mutex_;
};
