#ifndef EVALUATION_H
#define EVALUATION_H 1

#include "evaluation_hce.h"
#include "evaluation_nn.h"

#if NNUE
using Evaluation = EvaluationNnue;
#else
using Evaluation = EvaluationHce;
#endif

#endif // !EVALUATION_H
