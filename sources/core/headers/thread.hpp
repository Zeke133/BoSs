
#include <cstdint>

#include <functional>

class Thread {

public:

<<<<<<< HEAD
=======
// private:

>>>>>>> f76522a622cf114e3941f203dd3f4b2c0f7273df
    uint32_t * stackTop;

    uint8_t bits;

    Thread * nextTask;

    void create(std::function<void()> task, uint32_t stackSize, uint32_t * allocatedStack, uint8_t param);

<<<<<<< HEAD
private:

=======
>>>>>>> f76522a622cf114e3941f203dd3f4b2c0f7273df
}
