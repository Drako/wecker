#pragma once

#include "color.hxx"
#include "singleton.hxx"

#include <pico/sync.h>

class RgbLed final : public Singleton<RgbLed> {
public:
    void set_color(Color const &color);

    void off() {
        set_color(colors::BLACK);
    }

private:
    RgbLed();

    friend class Singleton;

    bool ws2812_working_;
    mutable mutex_t mutex_;
};
