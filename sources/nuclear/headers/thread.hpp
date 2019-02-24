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
 * Class implement thread instance abstraction.
 * Some parts of code are used from Asembler, so change carefully.
 */
class Thread {

friend class Scheduler;

public:

    /** Thread task function type */
    typedef void (*taskType)(void);
    /**
     * Enumeration of thread states
     */
    enum class State : uint8_t {

        initialized = 0,    /**< thread initialized and ready to run */
        paused,             /**< thread is waiting */
        running,            /**< thread is active */
    };

    Thread(taskType task, uint32_t stackSizeWords, uint32_t * allocatedStack);

    // join()
    // wait()
    // kill()
    
    uint32_t * getStackTop() const { return stackTop; };
    State getState() const { return state; };
    Thread * getNext() const { return this->next; };

private:

    void setState(State newState) { this->state = newState; };
    void setNext(Thread * nextThread) { this->next = nextThread; };

    uint32_t * stackTop;                /**< must be on first place! used from asembler */
    Thread * next = nullptr;            /**< next thread in linked list */
    State state;                        /**< current state */

    constexpr static uint32_t onReturnProc = 333; /**< just a MagicInt dummy implementation */
    
};

#endif
