
# ProgLED Examples

Examples for ProgLED library can be found in [3D Clock firmware](https://github.com/silvio3105/3DCLK-FW). Code is located in `Core/Src/LED.cpp` file.
To drive LEDs, PWM with DMA is used. Signal(PWM) frequency is 800kHz(**depends on LED chip**) and auto reload value is 10. Reload value of `3` represents PWM duty cycle of 30%.
200us reset code is replaced with pull down resistor on output pin.
SPI also can drive LEDs. [Click here to see more](https://www.newinnovations.nl/post/controlling-ws2812-and-ws2812b-using-only-stm32-spi/).