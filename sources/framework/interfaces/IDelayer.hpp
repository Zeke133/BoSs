/**
 * @file    IDelayer.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    11-04-2019
 * @brief   Delayer interface.
 * @details   -
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

/**
 * Interface to provide microseconds time delays functionality in easy way.
 */
class IDelayer {

public:

    virtual void us(unsigned int us) = 0;

};
