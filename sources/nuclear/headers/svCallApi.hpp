/**
 * @file    svCallApi.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    24-03-2019
 * @brief   BoSs Supervisor Call API
 * @details   Declarations of svCallApi
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include "thread.hpp"

/**
 * BoSs Supervisor Call API
 */
// namespace svCallApi {

extern "C" {

extern void SaveContext(Thread * t);

extern void svCallSleep(void);

}
// } // namespace svCallApi
