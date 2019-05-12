/**
 * @file    IProfiler.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    11-04-2019
 * @brief   Profiler interface.
 * @details   -
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

/**
 * Interface for profiling and performance measuring.
 */
class IProfiler {

public:

    virtual void startProfiling(void) = 0;
    virtual unsigned int getExecutionTicks(void) = 0;

};
