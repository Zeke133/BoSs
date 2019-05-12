/**
 * @file    ITextInput.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    15-04-2019
 * @brief   Text input interface.
 * @details   -
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

/**
 * Interface for text input.
 */
class ITextInput {

public:

    virtual char getChar(void) = 0;

};
