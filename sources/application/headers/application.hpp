/**
 * @file    application.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    10-03-2019
 * @brief   Application class
 * @details   Implementation of application
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include <cstdint>

#include "IBitBanding.hpp"

#include "thread.hpp"
#include "scheduler.hpp"

#include "delay.hpp"
#include "led.hpp"
#include "sysClock.hpp"
#include "dma.hpp"
#include "uart.hpp"

using std::uint32_t;
