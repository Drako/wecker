#pragma once

#include "color.hxx"

#include <pico/sync.h>

class RgbLed final {
public:
    static RgbLed &get();

    void set_color(Color const &color);

    void off() {
        set_color(colors::BLACK);
    }

    RgbLed(RgbLed const &) = delete;

    RgbLed &operator=(RgbLed const &) = delete;

private:
    RgbLed();

    bool ws2812_working_;
    mutable mutex_t mutex_;
};
