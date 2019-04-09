/**
 * @file    thread.hpp
 * @author  Denis Homutovski
 * @version V1.0.2
 * @date    17-03-2019
 * @brief   Thread class
 * @details   Implementation of thread instance
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include <cstdint>
#include <boost/intrusive/slist.hpp>

using namespace boost::intrusive;

/**
 * Class implement thread instance abstraction.
 * Some parts of code are used from Asembler, so change carefully.
 */
class Thread {

public:

    typedef void (*TaskType)(void);     /**< Thread task function type */
    
    enum class State {      /**< Enumeration of thread states */

        initialized = 0,    /**< new thread initialized and ready to run */
        waiting,            /**< waiting in queue */
        active,             /**< active */
        sleep,              /**< sleeping in queue */
        blocked,            /**< blocked with mutex/semafore */
        killed              /**< thread is dead! */
    };

    // auto lambda = +[](int result, const char* str);
    Thread(TaskType task, uint32_t stackSizeWords, uint32_t * allocatedStack);
    
    auto getStackPointer() const { return stackPointer; };

    auto getState() const { return state; };
    void setState(State newState) { this->state = newState; };

    auto getSleepTicks() { return sleepTicks; };
    void setSleepTicks(uint32_t ticks) { this->sleepTicks = ticks; };

    auto operator==(const Thread& t) const { return stackPointer == t.stackPointer; }
    auto operator!=(const Thread& t) const { return !(*this == t); }

    // join()
    // wait()
    
private:

    uint32_t * stackPointer;            /**< must be on first place! used from asembler */

    // uint8_t pid;                     /**< process Id */
    State state;                        /**< current state */
    uint32_t sleepTicks;                /**< amount of SysTicks to sleep */

    constexpr static uint32_t onReturnProc = 333; /**< just a MagicInt dummy implementation */

public:

    slist_member_hook<> slist_hook_;    /**< Boost singly linked list hook */
    /**
     * Can't use inheritance because 'stackPointer'
     * has to be in first byte of Class data for correct work
     * of assembler writen contex switcher
     */

};

/** Define an slist that will store Thread using the public member hook */
typedef member_hook <Thread, slist_member_hook<>, &Thread::slist_hook_> ThreadSlistMemberOption;
typedef slist <Thread, ThreadSlistMemberOption, cache_last <true>> ThreadList;
