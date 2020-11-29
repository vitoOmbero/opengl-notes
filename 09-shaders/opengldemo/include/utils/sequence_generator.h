#ifndef SEQUENCEGENERATOR_H
#define SEQUENCEGENERATOR_H

#include <cstdint>

class SequenceGenerator
{
public:
    SequenceGenerator();
    uint32_t getNext();
    uint32_t seeCurrent();

private:
    uint32_t counter_{ 0 };
};

inline static SequenceGenerator gRenderingTargetIdGen;

#endif // SEQUENCEGENERATOR_H
