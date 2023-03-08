#ifndef EVALUATION_H
#define EVALUATION_H 1

#include "types.h"
#include "position.h"

struct Evaluation
{
    static Score evaluate(const Position& pos);
};

#endif // !EVALUATION_H
