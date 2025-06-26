#pragma once

#include <functional>

#include <pico/types.h>

class Button final {
public:
    Button(Button const &) = delete;

    Button &operator=(Button const &) = delete;

    static Button &get_gp14();

    static Button &get_gp15();

    static Button *get(uint pin);

    void set_on_press(std::function<void()> &&on_press) {
        on_press_ = on_press;
    }

    void set_on_release(std::function<void()> &&on_release) {
        on_release_ = on_release;
    }

    [[nodiscard]] bool get_state() const {
        return is_pressed_;
    }

private:
    explicit Button(uint pin);

    static void gpio_callback(uint pin, uint32_t events);

    uint const pin_;
    std::function<void()> on_press_;
    std::function<void()> on_release_;
    bool is_pressed_;
};
