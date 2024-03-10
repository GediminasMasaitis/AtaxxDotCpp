#ifndef EVALUATION_NN_H
#define EVALUATION_NN_H 1

#include "types.h"
#include "position.h"

struct EvaluationNnue
{
    static Score evaluate(const Position& pos);
    static Score evaluate_from_pov(const Position& pos, Color color = Colors::White);
};

#endif // !EVALUATION_NN_H
