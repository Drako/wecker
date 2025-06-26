#pragma once

#include <pico/types.h>

class Led final {
public:
    Led(Led const &) = delete;

    Led &operator=(Led const &) = delete;

    static Led &get_gp16();

    static Led &get_gp17();

    static Led *get(uint pin);

    void set_state(bool state);

    [[nodiscard]] bool get_state() const;

    void on() {
        set_state(true);
    }

    void off() {
        set_state(false);
    }

    void toggle() {
        set_state(!get_state());
    }

    [[nodiscard]] bool is_on() const {
        return get_state();
    }

    [[nodiscard]] bool is_off() const {
        return !get_state();
    }

private:
    explicit Led(uint pin);

    uint const pin_;
};
