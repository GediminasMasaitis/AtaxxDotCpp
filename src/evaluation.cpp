#include "evaluation.h"
#include "attacks.h"

#include <array>
#include <cmath>

using namespace std;

Score evaluate_color(const PositionBase& pos, Color color)
{
    Score score = 0;
    auto pieces = pos.Bitboards[color];

    while (pieces)
    {
        const auto sq = pop_lsb(pieces);

        // MATERIAL
        score += 100;

        // UNATTACKABLE
        const Bitboard empty_near_sq = Attacks.near[sq] & pos.Bitboards[Pieces::Empty];
        if(!empty_near_sq)
        {
            score += 50;
        }
    }

    return score;
}

Score EvaluationHce::evaluate(const PositionBase& pos)
{
    Score score = 0;

    // TEMPO
    
    score += 400;
    
    score += evaluate_color(pos, pos.Turn);
    score -= evaluate_color(pos, !pos.Turn);
    return score;
}

Score EvaluationHce::evaluate_from_pov(const PositionBase& pos, Color color)
{
    Score score = evaluate(pos);
    if(pos.Turn != color)
    {
        score = static_cast<Score>(-score);
    }
    return score;
}

static double sigmoid(const double val)
{
    const auto res = 1 / (1 + exp(-val));
    return res;
}

Score EvaluationNnue::evaluate(const PositionBase& pos)
{
    auto weights = array<double, 128>{
        0.4892, 0.5063, -0.1125, 0.4744, 0.6698, -0.6034, 0.1504, 0.0786,
        0.3545, 0.1313, 0.6355, -0.3518, 0.3672, -0.5341, 0.5568, -0.0217,
        0.1254, 0.0463, -0.0536, -0.2152, 0.0541, -0.1041, 0.9962, -0.0487,
        0.2488, 0.0222, 0.0199, -0.3517, -0.2339, -0.3633, 0.4261, 0.0474,
        -0.1005, 0.0778, 0.1620, 0.1074, 0.6056, -0.1620, 0.6378, -0.0155,
        0.4618, -0.2209, 0.2862, 0.2597, -0.0715, 0.2665, -0.1600, 0.0744,
        0.0342, 0.2575, 0.4453, 0.1882, 0.5605, 0.0848, -0.0808, -0.0099,
        0.0643, -0.0266, -0.0279, -0.0213, 0.0642, 0.0261, -0.0766, -0.0516,
        0.1825, -0.3184, -0.3480, -0.4704, 0.2427, -1.2188, -0.4049, 0.0289,
        0.2788, -0.1800, 0.3746, 0.2179, -0.0262, -0.3182, -0.1981, -0.0372,
        -0.0549, -0.3075, 0.0894, -0.3445, -0.4810, -0.4097, -0.2297, 0.0880,
        -0.1061, -0.2262, -0.1480, -0.6308, -0.2203, -0.2801, -0.1535, 0.0661,
        -0.9183, 0.1174, -0.1191, 0.0429, -0.0646, 0.2889, -0.5271, 0.0342,
        0.1845, -0.4257, 0.2944, 0.1047, -0.0259, -0.1928, -0.2777, -0.0204,
        -0.8370, -0.0377, -0.2879, -0.3589, 1.0597, -0.5692, -0.8773, 0.0081,
        -0.0443, -0.0378, -0.0129, -0.0017, 0.0257, -0.0411, -0.0343, -0.0271
    };
    auto bias = 0.9548;

    auto inputs = std::array<double, 128>{0};
    for(auto i = 0; i < 64; i++)
    {
        if(pos.Bitboards[Pieces::White] & (1ULL << i))
        {
            inputs[i] = 1;
        }
        else if(pos.Bitboards[Pieces::Black] & (1ULL << i))
        {
            inputs[i + 64] = 1;
        }
    }

    double score = 0;
    for(auto i = 0; i < 128; i++)
    {
        score += inputs[i] * weights[i];
    }
    score += bias;
    score *= 128;

    if(pos.Turn == Colors::Black)
    {
        score = -score;
    }

    const auto final_score = static_cast<Score>(round(score));
    return final_score;
}

Score EvaluationNnue::evaluate_from_pov(const PositionBase& pos, Color color)
{
    Score score = evaluate(pos);
    if (pos.Turn != color)
    {
        score = static_cast<Score>(-score);
    }
    return score;
}
