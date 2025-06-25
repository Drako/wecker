#include "rgb_led.hxx"

#include <cstdio>

#include <hardware/pio.h>
#include <ws2812.pio.h>

RgbLed::RgbLed(): mutex_{} {
    if (int const offset = pio_add_program(pio0, &ws2812_program); offset < 0) {
        ws2812_working_ = false;
        std::puts(" - RGB LED... FAIL");
    } else {
        ws2812_working_ = true;
        ws2812_program_init(pio0, 0, offset, 12, 800'000.0f, true);
        mutex_init(&mutex_);
        std::puts(" - RGB LED... OK");
    }
}

RgbLed &RgbLed::get() {
    static RgbLed instance_{};
    return instance_;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void RgbLed::set_color(Color const &color) {
    if (!ws2812_working_) { return; }

    mutex_enter_blocking(&mutex_);
    pio_sm_put_blocking(pio0, 0, color.to_ws2812());
    mutex_exit(&mutex_);
}
