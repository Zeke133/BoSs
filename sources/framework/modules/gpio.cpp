#include <gpio.h>

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
void GPIO::enableAPB2Clock(GPIO_TypeDef * port) {

    uint32_t APB2_Periph;

    if (port == GPIOA) APB2_Periph = LL_APB2_GRP1_PERIPH_GPIOA;
    else if (port == GPIOB) APB2_Periph = LL_APB2_GRP1_PERIPH_GPIOB;
    else if (port == GPIOC) APB2_Periph = LL_APB2_GRP1_PERIPH_GPIOC;
    else if (port == GPIOD) APB2_Periph = LL_APB2_GRP1_PERIPH_GPIOD;
    else if (port == GPIOE) APB2_Periph = LL_APB2_GRP1_PERIPH_GPIOE;
    else return;

    // Turn ON Clock for specified port
    LL_APB2_GRP1_EnableClock(APB2_Periph);
}
