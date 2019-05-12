/**
 * @file    gpio.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    01-03-2019
 * @brief   GPIO interface
 * @details   General purpose input/output ports interface
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_gpio.h>

/**
 * API for general purpose IO ports usage
 */
namespace GPIO {

/**
 * @brief  Enable APB2 clock for specified port.
 * @param  port GPIO Port
 *         GPIOA
 *         GPIOB
 *         GPIOC
 *         GPIOD
 *         GPIOE
 * @retval None
 */
void enableAPB2Clock(GPIO_TypeDef * port);

/**
 * GPIO Ports working modes
 * ---
 * CNFy[1:0] MODEy[1:0] ... CNF0[1:0] MODE0[1:0]
 * ---
 * CNFy[1:0]: Port x configuration bits (y= 0 .. 7)
 * These bits are written by software to configure the corresponding I/O port.
 * Refer to Table 20: Port bit configuration table.
 * ---
 * In input mode (MODE[1:0]=00):
 * 00: Analog mode
 * 01: Floating input (reset state)
 * 10: Input with pull-up / pull-down
 * ---
 * In output mode (MODE[1:0] > 00):
 * 00: General purpose output push-pull
 * 01: General purpose output Open-drain
 * 10: Alternate function output Push-pull
 * 11: Alternate function output Open-drain
 * ---
 * MODEy[1:0]: Port x mode bits (y= 0 .. 7)
 * These bits are written by software to configure the corresponding I/O port.
 * Refer to Table 20: Port bit configuration table.
 * 00: Input mode (reset state)
 * 01: Output mode, max speed 10 MHz.
 * 10: Output mode, max speed 2 MHz.
 * 11: Output mode, max speed 50 MHz.
 */


/**
 * @brief  Configure gpio mode for a dedicated pin on dedicated port.
 * @note   I/O mode can be Analog, Floating input, Input with pull-up/pull-down, General purpose Output,
 *         Alternate function Output.
 * @note   Warning: only one pin can be passed as parameter.
 * @rmtoll CRL        CNFy         LL_GPIO_SetPinMode
 * @rmtoll CRL        MODEy        LL_GPIO_SetPinMode
 * @rmtoll CRH        CNFy         LL_GPIO_SetPinMode
 * @rmtoll CRH        MODEy        LL_GPIO_SetPinMode
 * @param  GPIOx GPIO Port
 * @param  Pin This parameter can be one of the following values:
 *         @arg @ref LL_GPIO_PIN_0
 *                   ...
 *         @arg @ref LL_GPIO_PIN_15
 * @param  Mode This parameter can be one of the following values:
 *         @arg @ref LL_GPIO_MODE_ANALOG
 *         @arg @ref LL_GPIO_MODE_FLOATING
 *         @arg @ref LL_GPIO_MODE_INPUT
 *         @arg @ref LL_GPIO_MODE_OUTPUT
 *         @arg @ref LL_GPIO_MODE_ALTERNATE
 * @retval None
 */
// try to make constexpr
// void initPin(GPIO_TypeDef * GPIOx, uint32_t pin, uint32_t mode, uint32_t speed = LL_GPIO_SPEED_FREQ_HIGH) {

//     /* Pin Mode configuration */
//     LL_GPIO_SetPinMode(GPIOx, pin, mode);

//     /* Pull-up Pull-down resistor configuration*/
//     // LL_GPIO_SetPinPull(GPIOx, pin, GPIO_InitStruct->Pull);

//     if ((mode == LL_GPIO_MODE_OUTPUT) || (mode == LL_GPIO_MODE_ALTERNATE))
//     {
//         /* Speed mode configuration */
//         LL_GPIO_SetPinSpeed(GPIOx, pin, speed);

//         /* Output mode configuration*/
//         LL_GPIO_SetPinOutputType(GPIOx, pin, GPIO_InitStruct->OutputType);
//     }
// };

// Lock pin mech() - will help for instanses created once on start of programm

}
