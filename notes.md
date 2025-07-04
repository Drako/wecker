# Notes

This is just a list of things I found.

## Power Supply

Obviously, powering through micro USB works fine.
Powering through VSYS doesn't deliver enough power and results in the RGB LED not lighting up.
Powering through VBUS works, but I'm not sure if that's the right thing to do.

Also, I consider adding UART output, so I can still see the log output when not connected via micro USB.
I might check out the debug pins as well.

## Mutexes

Pico Mutexes (`mutex_t` and `recursive_mutex_t`) use spin locks (`spin_lock_t`).
There is a fixed number of these:

```c
#define NUM_SPIN_LOCKS _u(32)
```

So one would think one can only have 32 mutexes, but wrong, because, when initializing a mutex,
only striped spin locks are used, which are the spin locks with the numbers 16 to 23, leaving us with 8 mutexes.

But it doesn't stop there: when initializing a mutex, the spin locks will just wrap around, meaning the
nineth mutex and the first one will technically be the same mutex.

Looking at the different macros in `spin_lock.h` it turns out that you can reconfigure the different ranges,
and that these are used to have reserved spin locks for different kinds of applications.
Not having an OS, but being the OS myself, I can technically increase the number of available mutexes,
should I need to.

## Joystick

The Joystick gives ADC values from 0 to 4095, but actually it is not possible to push it physically so far
that 0 or 4095 would be read. So I tried to optimize the calculation of the x/y position, but I head to realize
that the range is not exactly stable. Basically, pushing the stick to an outermost position would yield one value,
and doing the same again later might result in an even bigger/lower value, meaning the range needs to be extended again.

So in the end, I went back to the value range and calculation found in the bread board kit example.

## LVGL

There have been some major changes between LVGL 8 and 9. The pico_breakboard_kit comes with LVGL 8.
As such, I'm using that as well for now. I might upgrade to LVGL 9, once I have a better understanding of it.

## RTC

To get the real time clock running, `rtc_init()` is not enough. One must also set an initial date time using
`rtc_set_datetime()`, and it must be a valid. For now, I set it to the UNIX epoch. However, maybe I can do a little startup sequence,
where I connect to Wi-Fi, grab the current date time from a time-server and then shut down the network stuff again.
