# Notes

This is just a list of things I found.

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
