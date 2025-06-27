# Used Pins

The following pins are already used either on-board or via the breakboard kit:

- GP12: RGB LED
- GP13: Beeper
- GP14 & GP15: Simple buttons
- GP16 & GP17: Simple LEDs
- GP26 & GP27: Joystick (GP26 = ADC0 = X, GP27 = ADC1 = Y)

The display alone uses the following pins (basically 2–11):

- GP2 (SCLK), GP3 (COPI), GP4 (POCI), GP5 (CS) for display via SPI
    - GP4 doesn't seem to be used
- GP6 (DC) & GP7 (RST) also for display, but not sure how exactly they work
- GP8 (SDA) & GP9 (SCL) for touch input via I2C
- GP10 (TPRST) & GP11 (TPINT) for touch input, but not sure how exactly they work
    - these two are not used by any code from the pico_breadboard_kit        
