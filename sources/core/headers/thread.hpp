
#include <cstdint>

#include <functional>

class Thread {

public:

// private:

    uint32_t * stackTop;

    uint8_t bits;

    Thread * nextTask;

    void create(std::function<void()> task, uint32_t stackSize, uint32_t * allocatedStack, uint8_t param);

}
