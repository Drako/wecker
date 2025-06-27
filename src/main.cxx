#include "on_board_led.hxx"
#include "beeper.hxx"
#include "rgb_led.hxx"
#include "joystick.hxx"
#include "led.hxx"
#include "button.hxx"

#include <cstdio>

#include <pico/stdio.h>
#include <pico/multicore.h>
#include <pico/time.h>

#include <hardware/rtc.h>

#include <lvgl.h>
#include "../lv_port_disp.h"
#include "../lv_port_indev.h"

#include <hw_config.h>
#include <f_util.h>
#include <ff.h>

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

static lv_obj_t *datetime_label = nullptr;
static datetime_t previous_datetime{};

void update_datetime_label() {
    if (!datetime_label) return;

    datetime_t dt{};
    if (!rtc_get_datetime(&dt)) return;

    if (dt.year != previous_datetime.year
        || dt.month != previous_datetime.month
        || dt.day != previous_datetime.day
        || dt.hour != previous_datetime.hour
        || dt.min != previous_datetime.min
        || dt.sec != previous_datetime.sec
    ) {
        previous_datetime = dt;
        lv_label_set_text_fmt(datetime_label, "%02d.%02d.%04d %02d:%02d:%02d",
                              dt.day, dt.month, dt.year,
                              dt.hour, dt.min, dt.sec
        );
    }
}

void init_ui() {
    auto const screen = lv_scr_act();

    lv_obj_clean(screen);

    lv_obj_t *beep_btn = lv_btn_create(screen);
    lv_obj_add_event_cb(beep_btn, [](lv_event_t *const evt) {
        if (lv_event_get_code(evt) == LV_EVENT_VALUE_CHANGED) {
            auto const beeper = static_cast<Beeper *>(evt->user_data);
            auto const button = lv_event_get_target(evt);
            beeper->set_state((button->state & LV_STATE_CHECKED) != 0);
        }
    }, LV_EVENT_ALL, &Beeper::get());
    lv_obj_align(beep_btn, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_add_flag(beep_btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_height(beep_btn, LV_SIZE_CONTENT);

    lv_obj_t *label = lv_label_create(beep_btn);
    lv_label_set_text(label, "Beep");
    lv_obj_center(label);

    static lv_style_t label_style;
    lv_style_init(&label_style);
    lv_style_set_text_font(&label_style, &lv_font_montserrat_32);

    datetime_label = lv_label_create(screen);
    lv_obj_add_style(datetime_label, &label_style, LV_PART_MAIN);
    update_datetime_label();
    lv_obj_center(datetime_label);
}

void read_sd_card() {
    sd_init_driver();

    FATFS fs{};
    if (FRESULT const result = f_mount(&fs, "0:", 1); result != FR_OK) {
        std::printf(" - SD Card... FAIL (f_mount: %s)\n", FRESULT_str(result));
        return;
    }

    DIR root_dir{};
    if (FRESULT const result = f_opendir(&root_dir, "/"); result != FR_OK) {
        std::printf(" - SD Card... FAIL (f_opendir: %s)\n", FRESULT_str(result));
        return;
    }

    FILINFO file_info{};
    while (f_readdir(&root_dir, &file_info) == FR_OK && file_info.fname[0] != 0) {
        std::printf("%s: %c\n", file_info.fname, file_info.fattrib & AM_DIR ? 'd' : 'f');
    }

    f_closedir(&root_dir);
    f_unmount("");

    std::puts(" - SD Card... OK");
}

[[noreturn]] void core1_main() {
    std::printf("Core %d: I'm alive!\n", get_core_num());

    for (;;) {
        Joystick::get().update();
        lv_timer_handler();
        sleep_ms(100u);
    }
}

[[noreturn]] int main() {
    stdio_init_all();
    sleep_ms(1'000u);

    std::printf("Welcome to Wecker-OS!\n");
    std::printf("Initializing...\n");

    constexpr datetime_t epoch{1970, 1, 1, 4, 0, 0, 0};
    rtc_init();
    rtc_set_datetime(&epoch);
    while (!rtc_running()) {
        sleep_ms(100u);
    }
    std::puts(" - RTC... OK");

    init_onboard_led();
    init_beeper();
    init_rgb_led();
    init_joystick();
    init_leds();
    init_buttons();

    read_sd_card();

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    init_ui();

    multicore_launch_core1(&core1_main);
    std::printf("Core %d: I'm alive!\n", get_core_num());

    absolute_time_t previous = get_absolute_time();
    for (;;) {
        absolute_time_t const start = get_absolute_time();
        auto const elapsed = static_cast<std::uint32_t>(absolute_time_diff_us(previous, start) / 1000ull);
        previous = start;
        update_datetime_label();
        lv_tick_inc(elapsed);
        sleep_ms(50u);
    }
}
