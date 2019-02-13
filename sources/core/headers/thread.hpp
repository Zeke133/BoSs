
#include <cstdint>

#include <functional>

class Thread {

public:

    uint32_t * stackTop;

    uint8_t bits;

    void create(void(*task)(), uint32_t stackSizeWords, uint32_t * allocatedStack);

private:

};

