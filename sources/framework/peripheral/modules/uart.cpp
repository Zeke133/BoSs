/**
 * @file    uart.cpp
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

#include "uart.hpp"

/**
 * Static pointers to internal buffers for interrupt handlers
 */
volatile uint8_t * Uart::uart1InBufPtr = 0;
volatile uint8_t * Uart::uart2InBufPtr = 0;
volatile uint8_t * Uart::uart3InBufPtr = 0;
volatile uint16_t * Uart::uart1InBufCntPtr = 0;
volatile uint16_t * Uart::uart2InBufCntPtr = 0;
volatile uint16_t * Uart::uart3InBufCntPtr = 0;

/**
 * Interrupt handlers
 */
void USART1_IRQHandler(void) {

    if (USART1->SR & USART_SR_RXNE) {
        if (*Uart::uart1InBufCntPtr < Uart::buffersSize) {
            
            Uart::uart1InBufPtr[(*Uart::uart1InBufCntPtr)++] = LL_USART_ReceiveData8(USART1);
        }
        else LL_USART_ReceiveData8(USART1);
    }
}
void USART2_IRQHandler(void) {

    if (USART2->SR & USART_SR_RXNE) {
        if (*Uart::uart2InBufCntPtr < Uart::buffersSize) {

            Uart::uart2InBufPtr[(*Uart::uart2InBufCntPtr)++] = LL_USART_ReceiveData8(USART2);
        }
        else LL_USART_ReceiveData8(USART2);
    }
}
void USART3_IRQHandler(void) {

    if (USART3->SR & USART_SR_RXNE) {
        if (*Uart::uart3InBufCntPtr < Uart::buffersSize) {
            
            Uart::uart3InBufPtr[(*Uart::uart3InBufCntPtr)++] = LL_USART_ReceiveData8(USART3);
        }
        else LL_USART_ReceiveData8(USART3);
    }
}

/**
 * @brief  Driver constructor.
 * @param  conf: device configuration
 * @param  txDMA: reference to used dma instance
 * @param  bauld: baud rate
 * @param  dataBits: data bits
 * @param  stopBits: stop bits
 * @param  parity: parity
 */
Uart::Uart(Config c, DMA& txDma, uint32_t bauld, uint32_t dataBits, uint32_t stopBits, uint32_t parity)
    :   txDMA(txDma),
        conf(c) {

    if (conf.uartPort == USART1) {

        uart1InBufPtr = inputBuffer.data();
        uart1InBufCntPtr = &inputBufCnt;
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
    }
    else if (conf.uartPort == USART2) {
    
        uart2InBufPtr = inputBuffer.data();
        uart2InBufCntPtr = &inputBufCnt;
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    }
    else if (conf.uartPort == USART3) {
    
        uart3InBufPtr = inputBuffer.data();
        uart3InBufCntPtr = &inputBufCnt;
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
    }
    else {
        // assert ???
    }

    GPIO::enableAPB2Clock(conf.gpioPort);
    // Configure Tx as alternate function push-pull
    LL_GPIO_SetPinMode(conf.gpioPort, conf.txPin, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinPull(conf.gpioPort, conf.txPin, LL_GPIO_PULL_DOWN);
    LL_GPIO_SetPinSpeed(conf.gpioPort, conf.txPin, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(conf.gpioPort, conf.txPin, LL_GPIO_OUTPUT_PUSHPULL);
    // Configure Rx as input floating    
    LL_GPIO_SetPinMode(conf.gpioPort, conf.rxPin, LL_GPIO_MODE_FLOATING);
    LL_GPIO_SetPinPull(conf.gpioPort, conf.txPin, LL_GPIO_PULL_DOWN);

    // Configure UART
    configUart(bauld, dataBits, stopBits, parity);
    LL_USART_Enable(conf.uartPort);

    // Configure DMA
    LL_USART_EnableDMAReq_TX(conf.uartPort);

    // Enable the usart Receive interrupt: on usart receive data register is not empty
    LL_USART_EnableIT_RXNE(conf.uartPort);

    // Global usart irq enable
    NVIC_EnableIRQ(conf.irqN);
}

/**
 * @brief  Usart configuration.
 * @param  bauld: baud rate
 * @param  dataBits: data bits
 * @param  stopBits: stop bits
 * @param  parity: parity
 * @retval None
 */
void Uart::configUart(uint32_t baud, uint32_t dataBits, uint32_t stopBits, uint32_t parity) {

    LL_USART_InitTypeDef params {

        .BaudRate = baud,
        .DataWidth = dataBits,
        .StopBits = stopBits,
        .Parity = parity,
        .TransferDirection = LL_USART_DIRECTION_TX_RX,
        .HardwareFlowControl = LL_USART_HWCONTROL_NONE
        // OverSampling = LL_USART_OVERSAMPLING_16
    };
    LL_USART_Init(conf.uartPort, &params);
}

/**
 * @brief  Transmit single byte.
 * @param  byte: byte to transmit
 * @retval None
 */
void Uart::putc(char byte) {

    while(!LL_USART_IsActiveFlag_TXE(conf.uartPort)) ;
    LL_USART_TransmitData8(conf.uartPort, byte);
    while(!LL_USART_IsActiveFlag_TC(conf.uartPort)) ;
}

/**
 * @brief  Transmit null terminated C-string.
 * @param  cstr: pointer to C-string
 * @retval None
 */
void Uart::puts(const char * cstr) {

    while (*cstr) {
        while(!LL_USART_IsActiveFlag_TXE(conf.uartPort)) ;
        LL_USART_TransmitData8(conf.uartPort, *cstr++);
    }
    while(!LL_USART_IsActiveFlag_TC(conf.uartPort)) ;
}

/**
 * @brief  Transmit null terminated C-string
 *         or specified length buffer asynchronously.
 *         Uses DMA for transfer.
 *         Has internal wait if device is busy.
 * @param  str: pointer to C-string
 * @param  len: 0 - if null terminated string shoud be transfered
 *         or length of bytes to transmit
 * @retval None
 */
void Uart::putsAsync(const char * cstr, uint16_t len) {

    while(!LL_USART_IsActiveFlag_TC(conf.uartPort)) ;
    
    if (len == 0) {

        for( ; cstr[len] != 0; ++len) {

            outputBuffer[len] = cstr[len];
        }
    } else {

        for(int i = 0; i < len; ++i) {

            outputBuffer[i] = cstr[i];
        }
    }

    // LL_USART_DMA_GetRegAddr(conf.uartPort);
    txDMA.runTransfer((void*)&conf.uartPort->DR, outputBuffer.data(), len);
}

/**
 * @brief  Clear internal receive buffer.
 * @retval None
 */
void Uart::rcvClear() {

    inputBufCnt = 0;
}

/**
 * @brief  Get amount of bytes in internal receive buffer.
 * @retval uint16_t
 */
auto Uart::rcvGetCount() -> uint16_t {

    return inputBufCnt;
}

/**
 * @brief  Get pointer to data internal receive buffer.
 * @retval uint8_t *
 */
auto Uart::rcvGetData() -> uint8_t * {

    return inputBuffer.data();
}
