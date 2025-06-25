#include "joystick.hxx"
#include "lock_guard.hxx"

#include <cstdio>

#include <hardware/adc.h>

Joystick::Joystick(): mutex_{} {
    adc_init();

    // Make sure GPIO is high impedance, no pullups etc (copied from pico_breadboard_kit example)
    adc_gpio_init(26);
    adc_gpio_init(27);

    mutex_init(&mutex_);

    std::puts(" - Joystick... OK");
}

Vec2<std::uint16_t> Joystick::get_raw_position() const {
    LockGuard lock{mutex_};

    adc_select_input(0);
    std::uint16_t const adc_x_raw = adc_read();
    adc_select_input(1);
    std::uint16_t const adc_y_raw = adc_read();

    return {adc_x_raw, adc_y_raw};
}

Vec2<int> Joystick::get_position() const {
    auto const [x, y] = get_raw_position();
    std::uint32_t const x32 = x;
    std::uint32_t const y32 = y;

    constexpr int value_range = MAX_VALUE * 2 + 1;

    // these have been determined empirically by pushing the stick and reading the raw values.
    constexpr std::uint32_t min_raw_x = 72; // should be 0
    constexpr std::uint32_t max_raw_x = 3829; // should be 4095
    constexpr std::uint32_t min_raw_y = 336; // should be 0
    constexpr std::uint32_t max_raw_y = 4062; // should be 4095
    // interestingly the center seems to work, so that will still be placed at coordinate 2048,2048

    std::uint32_t const x_fixed = x < 2048u
                                      ? ((x32 - min_raw_x) << 11) / (2048u - min_raw_x)
                                      : ((x32 - 2048u) << 11) / (max_raw_x - 2048u) + 2048u;
    std::uint32_t const y_fixed = y < 2048u
                                      ? ((y32 - min_raw_y) << 11) / (2048u - min_raw_y)
                                      : ((y32 - 2048u) << 11) / (max_raw_y - 2048u) + 2048u;

    constexpr std::uint32_t adc_max = (1 << 12) - 1;
    int const bar_x_pos = static_cast<int>(x_fixed * value_range / adc_max) - MAX_VALUE;
    int const bar_y_pos = static_cast<int>(y_fixed * value_range / adc_max) - MAX_VALUE;

    return {bar_x_pos, bar_y_pos};
}
