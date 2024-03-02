#ifndef EVALUATION_H
#define EVALUATION_H 1

#include "types.h"
#include "position.h"

struct EvaluationHce
{
    static void init();
    static Score evaluate(const PositionBase& pos);
    static Score evaluate_from_pov(const PositionBase& pos, Color color = Colors::White);
};

struct EvaluationNnue
{
    static void init();
    static float evaluate_inner(const PositionBase& pos);
    static float evaluate_sigmoid(const PositionBase& pos);
    static Score evaluate(const PositionBase& pos);
    static Score evaluate_from_pov(const PositionBase& pos, Color color = Colors::White);
};

//using Evaluation = EvaluationHce;
using Evaluation = EvaluationNnue;

#endif // !EVALUATION_H
