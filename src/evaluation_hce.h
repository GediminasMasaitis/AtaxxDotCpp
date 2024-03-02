#ifndef EVALUATION_HCE_H
#define EVALUATION_HCE_H 1

#include "types.h"
#include "position.h"

struct EvaluationHce
{
    static void init();
    static Score evaluate(const PositionBase& pos);
    static Score evaluate_from_pov(const PositionBase& pos, Color color = Colors::White);
};

#endif // !EVALUATION_HCE_H
