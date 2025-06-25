#pragma once

#include "singleton.hxx"

#include <pico/sync.h>

class OnBoardLed final : public Singleton<OnBoardLed> {
public:
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
    OnBoardLed();

    friend class Singleton;

    bool cyw43_working_;
    mutable mutex_t mutex_;
};
