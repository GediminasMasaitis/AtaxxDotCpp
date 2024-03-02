#ifndef EVALUATION_NN_H
#define EVALUATION_NN_H 1

#include "types.h"
#include "position.h"

struct EvaluationNnue
{
    static void init();
    static Score evaluate(const PositionBase& pos);
    static Score evaluate_from_pov(const PositionBase& pos, Color color = Colors::White);
};

#endif // !EVALUATION_NN_H
