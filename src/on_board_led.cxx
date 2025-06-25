#include "on_board_led.hxx"
#include "lock_guard.hxx"

#include <cstdio>

#include <pico/cyw43_arch.h>

OnBoardLed::OnBoardLed() : mutex_{} {
    cyw43_working_ = (cyw43_arch_init() == 0);
    if (!cyw43_working_) {
        std::puts(" - CYW43 LED... FAIL");
    } else {
        mutex_init(&mutex_);
        std::puts(" - CYW43 LED... OK");
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void OnBoardLed::set_state(bool const state) {
    if (!cyw43_working_) { return; }

    LockGuard lock{mutex_};
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
}

bool OnBoardLed::get_state() const {
    if (!cyw43_working_) { return false; }

    LockGuard lock{mutex_};
    return cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
}
