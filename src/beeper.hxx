#pragma once

#include "singleton.hxx"

#include <pico/sync.h>
#include <pico/types.h>

class Beeper final : public Singleton<Beeper> {
public:
    constexpr static uint PIN = 13u;

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

private:
    Beeper();

    friend class Singleton;

    mutable mutex_t mutex_;
};
