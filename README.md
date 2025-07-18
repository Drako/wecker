# Wecker

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

This is an alarm (German "Wecker") built around the
[Raspberry Pi Pico W](https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html#picow-technical-specification)
and the [Pico Breadboard Kit](https://wiki.52pi.com/index.php?title=EP-0172).

It is mostly a learning project to get comfortable with microcontroller programming and
communication with all kinds of peripherals.

**What works so far:**

- Talking to the computer via USB CDC (currently just used for logging).
- Talking to the builtin LED of the Pico W (read/write)
- Talking to the RGB LED on the breadboard kit (write-only)
- Talking to the Beeper on the breadboard kit (read/write)
- Talking to the Joystick (read-only)
- Talking to the simple LEDs at GP16 and GP17 (read/write)
- Talking to the simple Buttons at GP14 and GP15 via IRQs (read-only)
- Showing UI on the display using LVGL
- Settings and reading the RTC

Most likely, I won't be using all the features for the final result.

**Current plan:**

- Get the touch screen running to have a configuration UI for setting the current time and the wake-up time.
- Use the RTC (apparently there is one on the Pico W itself, not sure about persistence though...) to keep track of
  time.
- Use the beeper to wake me up (it is annoying as hell).
- Use one of the buttons or the touch screen to get it to shut up.

Everything else is just for experimentation and learning.

Currently, I'm considering actually using the Wi-Fi chip to initialize the RTC with NTP.
That might be a bit more complicated, though.
