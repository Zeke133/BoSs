/**
 * @file    thread.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    19-12-2018
 * @brief   Thread class
 * @details   Implementation of thread instance
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#ifndef _THREAD_H
#define _THREAD_H

// using
#include <cstdint>

/**
 * Class header code-style template.
 * Here put detailed description ...
 * ...
 */
// template <stackSizeWords>
class Thread {

friend class Scheduler;

public:

    /** Private constant. */
    typedef void (*task_t)(void);
    /**
     * Enumeration
     */
    enum class State : uint8_t {

        initialized = 0,    /// initialized and ready to run
        paused,             /// 
        running,            /// 

        LastElement         /// for compile-time out of range check
    };

    Thread(task_t task, uint32_t stackSizeWords, uint32_t * allocatedStack);

    // join

private:

    void setNext(Thread * nextThread) { this->next = nextThread; };
    Thread * getNext() const { return this->next; };

    void setState(State newState) { this->state = newState; };
    State getState() const { return state; }

    uint32_t * stackTop;                /**< must be on first place! used from asembler */
    Thread * next = nullptr;            /**< next thread in linked list */
    State state;                        /**< current state */

    constexpr static uint32_t onReturnProcedurePtr = 0xEEEE; /**< just a MagicInt dummy implementation */
    
};

#endif
