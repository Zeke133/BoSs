/**
 * @file    dma.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    18-04-2019
 * @brief   DMA class
 * @details   Direct Memory Access controller driver for STM32F10x.
 * @pre       -
 * @bug       -
 * @warning   DMA2 - is not implemented, please add if needed.
 * @copyright GNU Public License.
 */

#pragma once

#include <stm32f1xx_ll_rcc.h>
#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_dma.h>

#include <array>

// IRQ handlers. Extern "C" macro is needed for correct link procedure.
extern "C" {
    void DMA1_Channel1_IRQHandler(void);
    void DMA1_Channel2_IRQHandler(void);
    void DMA1_Channel3_IRQHandler(void);
    void DMA1_Channel4_IRQHandler(void);
    void DMA1_Channel5_IRQHandler(void);
    void DMA1_Channel6_IRQHandler(void);
    void DMA1_Channel7_IRQHandler(void);
}

/**
 * Direct Memory Access controller driver class.
 * Each instance represents one DMA channel.
 * Class instance must be created with make_dma() function.
 * Here is no any hardware resources sharing protection,
 * so you must not create more than one instance for each physical DMA channel.
 * This implementation uses interrupts and able to execute user-defined
 * callback functions with specified actions.
 */
class DMA {

    /**< Interrupt handlers need access to 'callbacks' */
    friend void DMA1_Channel1_IRQHandler(void);
    friend void DMA1_Channel2_IRQHandler(void);
    friend void DMA1_Channel3_IRQHandler(void);
    friend void DMA1_Channel4_IRQHandler(void);
    friend void DMA1_Channel5_IRQHandler(void);
    friend void DMA1_Channel6_IRQHandler(void);
    friend void DMA1_Channel7_IRQHandler(void);

public:

    /**
     * Serviced devices supported with DMA controller
     */
    enum class Device {

        // DMA1 devices
        ADC_1,

        SPI1_RX,
        SPI1_TX,

        SPI2_I2S2_RX,
        SPI2_I2S2_TX,

        USART3_TX,
        USART3_RX,

        USART1_TX,
        USART1_RX,

        USART2_RX,
        USART2_TX,

        I2C2_TX,
        I2C2_RX,
        I2C1_TX,
        I2C1_RX,

        TIM1_CH1,
        TIM1_CH4,
        TIM1_TRIG,
        TIM1_COM,
        TIM1_UP,
        TIM1_CH3,

        TIM2_CH3,
        TIM2_UP,
        TIM2_CH1,
        TIM2_CH2,
        TIM2_CH4,

        TIM3_CH3,
        TIM3_CH4,
        TIM3_UP,
        TIM3_CH1,
        TIM3_TRIG,

        TIM4_CH1,
        TIM4_CH2,
        TIM4_CH3,
        TIM4_UP,

        // DMA2 devices
        // ADC3,

        SPI_I2S3_RX,
        SPI_I2S3_TX,

        UART4_RX,
        UART4_TX,

        // SDIO,

        TIM5_CH4,
        TIM5_TRIG,
        TIM5_CH3,
        TIM5_UP,
        TIM5_CH2,
        TIM5_CH1,

        TIM6_UP_DAC_Channel1,

        TIM7_UP_DAC_Channel2,

        // TIM8_CH3,
        // TIM8_UP,
        // TIM8_CH4,
        // TIM8_TRIG,
        // TIM8_COM,
        // TIM8_CH1,
        // TIM8_CH2

        // end
        LAST_ELEMENT
    };

    using CallbackFunc = void (*)(void);    /**< Callback function type. */

