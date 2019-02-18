
#include <cstdint>
// #include <functional>
// #include <bitset>

// template <stackSizeWords>
class Thread {

public:

    typedef void (*task_t)(void);
    enum class State : uint8_t {

        initialized = 0,    // initialized and ready to run
        paused,             // 
        running,            // 

        LastElement         // for compile-time out of range check
    };

    Thread(task_t task, uint32_t stackSizeWords, uint32_t * allocatedStack);

    void setNext(Thread * nextThread) { this->next = nextThread; };
    Thread * getNext() const { return this->next; };

    void setState(State newState) { this->state = newState; };
    State getState() const { return state; }

private:

    uint32_t * stackTop;                // must be on first place!
    Thread * next = nullptr;            // next thread in linked list
    State state;                        // current state

    constexpr static uint32_t onReturnProcedurePtr = 0xEEEE; // just a MagicInt dummy implementation
    
};

