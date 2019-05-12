/**
 * @file    ITextOutput.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    15-04-2019
 * @brief   Text output interface.
 * @details   -
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

/**
 * Interface for text output.
 */
class ITextOutput {

public:

    virtual void putc(char byte) = 0;
    virtual void puts(const char *str) = 0;

};
