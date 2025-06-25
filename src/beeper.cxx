#include "beeper.hxx"

#include <cstdio>

#include <hardware/gpio.h>

Beeper::Beeper() : mutex_{} {
    gpio_init(PIN);
    gpio_set_dir(PIN, GPIO_OUT);
    mutex_init(&mutex_);
    std::puts(" - Beeper... OK");
}

Beeper &Beeper::get() {
    static Beeper instance_{};
    return instance_;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Beeper::set_state(bool const state) {
    mutex_enter_blocking(&mutex_);
    gpio_put(PIN, state);
    mutex_exit(&mutex_);
}

bool Beeper::get_state() const {
    mutex_enter_blocking(&mutex_);
    bool const result = gpio_get(PIN);
    mutex_exit(&mutex_);
    return result;
}
