#include "utils/sequence_generator.h"

SequenceGenerator::SequenceGenerator() {}

uint32_t SequenceGenerator::getNext()
{
    return ++counter_;
}

uint32_t SequenceGenerator::seeCurrent()
{
    return counter_;
}
