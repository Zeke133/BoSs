/**
 * @file    led.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    01-03-2019
 * @brief   LED class
 * @details   LED class interface
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include "gpio.hpp"

/**
 * API for easy operations with signaling LEDs
 */
class LED {

public:

    LED(GPIO_TypeDef* port, uint32_t pin);

    /**
     * Delete copy constructor and assignment operator.
     * This class is not thread safe implemented
     * and copied instance will use same HW resources like original one.
     * This will lead to race-condition.
     */
    LED(const LED&) = delete;
    LED& operator=(const LED&) = delete;

    void on();
    void off();
    void invert();

private:

    enum struct State {
        OFF,
        ON
    } state;

    GPIO_TypeDef* port;
    uint32_t pin;

};
