/**
 * @file    sleeping.cpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    12-03-2019
 * @brief   Sleeping class
 * @details   Implementation of sleeping threads queue
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#include "sleeping.hpp"

/**
 * @brief  Thread constructor
 * @param  task: pointer to thread task function
 * @param  stackSizeWords: size of thread stack in words (word = 4 byte)
 * @param  allocatedStack: pointer to memory allocated for thread stack 
 * @retval None
 */

