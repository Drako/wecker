#include "on_board_led.hxx"
#include "beeper.hxx"
#include "rgb_led.hxx"
#include "joystick.hxx"
#include "led.hxx"
#include "button.hxx"

#include <cstdio>

#include <pico/stdio.h>
#include <pico/multicore.h>

void init_onboard_led() {
    auto &led = OnBoardLed::get();
    led.on();
    sleep_ms(500u);
    led.off();
}

void init_beeper() {
    auto &beeper = Beeper::get();
    beeper.on();
    sleep_ms(200u);
    beeper.off();
}

void init_rgb_led() {
    auto &led = RgbLed::get();
    led.set_color(colors::RED);
    sleep_ms(200u);
    led.set_color(colors::GREEN);
    sleep_ms(200u);
    led.set_color(colors::BLUE);
    sleep_ms(200u);
    led.set_color(colors::WHITE);
    sleep_ms(200u);
    led.off();
}

void init_joystick() {
    auto &joystick = Joystick::get();
    joystick.update();
}

void init_leds() {
    auto &led16 = Led::get_gp16();
    auto &led17 = Led::get_gp17();

    for (int n = 0; n < 4; ++n) {
        led16.set_state((n & 1) == 1);
        led17.set_state((n & 2) == 2);
        sleep_ms(200u);
    }
    led16.off();
    led17.off();
}

void link_button_with_led(uint const button_pin, uint const led_pin) {
    auto const button = Button::get(button_pin);
    auto const led = Led::get(led_pin);
    if (!button || !led) { return; }

    button->set_on_press([button_pin, led] {
        std::printf("Button GP%d pressed!\n", button_pin);
        led->on();
    });
    button->set_on_release([button_pin, led] {
        std::printf("Button GP%d released!\n", button_pin);
        led->off();
    });
}

void init_buttons() {
    link_button_with_led(14, 16);
    link_button_with_led(15, 17);
}

[[noreturn]] void core1_main() {
    std::printf("Core %d: I'm alive!\n", get_core_num());

    for (;;) {
        Joystick::get().update();
        sleep_ms(100u);
    }
}

[[noreturn]] int main() {
    stdio_init_all();
    sleep_ms(1'000u);

    std::printf("Welcome to Wecker-OS!\n");
    std::printf("Initializing...\n");

    init_onboard_led();
    init_beeper();
    init_rgb_led();
    init_joystick();
    init_leds();
    init_buttons();

    multicore_launch_core1(&core1_main);
    std::printf("Core %d: I'm alive!\n", get_core_num());

    for (;;) {
        sleep_ms(1'000u);
        auto &joystick = Joystick::get();
        auto const raw_pos = joystick.get_raw_position();
        const auto [x, y] = Joystick::from_raw(raw_pos);
        std::printf("Joystick position: %d, %d\n", x, y);
        std::printf("Joystick position (raw): %d, %d\n", raw_pos.x, raw_pos.y);
    }
}
