/**
 * @file    led.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    01-03-2019
 * @brief   LED class
 * @details   LED class implementation
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#include "led.hpp"

/**
 * @brief  LED class constructor
 *         Initialize APB2 clock and setup port.
 *         Set LED ON
 * @param  port GPIO Port
 * @param  pin to be used
 * @retval None
 */
LED::LED(GPIO_TypeDef* port, uint32_t pin)
    : port(port), pin(pin) {

    GPIO::enableAPB2Clock(port);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(port, pin, LL_GPIO_OUTPUT_PUSHPULL);
    
    LL_GPIO_ResetOutputPin(port, pin);
    state = State::ON;
}

void LED::on() {

    LL_GPIO_ResetOutputPin(port, pin);
    state = State::ON;
}

void LED::off() {

    LL_GPIO_SetOutputPin(port, pin);
    state = State::OFF;
}

void LED::invert() {

    if(state == State::ON) off();
    else on();
}
