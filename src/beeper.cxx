#include "beeper.hxx"
#include "lock_guard.hxx"

#include <cstdio>

#include <hardware/gpio.h>

Beeper::Beeper() : mutex_{} {
    gpio_init(PIN);
    gpio_set_dir(PIN, GPIO_OUT);
    mutex_init(&mutex_);
    std::puts(" - Beeper... OK");
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Beeper::set_state(bool const state) {
    LockGuard lock{mutex_};
    gpio_put(PIN, state);
}

bool Beeper::get_state() const {
    LockGuard lock{mutex_};
    return gpio_get(PIN);
}