    /**
     * @brief  Creates instance of class.
     *         Puts required configuration to constructor depending
     *         on user chosen device to be serviced.
     * @param  dev: enum Device device to be serviced qith this instance.
     * @retval DMA instance
     */
    template<Device dev>
    static constexpr auto make_dma() {

        /**
         * Table of serviced devices descriptors.
         * Maps each 'enum DMA::Device' instance to 'DMA::DMAConfig'.
         */
        constexpr std::array
                            <
                                DMAConfig,
                                static_cast<unsigned int>(Device::LAST_ELEMENT)
                            >
                            configurationsTable = {{

            {DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel1_IRQn},    /// ADC,
            {DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel2_IRQn},    /// SPI1_RX,
            {DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH, DMA1_Channel3_IRQn},    /// SPI1_TX,
            {DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel4_IRQn},    /// SPI2/I2S2_RX,
            {DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_MEMORY_TO_PERIPH, DMA1_Channel5_IRQn},    /// SPI2/I2S2_TX,
            {DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH, DMA1_Channel2_IRQn},    /// USART3_TX,
            {DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel3_IRQn},    /// USART3_RX,
            {DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH, DMA1_Channel4_IRQn},    /// USART1_TX,
            {DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel5_IRQn},    /// USART1_RX,
            {DMA1, LL_DMA_CHANNEL_6, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel6_IRQn},    /// USART2_RX,
            {DMA1, LL_DMA_CHANNEL_7, LL_DMA_DIRECTION_MEMORY_TO_PERIPH, DMA1_Channel7_IRQn},    /// USART2_TX,
            {DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH, DMA1_Channel4_IRQn},    /// I2C2_TX,
            {DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel5_IRQn},    /// I2C2_RX,
            {DMA1, LL_DMA_CHANNEL_6, LL_DMA_DIRECTION_MEMORY_TO_PERIPH, DMA1_Channel6_IRQn},    /// I2C1_TX,
            {DMA1, LL_DMA_CHANNEL_7, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel7_IRQn},    /// I2C1_RX,
            {DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel2_IRQn},    /// TIM1_CH1,
            {DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel4_IRQn},    /// TIM1_CH4,
            {DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel4_IRQn},    /// TIM1_TRIG,
            {DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel4_IRQn},    /// TIM1_COM,
            {DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel5_IRQn},    /// TIM1_UP,
            {DMA1, LL_DMA_CHANNEL_6, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel6_IRQn},    /// TIM1_CH3,
            {DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel1_IRQn},    /// TIM2_CH3,
            {DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel2_IRQn},    /// TIM2_UP,
            {DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel5_IRQn},    /// TIM2_CH1,
            {DMA1, LL_DMA_CHANNEL_7, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel7_IRQn},    /// TIM2_CH2,
            {DMA1, LL_DMA_CHANNEL_7, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel7_IRQn},    /// TIM2_CH4,
            {DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel2_IRQn},    /// TIM3_CH3,
            {DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel3_IRQn},    /// TIM3_CH4,
            {DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel3_IRQn},    /// TIM3_UP,
            {DMA1, LL_DMA_CHANNEL_6, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel6_IRQn},    /// TIM3_CH1,
            {DMA1, LL_DMA_CHANNEL_6, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel6_IRQn},    /// TIM3_TRIG
            {DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel1_IRQn},    /// TIM4_CH1,
            {DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel4_IRQn},    /// TIM4_CH2,
            {DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel5_IRQn},    /// TIM4_CH3,
            {DMA1, LL_DMA_CHANNEL_7, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel7_IRQn},    /// TIM4_UP
                                                                                                /// ADC3,
            // {DMA2, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel1_IRQn}  /// SPI_I2S3_RX,
            // {DMA2, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH, DMA1_Channel2_IRQn}  /// SPI_I2S3_TX,
            // {DMA2, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel3_IRQn}  /// UART4_RX,
            // {DMA2, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_MEMORY_TO_PERIPH, DMA1_Channel5_IRQn}  /// UART4_TX,
                                                                                                /// SDIO
            // {DMA2, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel1_IRQn}  /// TIM5_CH4,
            // {DMA2, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel1_IRQn}  /// TIM5_TRIG,
            // {DMA2, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel2_IRQn}  /// TIM5_CH3,
            // {DMA2, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel2_IRQn}  /// TIM5_UP,
            // {DMA2, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel4_IRQn}  /// TIM5_CH2,
            // {DMA2, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel5_IRQn}  /// TIM5_CH1,
            // {DMA2, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel3_IRQn}  /// TIM6_UP_DAC_Channel1,
            // {DMA2, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_PERIPH_TO_MEMORY, DMA1_Channel4_IRQn}  /// TIM7_UP_DAC_Channel2,
                                                                                                /// TIM8_CH3,
                                                                                                /// TIM8_UP,
                                                                                                /// TIM8_CH4,
                                                                                                /// TIM8_TRIG,
                                                                                                /// TIM8_COM,
                                                                                                /// TIM8_CH1,
                                                                                                /// TIM8_CH2
        }};
        constexpr auto conf = configurationsTable[static_cast<int>(dev)];
        return DMA(conf.controller, conf.channel, conf.direction, conf.irqN);
    };

    void runTransfer(void * perephPtr, const uint8_t * memoryPtr, uint32_t size) const;
    void setCallbackOnTransferComplete(CallbackFunc func) const;

    DMA(const DMA&) = delete;               /**< Delete copy constructor. */
    DMA& operator=(const DMA&) = delete;    /**< Delete assignment operator. */

private:

    DMA(DMA_TypeDef * dma, uint32_t channel, uint32_t direction, IRQn_Type irqN);

    void stopTransfer() const;
    auto isTransferComplete() const -> bool;

    static CallbackFunc callbacks[7];
    
    /**
     * Configuration of devices serviced with DMA.
     * Needed to configure driver instance accordingly.
     */
    struct DMAConfig {

        DMA_TypeDef * controller;   /**< DMA1 */
        uint32_t channel;           /**
                                     * LL_DMA_CHANNEL_1
                                     * LL_DMA_CHANNEL_2
                                     * LL_DMA_CHANNEL_3
                                     * LL_DMA_CHANNEL_4
                                     * LL_DMA_CHANNEL_5
                                     * LL_DMA_CHANNEL_6
                                     * LL_DMA_CHANNEL_7
                                     */
        uint32_t direction;         /**
                                     * LL_DMA_DIRECTION_PERIPH_TO_MEMORY
                                     * LL_DMA_DIRECTION_MEMORY_TO_PERIPH
                                     * LL_DMA_DIRECTION_MEMORY_TO_MEMORY
                                     */
        IRQn_Type irqN;             /**
                                     * DMA1_Channel1_IRQn
                                     * DMA1_Channel2_IRQn
                                     * DMA1_Channel3_IRQn
                                     * DMA1_Channel4_IRQn
                                     * DMA1_Channel5_IRQn
                                     * DMA1_Channel6_IRQn
                                     * DMA1_Channel7_IRQn
                                     */
    };
    
    DMAConfig config;               /**< Current channel configuration. */

};
