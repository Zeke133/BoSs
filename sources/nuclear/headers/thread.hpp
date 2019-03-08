/**
 * @file    thread.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    19-12-2019
 * @brief   Thread class
 * @details   Implementation of thread instance
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include <cstdint>
#include <functional>

using std::uint32_t;

/**
 * Class implement thread instance abstraction.
 * Some parts of code are used from Asembler, so change carefully.
 */
class Thread {

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

    // auto lambda = +[](int result, const char* str);
    Thread(taskType task, uint32_t stackSizeWords, uint32_t * allocatedStack);

    // join()
    // wait()
    // kill()
    
    uint32_t * getStackTop() const { return stackTop; };

    Thread * getNext() const { return this->next; };
    void setNext(Thread * nextThread) { this->next = nextThread; };

    State getState() const { return state; };
    void setState(State newState) { this->state = newState; };

    uint32_t getSleepTicks() { return sleepTicks; };
    void setSleepTicks(uint32_t ticks) { this->sleepTicks = ticks; };

private:

    uint32_t * stackTop;                /**< must be on first place! used from asembler */
    Thread * next = nullptr;            /**< next thread in linked list */
    State state;                        /**< current state */
    uint32_t sleepTicks;                /**< amount of SysTicks to sleep */

    constexpr static uint32_t onReturnProc = 333; /**< just a MagicInt dummy implementation */
    
};
