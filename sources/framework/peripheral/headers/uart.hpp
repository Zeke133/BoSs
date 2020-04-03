/**
 * @file    uart.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    18-04-2019
 * @brief   Uart class
 * @details   Implementation of UART driver
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include "ITextOutput.hpp"
#include "gpio.hpp"
#include "dma.hpp"

#include <stm32f1xx_ll_usart.h>

#include <array>

// IRQ handlers. Extern "C" macro is needed for correct link procedure.
extern "C" {
    void USART1_IRQHandler(void);
    void USART2_IRQHandler(void);
    void USART3_IRQHandler(void);
}

/**
 * UART driver class. Implements ITextOutput interface.
 * Each instance represents one USART peripheral device.
 * Class instance must be created with make_uart() function.
 * Here is no any hardware resources sharing protection,
 * so you must not create more than one instance for each physical device.
 * Possibility of using blocking and DMA based asynchronous transmition methods.
 */
class Uart : public ITextOutput {

    /**< Interrupt handlers need access to allocated buffers */
    friend void USART1_IRQHandler(void);
    friend void USART2_IRQHandler(void);
    friend void USART3_IRQHandler(void);

public:

    /**
     * @brief  Creates instance of class.
     *         Puts required configuration to constructor depending
     *         on user chosen device.
     * @param  uartN: USARTx device number 1 .. 3
     * @param  bauld: 115200
     * @param  dataBits: LL_USART_DATAWIDTH_8B
     * @param  stopBits: LL_USART_STOPBITS_1
     * @param  parity: LL_USART_PARITY_NONE
     * @retval Uart instance
     */
    template<uint8_t uartN>
    static constexpr auto make_uart(DMA& txDMA,
                                    uint32_t bauld = 115200,
                                    uint32_t dataBits = LL_USART_DATAWIDTH_8B,
                                    uint32_t stopBits = LL_USART_STOPBITS_1,
                                    uint32_t parity = LL_USART_PARITY_NONE) {
        
        /**
         * Table of device configurations.
         */
        constexpr std::array<Config, 3> configurationsTable = {{

            {USART1, GPIOA, LL_GPIO_PIN_9, LL_GPIO_PIN_10, USART1_IRQn},
            {USART2, GPIOA, LL_GPIO_PIN_2, LL_GPIO_PIN_3, USART2_IRQn},
            {USART3, GPIOB, LL_GPIO_PIN_10, LL_GPIO_PIN_11, USART3_IRQn}
        }};
        constexpr auto conf = configurationsTable[uartN-1];
        static_assert(uartN < 3, "Wrong USART number requested!");
        return Uart(conf, txDMA, bauld, dataBits, stopBits, parity);
    };

    void configUart(uint32_t baud, uint32_t dataBits, uint32_t stopBits, uint32_t parity);

    void putsAsync(const char * cstr, uint16_t len = 0);
    void putc(char byte);
    void puts(const char * cstr);

    auto rcvGetCount(void) -> uint16_t;
    auto rcvGetData(void) -> uint8_t *;
    void rcvClear(void);

    Uart(const Uart&) = delete;             /**< Delete copy constructor. */
    Uart& operator=(const Uart&) = delete;  /**< Delete assignment operator. */

private:

    /**
     * Configuration of USART device.
     * Needed to configure driver instance accordingly.
     */
    struct Config {

        USART_TypeDef * uartPort;   /**< USART1 ... USART3 */
        GPIO_TypeDef * gpioPort;    /**< GPIO port used */
        uint32_t txPin;             /**< GPIO pin used for output */
        uint32_t rxPin;             /**< GPIO pin used for input */
        IRQn_Type irqN;             /**< Interrupt number */
    };

    Uart( Config conf,
          DMA& txDMA,
          uint32_t bauld,
          uint32_t dataBits,
          uint32_t stopBits,
          uint32_t parity);

    
    static constexpr size_t buffersSize = 100;  /**< Internal buffers size */
    DMA& txDMA;                     /**< Reference to used dma instance */
    const Config conf;              /**< USART device configuration */

    std::array<uint8_t, buffersSize> outputBuffer;
    std::array<uint8_t, buffersSize> inputBuffer;
    uint16_t inputBufCnt = 0;

    // static pointers for interrupt handlers
    static volatile uint8_t * uart1InBufPtr;
    static volatile uint8_t * uart2InBufPtr;
    static volatile uint8_t * uart3InBufPtr;
    static volatile uint16_t * uart1InBufCntPtr;
    static volatile uint16_t * uart2InBufCntPtr;
    static volatile uint16_t * uart3InBufCntPtr;

};
