#include <led.h>

LED::LED(GPIO_TypeDef* port, uint32_t pin)
    : port(port), pin(pin) {

    GPIO::enableAPB2Clock(port);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(port, pin, LL_GPIO_OUTPUT_PUSHPULL);
    
    // LL_GPIO_ResetOutputPin(port, pin);
    state = 1;
}

inline void LED::on() {

    LL_GPIO_ResetOutputPin(port, pin);
    state = 1;
}

inline void LED::off() {

    LL_GPIO_SetOutputPin(port, pin);
    state = 0;
}

void LED::invert() {

    state ? off() : on();
}
