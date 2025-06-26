#include "led.hxx"

#include <cstdio>

#include <hardware/gpio.h>

Led::Led(uint const pin): pin_{pin} {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);

    std::printf(" - LED GP%d... OK\n", pin);
}

Led &Led::get_gp16() {
    static Led led{16};
    return led;
}

Led &Led::get_gp17() {
    static Led led{17};
    return led;
}

Led *Led::get(uint const pin) {
    switch (pin) {
        case 16: return &get_gp16();
        case 17: return &get_gp17();
        default: return nullptr;
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Led::set_state(bool const state) { // NOLINT(*-make-member-function-const)
    gpio_put(pin_, state);
}

bool Led::get_state() const {
    return gpio_get(pin_);
}
