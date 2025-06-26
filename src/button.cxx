#include "button.hxx"

#include <cstdio>

#include <hardware/gpio.h>

Button::Button(uint const pin): pin_{pin}, is_pressed_{false} {
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &Button::gpio_callback);

    std::printf(" - Button GP%d... OK\n", pin);
}

void Button::gpio_callback(uint const pin, uint32_t const events) {
    auto const button = get(pin);
    if (!button) { return; }

    button->is_pressed_ = (events & GPIO_IRQ_EDGE_FALL) != 0;
    if (button->is_pressed_) {
        if (button->on_press_)
            button->on_press_();
    } else {
        if (button->on_release_)
            button->on_release_();
    }
}

Button &Button::get_gp14() {
    static Button button{14};
    return button;
}

Button &Button::get_gp15() {
    static Button button{15};
    return button;
}

Button *Button::get(uint const pin) {
    switch (pin) {
        case 14: return &get_gp14();
        case 15: return &get_gp15();
        default: return nullptr;
    }
}
