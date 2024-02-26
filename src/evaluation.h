#ifndef EVALUATION_H
#define EVALUATION_H 1

#include "types.h"
#include "position.h"

struct EvaluationHce
{
    static Score evaluate(const PositionBase& pos);
    static Score evaluate_from_pov(const PositionBase& pos, Color color = Colors::White);
};

struct EvaluationNnue
{
    static Score evaluate(const PositionBase& pos);
    static Score evaluate_from_pov(const PositionBase& pos, Color color = Colors::White);
};

using Evaluation = EvaluationHce;

#endif // !EVALUATION_H
