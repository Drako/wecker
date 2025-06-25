#include "on_board_led.hxx"

#include <cstdio>

#include <pico/cyw43_arch.h>

OnBoardLed::OnBoardLed() : mutex_{} {
    std::puts(".............");
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

    mutex_enter_blocking(&mutex_);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
    mutex_exit(&mutex_);
}

bool OnBoardLed::get_state() const {
    if (!cyw43_working_) { return false; }

    mutex_enter_blocking(&mutex_);
    bool const result = cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
    mutex_exit(&mutex_);
    return result;
}
