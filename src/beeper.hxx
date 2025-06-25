#pragma once

#include <pico/sync.h>
#include <pico/types.h>

class Beeper final {
public:
    constexpr static uint PIN = 13u;

    static Beeper &get();

    void set_state(bool state);

    bool get_state() const;

    void on() {
        set_state(true);
    }

    void off() {
        set_state(false);
    }

    void toggle() {
        set_state(!get_state());
    }

    bool is_on() const {
        return get_state();
    }

    bool is_off() const {
        return !get_state();
    }

    Beeper(Beeper const &) = delete;

    Beeper &operator=(Beeper const &) = delete;

private:
    Beeper();

    mutable mutex_t mutex_;
};
