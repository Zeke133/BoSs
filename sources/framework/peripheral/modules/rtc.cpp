/**
 * @file    rtc.cpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    11-04-2019
 * @brief   RealTimeClock class
 * @details   Internal real time clock driver
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#include "rtc.hpp"
#include "scheduler.hpp"
#include "convertation.hpp"

/**
 * @brief  Driver constructor.
 * @retval None
 */
RealTimeClock::RealTimeClock() {

    if (init()) {

        // On first RTC init: setting default dateTime: 01.01.2018 00:00:00
        constexpr Convertation::DateTime time {

            .hours = 0,
            .minutes = 0,
            .seconds = 0,
            .date = 1,
            .wday = 0,
            .month = 1,
            .year = 2019
        };
        constexpr unsigned int unixTime = Convertation::dateTime2Unix(time);

        // Must have a delay after RTC initialization to set a time
        Scheduler::sleep(500);
        setTime(unixTime);
    }
}

/**
 * @brief  RTC peripheral device initialisation.
 * @retval None
 */
auto RealTimeClock::init(void) const -> bool {

    // Enable tacting of PowerManagement and BackupManagement modules
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR | LL_APB1_GRP1_PERIPH_BKP);
    // Enable access to Backup data sector
    LL_PWR_EnableBkUpAccess();

    // Initialize RTC if it's OFF
    if (!LL_RCC_IsEnabledRTC()) {

        LL_RCC_ForceBackupDomainReset();
        LL_RCC_ReleaseBackupDomainReset();

        // Set clock source 32768 oscillator
        LL_RCC_LSE_Enable();
        
        while (!LL_RCC_LSE_IsReady()) ;
        LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);

        // Set divider to count seconds with RTC
        LL_RTC_SetAsynchPrescaler(RTC, 0x7FFF);

        // Turn ON RTC
        LL_RCC_EnableRTC();

        LL_RTC_WaitForSynchro(RTC);

        return true;
    }
    return false;
}

/**
 * @brief  Set clock time.
 * @param  uint32_t: time in UNIX format
 * @retval None
 */
void RealTimeClock::setTime(uint32_t t) const {

    LL_RTC_TIME_Set(RTC, t);
}

/**
 * @brief  Get clock time.
 * @retval uint32_t: time in UNIX format
 */
uint32_t RealTimeClock::getTime(void) const {

    return LL_RTC_TIME_Get(RTC);
}
