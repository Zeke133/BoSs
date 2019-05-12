/**
 * @file    dma.cpp
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
  
#include "dma.hpp"

/**
 * Callback functions pointers being executed on interrupts
 * for each DMA channel.
 */
DMA::CallbackFunc DMA::callbacks[7] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

/**
 * Interrupt handlers
 */
void DMA1_Channel1_IRQHandler(void) {

    LL_DMA_ClearFlag_TC1(DMA1);
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
    if (DMA::callbacks[0] != nullptr) DMA::callbacks[0]();
}
void DMA1_Channel2_IRQHandler(void) {

    LL_DMA_ClearFlag_TC2(DMA1);
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
    if (DMA::callbacks[1] != nullptr) DMA::callbacks[1]();
}
void DMA1_Channel3_IRQHandler(void) {

    LL_DMA_ClearFlag_TC3(DMA1);
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
    if (DMA::callbacks[2] != nullptr) DMA::callbacks[2]();
}
void DMA1_Channel4_IRQHandler(void) {

    LL_DMA_ClearFlag_TC4(DMA1);
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
    if (DMA::callbacks[3] != nullptr) DMA::callbacks[3]();
}
void DMA1_Channel5_IRQHandler(void) {

    LL_DMA_ClearFlag_TC5(DMA1);
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);
    if (DMA::callbacks[4] != nullptr) DMA::callbacks[4]();
}
void DMA1_Channel6_IRQHandler(void) {

    LL_DMA_ClearFlag_TC6(DMA1);
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_6);
    if (DMA::callbacks[5] != nullptr) DMA::callbacks[5]();
}
void DMA1_Channel7_IRQHandler(void) {

    LL_DMA_ClearFlag_TC7(DMA1);
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);
    if (DMA::callbacks[6] != nullptr) DMA::callbacks[6]();
}

/**
 * @brief  Driver constructor.
 * @param  dma: dma device
 * @param  channel: device channel
 * @param  direction: transfer direction
 * @param  irqN: interrupt number
 */
DMA::DMA(DMA_TypeDef * dma, uint32_t channel, uint32_t direction, IRQn_Type irqN)
    : config({dma, channel, direction, irqN}) {

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
}

/**
 * @brief  Set callback function on transfer complete.
 *         Interrupt on transfer complete will be enabled.
 * @param  func: Pointer to user callback function.
 * @retval None
 */
void DMA::setCallbackOnTransferComplete(CallbackFunc func) const {

    callbacks[config.channel - 1] = func;
    // Enables DMA interrupt on transmition complete
    LL_DMA_EnableIT_TC(config.controller, config.channel);
    NVIC_EnableIRQ(config.irqN);
}

/**
 * @brief  Run data transmission.
 * @param  perephPtr: Pointer to perepheral device data register.
 * @param  memoryPtr: Pointer to memory buffer.
 * @param  size: Size of data to be transmited.
 * @retval None
 */
void DMA::runTransfer(void * perephPtr, const char * memoryPtr, unsigned int size) const {

    // If any transfer is already in progress - wait for finish and stop transfer
    if (LL_DMA_IsEnabledChannel(config.controller, config.channel)) {

        while (!isTransferComplete());
        stopTransfer();
    }
    
    LL_DMA_InitTypeDef params;

    params.PeriphOrM2MSrcAddress = reinterpret_cast<uint32_t>(perephPtr);
    params.MemoryOrM2MDstAddress = reinterpret_cast<uint32_t>(memoryPtr);

    params.Direction = config.direction;

    params.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
    params.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;

    params.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
    params.MemoryOrM2MDstDataSize = LL_DMA_PDATAALIGN_BYTE;

    params.Mode = LL_DMA_MODE_NORMAL;   // also LL_DMA_MODE_CIRCULAR is
    params.Priority = LL_DMA_PRIORITY_LOW;

    params.NbData = size;
    
    LL_DMA_Init(config.controller, config.channel, &params);
    LL_DMA_EnableChannel(config.controller, config.channel);
}

/**
 * @brief  Stop data transmission.
 * @retval None
 */
void DMA::stopTransfer() const {
    
    LL_DMA_DisableChannel(config.controller, config.channel);
}

/**
 * @brief  Check if transfer completed.
 * @retval bool
 */
auto DMA::isTransferComplete() const -> bool {

    // LL_DMA_IsActiveFlag_TC1(DMA_TypeDef *DMAx)
    uint32_t TCIF_bit = DMA_ISR_TCIF1 << ((config.channel-1)*4);

    return (READ_BIT(config.controller->ISR, TCIF_bit) == (TCIF_bit));
}
