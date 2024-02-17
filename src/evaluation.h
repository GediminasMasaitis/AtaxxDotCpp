#ifndef EVALUATION_H
#define EVALUATION_H 1

#include "types.h"
#include "position.h"

struct Evaluation
{
    static Score evaluate(const PositionBase& pos);
    static Score evaluate_from_pov(const PositionBase& pos, Color color = Colors::White);
};

#endif // !EVALUATION_H
