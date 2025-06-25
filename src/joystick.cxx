#include "joystick.hxx"
#include "lock_guard.hxx"

#include <cstdio>

#include <hardware/adc.h>

Joystick::Joystick(): raw_position_{}, mutex_{} {
    adc_init();

    // Make sure GPIO is high impedance, no pullups etc. (copied from pico_breadboard_kit example)
    adc_gpio_init(26);
    adc_gpio_init(27);

    mutex_init(&mutex_);

    std::puts(" - Joystick... OK");
}

Vec2<int> Joystick::from_raw(Vec2<std::uint16_t> const &raw) {
    constexpr int BAR_MAX = (BAR_WIDTH >> 1) - 1;
    int const bar_x_pos = (raw.x * BAR_WIDTH / ADC_MAX) - BAR_MAX;
    int const bar_y_pos = (raw.y * BAR_WIDTH / ADC_MAX) - BAR_MAX;
    return {bar_x_pos, bar_y_pos};
}

void Joystick::update() {
    LockGuard lock{mutex_};

    adc_select_input(0);
    std::uint16_t const adc_x_raw = adc_read();
    adc_select_input(1);
    std::uint16_t const adc_y_raw = adc_read();

    raw_position_ = {adc_x_raw, adc_y_raw};
}
