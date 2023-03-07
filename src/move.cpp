#include "move.h"

#include <iostream>
#include <string>

using namespace std;

MoveStr Move::to_move_str() const
{
    MoveStr to_str = to_square_str(To);
    if(From == no_square)
    {
        return to_str;
    }

    return to_square_str(From) + to_str;
}

Move Move::from_move_str(const Color color, const MoveStr& move_str)
{
    if(move_str.size() == 2)
    {
        return Move(color, no_square, from_square_str(move_str));
    }
    else if(move_str.size() == 4)
    {
        return Move(color, from_square_str(move_str.substr(0, 2)), from_square_str(move_str.substr(2, 2)));
    }
    else
    {
        do_throw("Invalid move string" + move_str);
    }
}
