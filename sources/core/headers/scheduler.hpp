/**
 * @file    scheduler.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    18-02-2018
 * @brief   Scheduler class
 * @details   Implementation of threads planning
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

// using
#include <stdint.h>

/**
 * Class header code-style template.
 * Here put detailed description ...
 * ...
 */
class Scheduler {

public:

    /**
     * Specifies decisions of scheduler to be fulfilled in interrupt handler
     */
    enum class Decision : uint8_t {
        noAction = 0,   /// context switch is not needed
        onlyRestore,    /// context just need to be restored from thread stack
        saveAndRestore, /// context need to be saved, thread switched to next
                        /// and new context restored

        LAST_ELEMENT
    };

    static void execute(void) const;

    static void stepPointers(void) const;
    // rename this to better ;)

    // ClassTempate(/*args*/);
    // ClassTempate(const ClassTempate&) = delete;             /**< Delete copy constructor. */
    // ClassTempate& operator=(const ClassTempate&) = delete;  /**< Delete assignment operator. */

    // void publicFunction(void) const;

private:

    static Decision lastDecision;       /**< Last decision of scheduler */

    // void privateFunction(void);

    // /**
    //  * Private struct or bit-field example
    //  */
    // struct PrivateBitField {

    //     uint32_t element1 : 2;
    //     /**< description of element1 */
    // };

    // /** Private constant. */ 
    // const uint32_t constant = 1000;

    // static uint8_t staticAtribute1;     /**< Description. */
    // static uint8_t staticAtribute2;     /**< Description. */
    
    // /**
    //  * long description
    //  * of attribute
    //  */
    // uint8_t * someAttribute;
    
    // PrivateBitField structAtribute;     /**< Attribute. */
    
};

#endif
