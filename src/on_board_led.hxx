#pragma once

#include <pico/sync.h>

class OnBoardLed final {
public:
    static OnBoardLed &get();

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

    OnBoardLed(OnBoardLed const &) = delete;

    OnBoardLed &operator=(OnBoardLed const &) = delete;

private:
    OnBoardLed();

    bool cyw43_working_;
    mutable mutex_t mutex_;
};
