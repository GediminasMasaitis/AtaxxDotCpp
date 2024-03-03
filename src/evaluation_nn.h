#ifndef EVALUATION_NN_H
#define EVALUATION_NN_H 1

#include "types.h"
#include "position.h"

struct EvaluationNnue
{
    static Score evaluate(const PositionNnue& pos);
    static Score evaluate_from_pov(const PositionNnue& pos, Color color = Colors::White);
};

#endif // !EVALUATION_NN_H
